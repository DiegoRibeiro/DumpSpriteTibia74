#include <stdio.h>
#include <stdlib.h>
#include "d_bitmap.h"

#pragma pack(1)
typedef struct tibiaspr {
	unsigned int version;
	unsigned short count;
} TIBIA_SPRITE_HEADER;
#pragma pack()

int main(int argc, char** argv) {
	FILE *fp = fopen("Tibia.spr", "rb");
	FILE *fp_log = fopen("log", "w");

    TIBIA_SPRITE_HEADER tsh;
    unsigned int* offsets = NULL;
	unsigned int spr_addr_offset;
    PIXEL p;
    int i;
    char buffer[50];

    // reading sprite header
    fread(&tsh, sizeof(TIBIA_SPRITE_HEADER), 1, fp);
	printf("Sprite version: %u\n", tsh.version);
	printf("Sprite count: %u\n", tsh.count);

    for(int i = 0; i < tsh.count; i++) {
        unsigned int addr_offset = 6 + (4*i);
        fseek(fp, addr_offset, SEEK_SET);
        unsigned int offset;
		fread(&offset, sizeof(unsigned int), 1, fp);

        if (offset == 0) { // reserved, its empty and dont point to anything
			continue;
		}

        printf("Processing sprite number %u ...\n", i);
		sprintf(buffer, "sprite%u.bmp", i);
		printf("Create file %s\n", buffer);

        FILE * fpi = fopen (buffer, "wb");
		printf("offset %x\n", offset);

        fseek(fp, offset, SEEK_SET);
        PIXEL tpix;
		fread(&tpix, sizeof(PIXEL), 1, fp);
        printf("Found transparent pixel with value: (%u, %u, %u)\n", tpix.r, tpix.g, tpix.b);
		unsigned short bytes_to_proccess;
		fread(&bytes_to_proccess, sizeof(unsigned short), 1, fp);
		printf("processing %u bytes ...\n", bytes_to_proccess);

        if (bytes_to_proccess == 0) {
            d_write_bmp_header(fpi, 0x0C36);
            d_write_dib_header(fpi, 0x20, 0xFFFFFFE0, 0x0C00);
            // empty sprite
            for(int j = 0; j < 1024; j++) {
                d_write_pixel(fpi, tpix);
            }
        }
        else {
            d_write_bmp_header(fpi, 0x0C36);
            d_write_dib_header(fpi, 0x20, 0xFFFFFFE0, 0x0C00);

            int byte_con = bytes_to_proccess;
			int idx = 0;
			while (byte_con > 0) {
                printf("bytes left = %d\n", byte_con);
				unsigned short c_tpix = 0;	// count transparent pixels
				unsigned short c_cpix = 0;	// count colored pixels

                fread(&c_tpix, sizeof(unsigned short), 1, fp);
				printf("Found %d  transparent pixel\n", c_tpix);
				fread(&c_cpix, sizeof(unsigned short), 1, fp);
				printf("Found %d colored pixel\n", c_cpix);

                byte_con -= sizeof(unsigned short);
				byte_con -= (c_cpix);

                for(int j = 0; j < c_tpix; j++) {
                    d_write_pixel(fpi, tpix);
                }

                PIXEL aux;
                for(int j = 0; j < c_cpix; j++) {
                    fread(&aux.r, sizeof(unsigned char), 1, fp);
					fread(&aux.g, sizeof(unsigned char), 1, fp);
					fread(&aux.b, sizeof(unsigned char), 1, fp);

                     d_write_pixel(fpi, aux);
                }
            }
        }
        fclose(fpi);
    }
    printf("Finished ... \n");

    fclose(fp);
	fclose(fp_log);

    return 0;
}