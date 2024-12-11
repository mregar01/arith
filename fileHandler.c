/*
 *     fileHandler.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the implementation for reading and writing 
 *     images/words from files
 *
 */

#include "fileHandler.h"




A2Methods_UArray2 read_and_trim(FILE *input)
{
        assert(input != NULL);
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);
        Pnm_ppm pixmap = Pnm_ppmread(input, methods);
        assert(pixmap != NULL);

        int trimmedheight = pixmap->height;
        int trimmedwidth = pixmap->width;
       
        if (pixmap->width % 2 != 0) {
                trimmedwidth--;
        }
        if (pixmap->height % 2 != 0) {
                trimmedheight--;
        }

        A2Methods_UArray2 newImage = methods->new(trimmedwidth, 
                                trimmedheight, sizeof(struct rgb));
        assert(newImage != NULL);
        
        /* loop through old array and copy onto new array for trimmed dim.*/
        for (int i = 0; i < trimmedwidth; i++) {
                for (int j = 0; j < trimmedheight; j++) {
                        Pnm_rgb currPixel = methods->at(pixmap->pixels, i, j);                                
                        struct rgb newpixel;
                        newpixel.red = ((float)(currPixel->red) / 
                                                (float)(pixmap->denominator));
                        newpixel.green = ((float) (currPixel->green) / 
                                                (float)(pixmap->denominator));
                        newpixel.blue = ((float) (currPixel->blue) / 
                                                (float)(pixmap->denominator));
                        
                        *((struct rgb *)methods->at(newImage, i, j)) = newpixel;
                }
        }
        Pnm_ppmfree(&pixmap);
        return newImage;    
}


void writeImage(int col, int row, A2Methods_UArray2 keyArray, 
                                void* curr, void* cl)
{
        (void) col;
        (void) row;
        (void) keyArray;
        (void) cl;
        
        uint32_t *word = (uint32_t *) curr;

        uint64_t final =  (uint64_t ) *word;
        
        /* Prints words in Big-Endian format */
        for (int i = 3; i >= 0; i--){
                putchar(Bitpack_getu(final, 8, i * 8));   
        }
}

A2Methods_UArray2 readImage(FILE *input, A2Methods_T methods)
{
        unsigned height, width;

        /* Reads image header */
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                                                        &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');
        
        A2Methods_UArray2 toReturn = methods->new(width / 2, height / 2, 
                                                        sizeof(uint32_t));
        
        uint64_t curr = 0;
        uint32_t keyword = 0;

        /* Reads each compressed word and stores it in toReturn array */
        for (unsigned row = 0; row < height / 2; row++){
                for (unsigned col = 0; col < width / 2; col++){
                        c = getc(input);
                        curr = Bitpack_newu(curr, 8, 24, c);
                        c = getc(input);
                        curr = Bitpack_newu(curr, 8, 16, c);
                        c = getc(input);
                        curr = Bitpack_newu(curr, 8, 8, c);
                        c = getc(input);
                        curr = Bitpack_newu(curr, 8, 0, c);
                        
                        keyword = (uint32_t) curr;
                        *((uint32_t *) methods->at(toReturn, col, 
                                                        row)) = keyword;
                }
        }
        return toReturn;       
}