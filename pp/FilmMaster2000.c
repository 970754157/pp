#include "FilmMaster2000.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool read_film(const char* filename, Film* film) {//读文件的函数const数据类型不可改变 char*字符串由一堆字符组成的数组读到Film* film
    FILE* fp = fopen(filename, "rb");//打开文件以二进制模式
    if (!fp) return false;//newNode == NULL这个指针是空的

    // Read header
    if (fread(&film->header, sizeof(FilmHeader), 1, fp) != 1) {//从指针fp读到film header，每一个元素的大小，元素个数
        fclose(fp);//元素个数不等于1提取失败
        return false;
    }

    // Validate header看一下是否合理
    if (film->header.channels > MAX_CHANNELS ||
        film->header.height > MAX_HEIGHT ||
        film->header.width > MAX_WIDTH ||
        film->header.num_frames <= 0) {
        fclose(fp);
        return false;
    }

    // Allocate frames array
    film->frames = malloc(film->header.num_frames * sizeof(FilmFrame));//开辟一块内存空间
    if (!film->frames) {//无效的
        fclose(fp);
        return false;
    }

    // Calculate frame size计算每一帧的大小
    size_t pixels_per_channel = film->header.height * film->header.width;//每个通道的像素数量大小
    size_t frame_size = film->header.channels * pixels_per_channel;//总通道的像素数量大小

    // Read each frame
    for (long i = 0; i < film->header.num_frames; i++) {//遍历每一帧
        film->frames[i].size = frame_size;//每一帧都大小
        film->frames[i].data = malloc(frame_size);//保存每一帧的数据
        
        if (!film->frames[i].data) {//malloc失败了释放之前存储的指针数组
            // Cleanup previously allocated frames
            for (long j = 0; j < i; j++) {
                free(film->frames[j].data);
            }
            free(film->frames);
            fclose(fp);
            return false;
        }


// Read the frame data channel by channel
        for (unsigned char ch = 0; ch < film->header.channels; ch++) {//按通道逐步读取帧数据
            size_t channel_offset = ch * pixels_per_channel;
            if (fread(film->frames[i].data + channel_offset, 1, pixels_per_channel, fp) != pixels_per_channel) {
                // Cleanup on read error
                for (long j = 0; j <= i; j++) {
                    free(film->frames[j].data);
                }
                free(film->frames);
                fclose(fp);
                return false;

            }
        }
    }

    fclose(fp);
    return true;
}

bool write_film(const char* filename, const Film* film) {写文件的函数const数据类型不可改变 char*字符串由一堆字符组成的数组写到Film* film
    FILE* fp = fopen(filename, "wb");
    if (!fp) return false;

    // Write headerizeof(FilmHeader)
    if (fwrite(&film->header, sizeof(FilmHeader), 1, fp) != 1) {//从指针fp写到film header，每一个元素的大小，元素个数
        fclose(fp);
        return false;
    }

    // Write frames
    size_t pixels_per_channel = film->header.height * film->header.width;
    
    for (long i = 0; i < film->header.num_frames; i++) {
        // Write each channel's data
        for (unsigned char ch = 0; ch < film->header.channels; ch++) {
            size_t channel_offset = ch * pixels_per_channel;
            if (fwrite(film->frames[i].data + channel_offset, 1, pixels_per_channel, fp) != pixels_per_channel) {
                fclose(fp);
                return false;
            }
        }
    }

    fclose(fp);
    return true;
}

void free_film(Film* film) {
    if (!film) return;
    
    if (film->frames) {
        for (long i = 0; i < film->header.num_frames; i++) {
            free(film->frames[i].data);
        }
        free(film->frames);
    }
}


void reverse_film(Film* film) {
    if (!film || !film->frames ) {
        // 如果电影指针为空，帧数组为空，
        printf("\n")
        return false;
    }
    else if (film->header.num_frames <= 1) {
        return;
    }//帧数小于等于1，则无需反转

    for (long i = 0; i < film->header.num_frames / 2; i++) {
        // 交换帧数组中的第 i 帧和倒数第 i 帧
        FilmFrame temp = film->frames[i];
        film->frames[i] = film->frames[num_frames - i - 1];
        film->frames[num_frames - i - 1] = temp;
    }
}

void swap_channel(Film* film, unsigned char channel1, unsigned char channel2) {
    if (!film || !film->frames) {
        // 如果电影指针为空，帧数组为空
        return false;
    }
    else if  (film->header.channels <= 1) {//通道数不足两个，不需要交换
        return;
    else if  (channel1 >= film->header.channels || channel2 >= film->header.channels) {
        // 如果通道索引无效
        printf("\n");
        return false;
    }

    size_t pixels_per_channel = ilm->header.height * film->header.width;//每个通道的像素数量大小

    for (long i = 0; i < film->header.num_frames; i++) {
        unsigned char* frame_data = film->frames[i].data;

        for (size_t j = 0; j < pixels_per_channel; j++) {
            // 计算每个通道中像素的位置
            size_t p1 = channel1 * pixels_per_channel + j;
            size_t p2 = channel2 * pixels_per_channel + j;

            // 交换像素值
            unsigned char temp = frame_data[p1];
            frame_data[p1] = frame_data[p2];
            frame_data[p2] = temp;
        }
    }
}

void clip_channel(Film* film, unsigned char channel, unsigned char minimum, unsigned char maximum) {
    if (!film || !film->frames || film->header.channels <= channel) {
        // 如果电影指针为空，帧数组为空，或者通道编号无效
        printf(" \n");
        return false;
    }

    if (minimum > maximum) {
        // 如果最小值大于最大值，输入无效
        printf("\n");
        return false;
    }

    size_t pixels_per_channel = film->header.height * film->header.width;

    for (long i = 0; i < film->header.num_frames; i++) {
        unsigned char* frame_data = film->frames[i].data;

        for (size_t j = 0; j < pixels_per_channel; j++) {
            // 计算当前通道中像素的位置
            size_t p = channel * pixels_per_channel + j;

            // 限制像素值在 [minimum, maximum] 范围内
            if (frame_data[p] < minimum) {
                frame_data[p] = minimum;
            }
            else if (frame_data[p] > maximum) {
                frame_data[p] = maximum;
            }
        }
   }
}

void scale_channel(Film* film, unsigned char channel, float scale_factor) {
    if (!film || !film->frames || channel >= film->header.channels) {
        // 检查电影数据是否有效，以及通道编号是否超出范围
        printf("\n");
        return false;
    }

    if (scale_factor <= 0) {
        // 缩放因子必须为正值
        printf("\n");
        return false;
    }

    size_t pixels_per_channel = film->header.height * film->header.width;

    for (long i = 0; i < film->header.num_frames; i++) {
        unsigned char* frame_data = film->frames[i].data;

        for (size_t j = 0; j < pixels_per_channel; j++) {
            // 计算当前通道中像素的位置
            size_t p = channel * pixels_per_channel + j;

            // 乘以缩放因子，并限制在 [0, 255] 范围内
            int scaled_value = (int)(frame_data[p] * scale_factor);
            if (scaled_value > 255) {
                frame_data[p] = 255;
            } else if (scaled_value < 0) {
                frame_data[p] = 0;
            } else {
                frame_data[p] = (unsigned char)scaled_value;
            }
        }
    }
}
