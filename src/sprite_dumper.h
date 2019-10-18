#ifndef _SPRITE_DUMPER_H
    #define _SPRITE_DUMPER_H

    #include <stdio.h>
    #include "d_bitmap.h"

    #pragma pack(1)
    struct tibiaspr {
        unsigned int version;
        unsigned short count;
    };
    #pragma pack()

    typedef struct tibiaspr TIBIA_SPRITE_HEADER;

    FILE* open_sprite(char* name);
    FILE* open_spritesheet(char* name, int i);
    void read_header(FILE* fp, TIBIA_SPRITE_HEADER* tsh);
    int read_sprite(FILE* fp, int id, unsigned char* buffer, D_PIXEL* tpix);
    void write_sprite(FILE* fp, unsigned char* buffer, D_PIXEL tpix, int read);

#endif