#ifndef FILMMASTER2000_H
#define FILMMASTER2000_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>

//头文件里存着一些常数数据结构和函数
// Constants
#define MAX_CHANNELS 3
#define MAX_HEIGHT 128
#define MAX_WIDTH 128

// Structure to hold film header information
//定义了一个自己的数据类型FilmHeader 每一帧都有四个头文件包括哪一帧 颜色 高和宽
typedef struct {
    long num_frames;
    unsigned char channels;//颜色通道
    unsigned char height;// 表示电影帧的高度（像素），范围 0-255
    unsigned char width;
} FilmHeader;

// Structure to represent a single frame 定义一帧的结构
typedef struct {
    unsigned char* data;  // Raw pixel data 星号代表指针或者数组的头，这一帧的数据
    size_t size;         // Size of data in bytes 数据大小有几个unsigned char
} FilmFrame;

// Structure to represent the entire film
typedef struct {//定义一个电影的结构
    FilmHeader header;//电影的头
    FilmFrame* frames;//一个电影有很多帧
} Film;

//所有的函数是不是都要在头文件里提一下
// Function prototypes
bool read_film(const char* filename, Film* film);
bool write_film(const char* filename, const Film* film);
void free_film(Film* film);
void reverse_film(Film* film);
void swap_channel(Film* film, unsigned char channel1, unsigned char channel2)
void clip_channel(Film* film, unsigned char channel, unsigned char minimum, unsigned char maximum)
void scale_channel(Film* film, unsigned char channel, float scale_factor)

#endif // FILMMASTER2000_H
