#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d_bitmap.h"
#include "sprite_dumper.h"

#define COL 16
#define ROW 16

int main(int argc, char** argv) {
    printf("STARTING APP ... \n");

    TIBIA_SPRITE_HEADER tsh;
    int read;
    BINARY_SPRITE_DATA bin;
    SPRITE_SHEET_DATA data;
    D_PIXEL_A** spr_row;
    FILE* fp;
    FILE* fps = NULL;

    // allocate temporary list of buffer row
    spr_row = (D_PIXEL_A**)malloc(sizeof(D_PIXEL_A*) * COL);
    for(int i = 0; i < COL; i++) {
        spr_row[i] = (D_PIXEL_A*)malloc(sizeof(D_PIXEL_A) * 1024);
    }

    open_sprite(&fp, "Tibia");

    if(fp == NULL) {
        printf("Error opening file ...\n");
        return 1;
    }

    read_header(fp, &tsh);
    printf("Sprite version: %u\n", tsh.version);
	printf("Sprite count: %u\n", tsh.count);

    int current_sprite = 0;
    int sprites_left = tsh.count;
    int sprites_wrote = 0;

    // for all sprites do
    while(sprites_left > 0) {
        // create a new spritesheet if it is full
        if(current_sprite % (COL * ROW) == 0) {
            if(fps) {
                fclose(fps);
            }

            sprites_wrote = 0;
            create_spritesheet(&fps, &data, COL, ROW, 32, (int)(current_sprite/(COL * ROW)));
        }

        // checking if it is the last sprites
        int total = sprites_left < COL ? sprites_left : COL;

        // read COL sprites
        for(int i = 0; i < total; i++) {
            bin.id = current_sprite;
            read = read_sprite(fp, &bin, &data);
            expand_sprite(spr_row[i], &bin, &data);
            current_sprite++;
        }

        // write COL sprites
        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < COL; j++) {
                write_sprite(fps, spr_row[j], i * 32, i * 32 + 32);
            }
        }
        sprites_wrote += COL;

        // decrease sprites left
        sprites_left -= COL;
    }

    // write the last sprites that did not match COL * ROW
    for(int i = 0; i < (COL*ROW) - sprites_wrote; i++) {
        write_sprite(fps, spr_row[0], 0, 1024);
    }

    if(fps) {
        fclose(fps);
    }
    
    // clean up
    for(int i = 0; i < COL; i++) {
        free(spr_row[i]);
    }
    free(spr_row);

    fclose(fp);
    return 0;
}