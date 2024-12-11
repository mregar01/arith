/*
 *     cvRGB.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the implementation for converting between RGB and 
 *     Component Video pixel formats.
 *
 */

#include "cvRGB.h"

A2Methods_UArray2 RGB_to_CV(A2Methods_UArray2 image, A2Methods_T methods)
{
        assert(image != NULL);
        assert(methods != NULL);
        
        A2Methods_UArray2 cvImage = methods->new(methods->width(image), 
                                methods->height(image), sizeof(struct cv)); 
                                        
        assert(cvImage != NULL);

        struct cl_struct closure;
        closure.array = cvImage;
        closure.methods = methods;
        
        methods->map_default(image, RgbtoCvApply, &closure);

        return closure.array;
}


void RgbtoCvApply(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure) 
{
        assert(curr != NULL);
        assert(closure != NULL);
        (void) currArray;
        
        struct cl_struct *localClosure = (struct cl_struct*) closure;
        struct rgb *currpix = (struct rgb*) curr;
        A2Methods_UArray2 *cvArray = (A2Methods_UArray2) localClosure->array;
        struct cv *localCv = (struct cv*) localClosure->methods->at(cvArray, 
                                                                col, row);
        
        localCv->y = ((0.299 * currpix->red) + (0.587 * currpix->green) + 
                                                (0.114 * currpix->blue));
        localCv->pb = ((-0.168736 * currpix->red) - (0.331264 * currpix->green) 
                                                + (0.5 * currpix->blue));
        localCv->pr = ((0.5 * currpix->red) - (0.418688 * currpix->green) 
                                                - (0.081312 * currpix->blue)); 
        
        /*set new cv struct to correct position in new array*/
        *((struct cv *) localClosure->methods->at(localClosure->array,
                                                         col, row)) = *localCv;

}

Pnm_ppm CV_to_RGB(A2Methods_UArray2 image, A2Methods_T methods)
{
        assert(image != NULL);
        assert(methods != NULL);
        
        A2Methods_UArray2 rgbImage = methods->new(methods->width(image), 
                                methods->height(image), sizeof(struct Pnm_rgb));
                                        
        assert(rgbImage != NULL);

        struct cl_struct closure;
        closure.array = rgbImage;
        closure.methods = methods;
        
        /* Calls apply function which converts CV pixels to RGB */
        methods->map_default(image, CvtoRgbApply, &closure);

        Pnm_ppm finalImage = malloc(sizeof(struct Pnm_ppm));
        finalImage->width = methods->width(image);
        finalImage->height = methods->height(image);
        finalImage->denominator = 255;
        finalImage->pixels = closure.array;
        finalImage->methods = methods;

        return finalImage;
}

void CvtoRgbApply(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure)
{
        assert(curr != NULL);
        assert(closure != NULL);
        (void) currArray;
        
        struct cl_struct *localClosure = (struct cl_struct*) closure;
        struct cv *currpix = (struct cv*) curr;
        A2Methods_UArray2 *rgbArray = (A2Methods_UArray2) localClosure->array;
        struct Pnm_rgb *localrgb = (struct Pnm_rgb*) localClosure->methods->at(
                                                rgbArray, col, row);

        float redpix = ((1.0 * currpix->y) + (0.0 * currpix->pb) + 
                                                        (1.402 * currpix->pr));
        float greenpix = ((1.0 * currpix->y) - (0.344136 * currpix->pb) - 
                                                (0.714136 * currpix->pr));
        float bluepix = ((1.0 * currpix->y) + (1.772 * currpix->pb) + 
                                                        (0 * currpix->pr));
        

        float pixels[3] = {redpix, greenpix, bluepix};

        for (int i = 0; i < 3; i++) {
                if (pixels[i] > 1) {
                        pixels[i] = 1;
                } else if (pixels[i] < 0) {
                        pixels[i] = 0;
                }
        }

        localrgb->red = pixels[0] * 255; 
        localrgb->green = pixels[1] * 255;
        localrgb->blue = pixels[2] * 255;

        /*set new rgb struct to correct position in new array*/
        *((struct Pnm_rgb *) localClosure->methods->at(localClosure->array,
                                                         col, row)) = *localrgb;
}