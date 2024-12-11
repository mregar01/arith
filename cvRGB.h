/*
 *     cvRGB.h
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the interface for converting between RGB and Component
 *     Video pixel formats.
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
#include "chromaDCT.h"



/**********RGB_to_CV********
 *
 * Converts pixels of an image from RGB to Component Video format.
 * Inputs: 
           - A UArray2 containing the image raster
           - An A2Methods object containing the desired array methods

 * Return: A UArray2 containing the updated pixels
 * Expects:
 *      The inputted array to contain RGB structs
 *
 * Notes:
 ************************/
A2Methods_UArray2 RGB_to_CV(A2Methods_UArray2 image, A2Methods_T methods);


/**********RgbtoCvApply********
 *
 * Apply Function which converts pixels of an image from RGB to Component Video
 * format.
 * 
 * Inputs: 
           - an int representing a column index in the array being mapped
           - an int representing a row index in the array being mapped
           - a UArray2 which is the array being traversed for this function
           - a void pointer to the current element
           - a void pointer to the function closure

 * Return: N/A
 * Expects:
 *      currArray contains RGB structs
 *      closure is a cl_struct struct
 * Notes:
 ************************/
void RgbtoCvApply(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure); 


/**********CV_to_RGB********
 *
 * Takes in a array with cv structs and converts to a PPM
 * Inputs: 
 *      -a UArray2 of the current image
 *      - methods
 * Return: A PPM
 * Expects:
 *      the UArray2 to be nonnull and contain CV structs
 *
 * Notes:
 *      
 ************************/
Pnm_ppm CV_to_RGB(A2Methods_UArray2 image, A2Methods_T methods);


/**********CvtoRgbApply********
 *
 * apply function that converts every pixel of a cv array to an rgb struct in 
 *      a ppm
 * Inputs: 
 *      -col 
 *      -row 
 *      -current UArray2 of width x height 
 *      -current value which is a cv struct
 *      -closure which is a struct with the new array and methods
 * Return: none
 * Expects:
 *      currArray to have cv structs
 *
 * Notes:
 *      uses conversions for each pixel to obtain r, g, b
 ************************/
void CvtoRgbApply(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure);