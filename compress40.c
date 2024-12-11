/*
 *     compress40.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file can perform compressions or decompressions on PPM images
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
#include "pack.h"

/**********compress40********
 *
 * Calls upon helper functions to read a ppm and write a compressed image
 * Inputs: 
 *      - input file containing ppm
 *
 * Return: none
 * Expects:
 *      input file to contain ppm
 *
 * Notes:
 *      
 ************************/
extern void compress40 (FILE *input)
{
        assert(input != NULL);

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);
        
        /*trims the image in case of odd width/height*/
        A2Methods_UArray2 image = read_and_trim(input);
        assert(image != NULL);

        /**converts RGB to Component Video representation */
        A2Methods_UArray2 CvArray = RGB_to_CV(image, methods);
        assert(CvArray != NULL);

        int width = methods->width(image) / 2;
        int height = methods->height(image) / 2;

        A2Methods_UArray2 blockedArray = methods->new(width, height, 
                                                sizeof(struct allVals));
        assert(blockedArray != NULL);

        /*Performs chromatization and DCT on pixel values*/
        chromaDCT(CvArray, blockedArray, methods);
        
        
        A2Methods_UArray2 keywordArr = methods->new(width, height, 
                                                sizeof(struct cl_struct));
        assert(keywordArr != NULL);

        struct cl_struct closure;
        closure.array = keywordArr;
        closure.methods = methods;

        /* Packs image information into 32-bit word */
        methods->map_row_major(blockedArray, quantizeKeyword, &closure);

        printf("COMP40 Compressed image format 2\n%u %u", methods->width(image)
                                                , methods->height(image));

        printf("\n");
        
        methods->map_row_major(keywordArr, writeImage, NULL);

        methods->free(&image);
        methods->free(&CvArray);
        methods->free(&blockedArray);
        methods->free(&keywordArr);
} 

/**********decompress40********
 *
 * Calls upon helper functions to read a compressed image and write a ppm
 * Inputs: 
 *      - input file containing compressed image
 *
 * Return: none
 * Expects:
 *      input file to contain compressed image
 *
 * Notes:
 *      
 ************************/
extern void decompress40(FILE *input)
{
        assert(input != NULL);

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* Stores image word representations in an array */
        A2Methods_UArray2 keywordArray = readImage(input, methods);
        assert(keywordArray != NULL);
        
        int width = methods->width(keywordArray);
        int height = methods->height(keywordArray);

        A2Methods_UArray2 allValues = methods->new(width, height, 
                                                sizeof(struct allVals));
        assert(allValues != NULL);

        struct cl_struct newClosure;
        newClosure.array = allValues;
        newClosure.methods = methods;

        /* Unpacks words from 32-bit representation */
        methods->map_default(keywordArray, Unpack, &newClosure);
        

        A2Methods_UArray2 newCv = methods->new(width * 2, height * 2, 
                                                sizeof(struct cv));
                                                
        assert(newCv != NULL);
                
        /* Performs inverse chromatization and DCT */
        reverseChromaDCT(newClosure.array, newCv, methods);

        /* Converts pixels from Component Video to RGB representation */
        Pnm_ppm final = CV_to_RGB(newCv, methods);
        assert(final != NULL);
        
        Pnm_ppmwrite(stdout, final);

        methods->free(&keywordArray);
        methods->free(&allValues);
        methods->free(&newCv);
        Pnm_ppmfree(&final);
}