/*
 *     pack.h
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the interface for packing and unpacking image words
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
#include "cvRGB.h"


/*********quantizeKeyword********
 *
 * apply function that quantizes all 6 image values (pb, pr, a, b, c, d) and
 *      packs them into a 32 bit unsigned int
 * Inputs: 
 *      -col 
 *      -row 
 *      -current UArray2 of width/2 x height/2 
 *      -current value which is an allVals struct
 *      -closure which is a UArray2 of uint32_t's
 * Return: nothing
 * Expects:
 *      nonnull closure and curr
 *
 * Notes:
 *      
 ************************/
void quantizeKeyword(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure);


/**********convertBCD********
 *
 * takes in b, c, d values from an image and converts them to a signed 4 bit
 * integer
 * Inputs: 
 *      -a float of the unconverted b, c, d values
 * Return: signed 4 bit int value
 * Expects:
 *      curr to be between -0.3, 0.3
 *
 * Notes:
 *      if curr is not between -0.3 and 0.3, returns -15 or 15, respectfully
 ************************/
int64_t convertBCD(float curr);


/**********Unpack********
 *
 * apply function for unpacking keywords
 * Inputs: 
 *      -col 
 *      -row 
 *      -current UArray2 of width/2 x height/2 
 *      -current value which is a uint32_t
 *      -closure which contains methods and UArray2 of allVals struct
 * Return: signed 4 bit int value
 * Expects:
 *      closure to have methods and allVals struct
 *
 * Notes:
 *      
 ************************/
void Unpack(int col, int row, A2Methods_UArray2 blockedArray, 
                                void* curr, void* cl);
