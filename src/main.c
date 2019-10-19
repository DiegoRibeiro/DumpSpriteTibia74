#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d_bitmap.h"
#include "sprite_dumper.h"

#define COL 90
#define ROW 90

int main(int argc, char** argv) {
    printf("STARTING APP ... \n");

    TIBIA_SPRITE_HEADER tsh;
    int read;
    BINARY_SPRITE_DATA bin;
    SPRITE_SHEET_DATA data;
    D_PIXEL** spr_row;
    FILE* fp;
    FILE* fps;

    // allocate temporary list of buffer row
    spr_row = (D_PIXEL**)malloc(sizeof(D_PIXEL*) * COL);
    for(int i = 0; i < COL; i++) {
        spr_row[i] = (D_PIXEL*)malloc(sizeof(D_PIXEL) * 1024);
    }

    open_sprite(&fp, "Tibia");

    if(fp == NULL) {
        printf("Error opening file ...\n");
        return 1;
    }

    read_header(fp, &tsh);
    printf("Sprite version: %u\n", tsh.version);
	printf("Sprite count: %u\n", tsh.count);

    create_spritesheet(&fps, &data, COL, ROW, 32, 0);

    for(int j = 0; j < ROW; j++) {
        for(int i = 0; i < COL; i++) {
            bin.id = 0 + (j*COL) + i;
            read = read_sprite(fp, &bin, &data);
            expand_sprite(spr_row[i], &bin, &data);
        }

        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < COL; j++) {
                write_sprite(fps, spr_row[j], i * 32, i * 32 + 32);
            }
        }
    }

    fclose(fps);
    
    // clean up
    for(int i = 0; i < COL; i++) {
        free(spr_row[i]);
    }
    free(spr_row);

    fclose(fp);
    return 0;
}