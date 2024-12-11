/*
 *     chromaDCT.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the interface for doing chroma and DCT transformations
 *     on images
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
#include "fileHandler.h"

/**********chromaDCT********
 *
 * Goes through an array of dimensions width/height and for each 2x2 block
 *      of elements obtains average pb, pr and a, b, c, d values and stores
 *      in width/2 x height/2 array
 * Inputs: 
 *      -array in cv form 
 *      -smaller array to store new data in 
 *      -methods
 * Return: width/2 x height/2 UArray2
 * Expects:
 *     nonnull cvArray and blocked array
 *
 * Notes:
 *      uses quadruple for loop to go through blocks in row major order
 ************************/
void chromaDCT(A2Methods_UArray2 cvArray, 
                        A2Methods_UArray2 blocked, A2Methods_T methods);


/**********reverseChromaDCT********
 *
 * Takes a blocked array of width/2 x height/2 and updates the cvArray which is
 *      of dimensions width x height
 * Inputs: 
 *      -initial blocked array
 *      -new normal sized array
 *      -methods
 * Return: none
 * Expects:
 *     blocked array dimensions w/2 x h/2, cvArray dimensions w x h
 *
 * Notes:
 *      updates cvArray
 ************************/


void reverseChromaDCT(A2Methods_UArray2 blockedArray, 
                                A2Methods_UArray2 cvArray, A2Methods_T methods);

/**********reverseChromaDCTmap********
 *
 * apply function for reversing chroma and DCT values
 * Inputs: 
 *      -col 
 *      -row 
 *      -current UArray2 of width/2 x height/2 
 *      -current value which is a allVals struct
 *      -closure which is a UArray2 of w x h
 * Return: signed 4 bit int value
 * Expects:
 *      curr to be between -0.3, 0.3
 *
 * Notes:
 *      if curr is not between -0.3 and 0.3, returns -15 or 15, respectfully
 ************************/
void reverseChromaDCTmap(int col, int row, A2Methods_UArray2 blockedArray, 
                                void* curr, void* cl);




