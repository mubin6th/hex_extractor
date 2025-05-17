#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "libbmp.h"

const char *const VERSION = "v0.0.1";
const char *const HELP =
    "usage:\n"
    "    ./hex_extractor [options]\n\n"
    ""
    "options:\n"
    "    -h            print help message and exit.\n"
    "    -v            print version message and exit.\n"
    "    -tw           provide tile width.\n"
    "    -th           provide tile height.\n"
    "    -i            provide input file. (must)\n"
    "    -o            provide output file name. Default:\n"
    "                      <original_file_name_without_extension>.bmp"
;

typedef struct _ImgConfig {
    uint32_t tile_x, tile_y, col_count;
} ImgConfig;

typedef struct _InputArgs {
    ImgConfig config;
    const char *input_path;
} InputArgs;

bool isStringRgb(const char *s, char delim);
uint8_t numFromHexChar(char c);
uint32_t hexFromRgbString(const char *s);
void createAndDrawPalette(bmp_img *img, uint32_t *colors,
                          size_t colors_len, ImgConfig config);
void getOutputPath(char *out, const char *input_path,
                  size_t input_path_len);
void getArgs(InputArgs *out_args, char **argv);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
                "err: expected the"
                "argument [input file].");
        return 1;
    }


    FILE *input_file_ptr = fopen(argv[1], "r");
    char line_buf[1024];
    char *token;

    uint32_t color_arr[1024];
    size_t color_arr_len = 0;

    while (fgets(line_buf, sizeof(line_buf),
                 input_file_ptr) != NULL)
    {
        token = strtok(line_buf, " ");

        while (token != NULL) {
            if (!isStringRgb(token, ' ')) {
                token = strtok(NULL, " ");
                continue;
            }

            color_arr[color_arr_len] = hexFromRgbString(token);
            color_arr_len++;

            token = strtok(NULL, " ");
        }
    }

    ImgConfig config = {
        .tile_x = 64,
        .tile_y = 64,
        .col_count = color_arr_len,
    };

    bmp_img palette_img;
    char output_path[256];

    createAndDrawPalette(&palette_img, color_arr,
                         color_arr_len, config);

    getOutputPath(output_path, argv[1], strlen(argv[1]));
    bmp_img_write(&palette_img, output_path);

    bmp_img_free(&palette_img);
    fclose(input_file_ptr);
    return 0;
}

bool isStringRgb(const char *s, char delim) {
    if (s == NULL || s[0] != '#') {
        return false;
    }
    s++;

    size_t counter = 0;

    while (*s == EOF) {
        if (*s == '\n' || *s == ' ') {
            break;
        }

        if (counter > 6) {
            return false;
        }

        if ((*s < '0' || *s > '9') &&
            (*s < 'A' || *s > 'F') &&
            (*s < 'a' || *s > 'f'))
        {
            return false;
        }

        s++;
        counter++;
    }

    return true;
}

uint8_t numFromHexChar(char c) {
    // no break statement required
    // as the funtion instantly returns.
    switch (c) {
        case '0'...'9':
            return c - '0';
        case 'A'...'F':
            return c - 'A' + 10;
        case 'a'...'f':
            return c - 'a' + 10;
        default:
            return 16;
    }
}

uint32_t hexFromRgbString(const char *s) {
    uint32_t r = numFromHexChar(s[1]) << 4 |
                 numFromHexChar(s[2]);
    uint32_t g = numFromHexChar(s[3]) << 4 |
                 numFromHexChar(s[4]);
    uint32_t b = numFromHexChar(s[5]) << 4 |
                 numFromHexChar(s[6]);

    return r << 24 | g << 16 | b << 8 | 0x00;
}

void createAndDrawPalette(bmp_img *img, uint32_t *colors,
                          size_t colors_len, ImgConfig config)
{
    int img_x = config.tile_x * config.col_count;
    int img_y = (int)ceil((float)colors_len / config.col_count) *
                config.tile_y;

    bmp_img_init_df(img, img_x, img_y);

    uint32_t target_color;
    size_t target_index;

    for (int y = 0; y < img_y; y++) {
        for (int x = 0; x < img_x; x++){
            target_index = (int)(x / config.tile_x) +
                           (int)(y / config.tile_y) * config.col_count;

            target_color = target_index < colors_len ?
                           colors[target_index] : 0x00;

            bmp_pixel_init(&img->img_pixels[y][x],
                           (uint8_t)(target_color >> 24),
                           (uint8_t)(target_color >> 16 & 0xff),
                           (uint8_t)(target_color >>  8 & 0xff));
        }
    }
}

void getOutputPath(char *out, const char *input_path,
                  size_t input_path_len)
{
    size_t k = input_path_len - 1;

    while (k != 0) {
        if (input_path[k] == '.') {
            break;
        }
        k--;
    }

    for (size_t i = 0; i <= k; i++) {
        *out = input_path[i];
        out++;
    }

    const char *ext = "bmp";

    while (true) {
        *out = *ext;
        if (*ext == '\0') {
            break;
        }

        ext++;
        out++;
    }
}

//void getArgs(InputArgs *out_args, char **argv) {
//    
//}
