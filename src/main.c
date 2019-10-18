#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d_bitmap.h"
#include "sprite_dumper.h"

int main(int argc, char** argv) {
    printf("STARTING APP ... \n");

    TIBIA_SPRITE_HEADER tsh;
    unsigned char buffer[10000];
    int read;
    D_PIXEL tpix;
    D_PIXEL cpix;

    FILE* fp = open_sprite("Tibia");

    if(fp == NULL) {
        printf("Error opening file ...\n");
        return 1;
    }

    read_header(fp, &tsh);
    printf("Sprite version: %u\n", tsh.version);
	printf("Sprite count: %u\n", tsh.count);

    for(int i = 0; i < tsh.count; i++) {
        FILE* fps = open_spritesheet("sprite", i);
        read = read_sprite(fp, i, buffer, &tpix);
        printf("read %d bytes ...\n", read);
        write_sprite(fps, buffer, tpix, read);
        fclose(fps);
    }

    fclose(fp);
    return 0;
}