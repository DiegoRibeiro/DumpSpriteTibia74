#include "sprite_dumper.h"
#include <string.h>
#include <stdint.h>

void open_sprite(FILE** fp, char* name) {
    unsigned char buffer[50];
    sprintf(buffer, "%s.spr", name);
    printf("opened %s\n", buffer);
    *fp = fopen(buffer, "rb");
}

void open_spritesheet(FILE** fp, char* name, int i) {
    char buffer[50];
    sprintf(buffer, "%s%d.bmp", name, i);
    printf("opened %s\n", buffer);
    *fp = fopen(buffer, "wb");
}

void read_header(FILE* fp, TIBIA_SPRITE_HEADER* tsh) {
    fread(tsh, sizeof(TIBIA_SPRITE_HEADER), 1, fp);
}

void create_spritesheet(FILE** fp, SPRITE_SHEET_DATA* data, int w, int h, int size, int index) {
    open_spritesheet(fp, "sprite", index);

    // sprite sheet 512x512
    data->w = w;
    data->h = h;

    int sizeHeader = HEADER_V4_SIZE + ( (data->w * size * PIXEL_V4_SIZE) + (data->h * size * PIXEL_V4_SIZE) );
    int width = data->w * size;
    int height = -data->h * size;
    int rawSizeHeader = (data->w * size * PIXEL_V4_SIZE) + (data->h * size * PIXEL_V4_SIZE);

    d_write_bmp_header_v4(*fp, sizeHeader);
    d_write_dib_header_v4(*fp, width, height, rawSizeHeader);
}

int read_sprite(FILE* fp, BINARY_SPRITE_DATA* bin, SPRITE_SHEET_DATA* data) {
    unsigned int addr_offset = 6 + (4 * bin->id) ;
    unsigned int offset; 

    fseek(fp, addr_offset, SEEK_SET);
    fread(&offset, sizeof(unsigned int), 1, fp);

    //printf("offset: %x\n", offset);

    if (offset == 0) { // reserved, its empty and dont point to anything
        memset(bin->buffer, 0, 10000);
        return 0;
    }

    fseek(fp, offset, SEEK_SET);

	fread(&data->tpix, sizeof(D_PIXEL), 1, fp);

    //printf("transparent pixel: (%x, %x, %x)\n", data->tpix.r, data->tpix.g, data->tpix.b);

    // choosing tranparent color
    //data->tpix.r = 248;
    //data->tpix.g = 248;
    //data->tpix.b = 240;

    uint16_t bytes_to_proccess;
	fread(&bytes_to_proccess, sizeof(uint16_t), 1, fp);
	//printf("processing %u bytes ...\n", bytes_to_proccess);

    int read = fread(bin->buffer, sizeof(uint8_t), bytes_to_proccess, fp);
    bin->read = read;

    //printf("readed %u\n", read);

    if(read != bytes_to_proccess)
        return 0;
    return bytes_to_proccess;
}

void write_full_sprite(FILE* fp, D_PIXEL* buffer) {
    for(int i = 0; i < 1024; i++) {
        d_write_pixel(fp, buffer[i]);
    }
}

void write_sprite(FILE* fp, D_PIXEL_A* buffer, int start, int end) {
    for(int i = start; i < end; i++) {
        d_write_pixel_v4(fp, buffer[i]);
    }
}

void expand_sprite(D_PIXEL_A* buffer, BINARY_SPRITE_DATA* bin, SPRITE_SHEET_DATA* data) {
    int i = 0; // tracker binary file
    uint16_t count_t; // count transparent pixel
    uint16_t count_c; // count colored pixel
    D_PIXEL cpix;
    int index = 0; // tracker bitmap file in pixels

    while(bin->read > i) {
        count_t = (uint16_t)(bin->buffer[i + 1] << 8 | bin->buffer[i]);
        count_c = (uint16_t)(bin->buffer[i + 3] << 8 | bin->buffer[i + 2]);

        i += 4; // skip 4 bytes
        
        // write transparent pixel
        for(int j = 0; j < count_t; j++) {
            buffer[index].r = data->tpix.r;
            buffer[index].g = data->tpix.g;
            buffer[index].b = data->tpix.b;
            buffer[index].a = 0x00;
            index++;
        }

        // write colored pixel
        for(int j = 0; j < count_c; j++) {
            buffer[index].r = bin->buffer[i];
            buffer[index].g = bin->buffer[i + 1];
            buffer[index].b = bin->buffer[i + 2];
            buffer[index].a = 0xFF;

            index++;
            i += 3;            
        }
    }

    // fill with the left over of image with transparent pixel
    while(index < 1024) {
        buffer[index].r = data->tpix.r;
        buffer[index].g = data->tpix.g;
        buffer[index].b = data->tpix.b;
        buffer[index].a = 0x00;
        index++;
    }
}