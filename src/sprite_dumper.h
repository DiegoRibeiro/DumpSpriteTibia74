#ifndef _SPRITE_DUMPER_H
    #define _SPRITE_DUMPER_H

    #include <stdio.h>
    #include "d_bitmap.h"
    #include "d_bitmap_v4.h"

    #pragma pack(1)
    struct tibiaspr {
        unsigned int version;
        unsigned short count;
    };
    #pragma pack()

    struct spritesheetdata {
        D_PIXEL tpix;
        int w;
        int h;
    };

    struct binaryspritedata {
        int id;
        unsigned char buffer[10000];
        int read;
    };

    typedef struct tibiaspr TIBIA_SPRITE_HEADER;
    typedef struct spritesheetdata SPRITE_SHEET_DATA;
    typedef struct binaryspritedata BINARY_SPRITE_DATA;

    void open_sprite(FILE** fp, char* name);
    void open_spritesheet(FILE** fp, char* name, int i);
    void create_spritesheet(FILE** fp, SPRITE_SHEET_DATA* data, int w, int h, int size, int index);
    void read_header(FILE* fp, TIBIA_SPRITE_HEADER* tsh);
    int read_sprite(FILE* fp, BINARY_SPRITE_DATA* bin, SPRITE_SHEET_DATA* data);
    void write_full_sprite(FILE* fp, D_PIXEL* buffer);
    void write_sprite(FILE* fp, D_PIXEL_A* buffer, int start, int end);
    void expand_sprite(D_PIXEL_A* buffer, BINARY_SPRITE_DATA* bin, SPRITE_SHEET_DATA* data);
#endif