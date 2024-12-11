/*
 *     fileHandler.h
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the interface for reading and writing images/words 
 *     from files
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "arith40.h"
#include "bitpack.h"

/*struct for storing rgb values*/
struct rgb {
        float red;
        float green;
        float blue;
};

/*struct for storing cv values*/
struct cv {
        float y;
        float pb;
        float pr;
};

/*closure struct for storing UArray2 and methods*/
struct cl_struct {
        A2Methods_UArray2 array;
        A2Methods_T methods;
};

/*struct for storing all values that will be packed into a keyword*/
struct allVals {
        float a;
        float b;
        float c;
        float d;
        float Pb;
        float Pr;
};


/**********read_and_trim********
 *
 * Reads in an image raster and ensures that width and height are both even
 * Inputs: 
 *      -An input file containing the image raster.
 * Return: A UArray2 containing an image raster with even x even dimensions
 * Expects:
 *
 * Notes:
 ************************/
A2Methods_UArray2 read_and_trim(FILE *input);


/**********writeImage********
 *
 * apply function for writing all keywords in big endian
 * Inputs: 
 *      -col 
 *      -row 
 *      -current UArray2 of width/2 x height/2 
 *      -current value which is a uint32_t
 *      -closure 
 * Return: signed 4 bit int value
 * Expects:
 *      closure to be NULL
 *
 * Notes:
 *      
 ************************/
void writeImage(int col, int row, A2Methods_UArray2 keyArray, 
                                void* curr, void* cl);
                                

/**********readImage********
 *
 * Function which reads 32-bit words for image decompression
 * Inputs: 
 *      - input file containing image words.
 *      - A2methods_T methods
 *
 * Return: A2Methods_UArray2 object containing 32-bit words
 * Expects:
 *
 * Notes:
 *      
 ************************/
A2Methods_UArray2 readImage(FILE *input, A2Methods_T methods);