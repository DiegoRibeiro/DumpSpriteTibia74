#include "sprite_dumper.h"
#include <string.h>

FILE* open_sprite(char* name) {
    unsigned char buffer[50];
    sprintf(buffer, "%s.spr", name);
    printf("opened %s\n", buffer);
    return fopen(buffer, "rb");
}

FILE* open_spritesheet(char* name, int i) {
    char buffer[50];
    sprintf(buffer, "%s%d.bmp", name, i);
    printf("opened %s\n", buffer);
    return fopen(buffer, "wb");
}

void read_header(FILE* fp, TIBIA_SPRITE_HEADER* tsh) {
    fread(tsh, sizeof(TIBIA_SPRITE_HEADER), 1, fp);
}

int read_sprite(FILE* fp, int id, unsigned char* buffer, D_PIXEL* tpix) {
    unsigned int addr_offset = 6 + (4 * id) ;
    unsigned int offset; 

    fseek(fp, addr_offset, SEEK_SET);
    fread(&offset, sizeof(unsigned int), 1, fp);

    printf("offset: %x\n", offset);

    if (offset == 0) { // reserved, its empty and dont point to anything
        return 0;
    }

    fseek(fp, offset, SEEK_SET);

    printf("sizeof (D_PIXEL): %u\n", sizeof(D_PIXEL));

	fread(tpix, sizeof(D_PIXEL), 1, fp);

    printf("transparent pixel: (%x, %x, %x)\n", tpix->r, tpix->g, tpix->b);

    unsigned short bytes_to_proccess;
	fread(&bytes_to_proccess, sizeof(unsigned short), 1, fp);
	printf("processing %u bytes ...\n", bytes_to_proccess);

    int read = fread(buffer, sizeof(unsigned char), bytes_to_proccess, fp);

    printf("readed %u\n", read);

    if(read != bytes_to_proccess)
        return 0;
    return bytes_to_proccess;
}

void write_sprite(FILE* fp, unsigned char* buffer, D_PIXEL tpix, int read) {
    d_write_bmp_header(fp, 54 + (32 * 32 * 3));
    d_write_dib_header(fp, 32, -32, (32 * 32 * 3));

    int i = 0;
    unsigned short count_t; // count transparent pixel
    unsigned short count_c; // count colored pixel
    D_PIXEL cpix;

    printf("transparent pixel: (%x, %x, %x)\n", tpix.r, tpix.g, tpix.b);
  
    while(read > i) {
        printf("i: %u\n", i);
        // little endian
        count_t = (unsigned short)(buffer[i + 1] << 8 | buffer[i]);
        count_c = (unsigned short)(buffer[i + 3] << 8 | buffer[i + 2]);

        printf("count tranparent: %u, count colored: %u\n", count_t, count_c);

        i += 4; // skip 4 bytes

        // write transparent pixel
        for(int j = 0; j < count_t; j++) {
            d_write_pixel(fp, tpix);
        }

        // write colored pixel
        for(int j = 0; j < count_c; j++) {
            cpix.r = buffer[i];
            cpix.g = buffer[i + 1];
            cpix.b = buffer[i + 2];
            i += 3;
            d_write_pixel(fp, cpix);
        }
    }
    printf("i: %u\n", i);

    // fill with the left over of image with transparent pixel
    while(i < (1024 * 3)) {
        d_write_pixel(fp, tpix);
        i += 3;
    }
}