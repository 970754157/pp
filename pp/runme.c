#include "FilmMaster2000.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    printf("Usage: film_tool <command> <input_file> [options]\n");
    printf("Commands:\n");
    printf("  info <input_file>                   - Display film information\n");
    printf("  reverse <input_file> <output_file>  - Reverse film frames\n");
    printf("  swap <input_file> <output_file> <channel1> <channel2>\n");
    printf("                                      - Swap two channels in frames\n");
    printf("  clip <input_file> <output_file> <channel> <min> <max>\n");
    printf("                                      - Clip pixel values in a channel\n");
    printf("  scale <input_file> <output_file> <channel> <factor>\n");
    printf("                                      - Scale pixel values in a channel\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    const char* command = argv[1];
    const char* input_file = argv[2];

    Film film = {0};

    if (!read_film(input_file, &film)) {
        printf("Error: Failed to read film file '%s'.\n", input_file);
        return 1;
    }

    if (strcmp(command, "info") == 0) {
        printf("Film Information:\n");
        printf("  Number of Frames: %ld\n", film.header.num_frames);
        printf("  Channels: %u\n", film.header.channels);
        printf("  Height: %u\n", film.header.height);
        printf("  Width: %u\n", film.header.width);
    } else if (strcmp(command, "reverse") == 0) {
        if (argc < 4) {
            printf("Error: reverse command requires output file parameter.\n");
            print_usage();
            free_film(&film);
            return 1;
        }
        const char* output_file = argv[3];
        reverse_film(&film);
        if (!write_film(output_file, &film)) {
            printf("Error: Failed to write reversed film to '%s'.\n", output_file);
            free_film(&film);
            return 1;
        }
        printf("Successfully reversed film and saved to '%s'.\n", output_file);
    } else if (strcmp(command, "swap") == 0) {
        if (argc < 6) {
            printf("Error: swap command requires output file and two channels.\n");
            print_usage();
            free_film(&film);
            return 1;
        }
        const char* output_file = argv[3];
        unsigned char channel1 = (unsigned char)atoi(argv[4]);
        unsigned char channel2 = (unsigned char)atoi(argv[5]);
        swap_channel(&film, channel1, channel2);
        if (!write_film(output_file, &film)) {
            printf("Error: Failed to write swapped film to '%s'.\n", output_file);
            free_film(&film);
            return 1;
        }
        printf("Successfully swapped channels %u and %u and saved to '%s'.\n", channel1, channel2, output_file);
    } else if (strcmp(command, "clip") == 0) {
        if (argc < 7) {
            printf("Error: clip command requires output file, channel, min, and max.\n");
            print_usage();
            free_film(&film);
            return 1;
        }
        const char* output_file = argv[3];
        unsigned char channel = (unsigned char)atoi(argv[4]);
        unsigned char minimum = (unsigned char)atoi(argv[5]);
        unsigned char maximum = (unsigned char)atoi(argv[6]);
        clip_channel(&film, channel, minimum, maximum);
        if (!write_film(output_file, &film)) {
            printf("Error: Failed to write clipped film to '%s'.\n", output_file);
            free_film(&film);
            return 1;
        }
        printf("Successfully clipped channel %u to range [%u, %u] and saved to '%s'.\n", channel, minimum, maximum, output_file);
    } else if (strcmp(command, "scale") == 0) {
        if (argc < 6) {
            printf("Error: scale command requires output file, channel, and factor.\n");
            print_usage();
            free_film(&film);
            return 1;
        }
        const char* output_file = argv[3];
        unsigned char channel = (unsigned char)atoi(argv[4]);
        float scale_factor = atof(argv[5]);
        scale_channel(&film, channel, scale_factor);
        if (!write_film(output_file, &film)) {
            printf("Error: Failed to write scaled film to '%s'.\n", output_file);
            free_film(&film);
            return 1;
        }
        printf("Successfully scaled channel %u by factor %.2f and saved to '%s'.\n", channel, scale_factor, output_file);
    } else {
        printf("Error: Unknown command '%s'.\n", command);
        print_usage();
        free_film(&film);
        return 1;
    }

    free_film(&film);
    return 0;
}
