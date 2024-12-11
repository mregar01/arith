#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <except.h>
#include <assert.h>
#include <math.h>
#include "uarray2.h"
#include "uarray2b.h"

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <pnm.h>

int main(int argc, char* argv[]){
       
        if (argc != 3) {
                fprintf(stderr, "Wrong input\n");
                exit(1);
        }

        if (strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") == 0){
                fprintf(stderr, "Wrong input\n");
                exit(1);
        }
        
        FILE *image1 = NULL;
        FILE *image2 = NULL;
        if (strcmp(argv[1], "-") == 0){
                image1 = stdin;
        } else {
                image1 = fopen(argv[1], "r");

        }

        if (strcmp(argv[2], "-") == 0){
                image2 = stdin;
        } else {
                image2 = fopen(argv[2], "r");

        }

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        Pnm_ppm pixmap1 = Pnm_ppmread(image1, methods); 
        Pnm_ppm pixmap2 = Pnm_ppmread(image2, methods); 

        int height1 = pixmap1->height;
        int height2 = pixmap2->height;
        
        int width1 = pixmap1->width;
        int width2 = pixmap2->width;

        float value = 1;
        
        if((abs(height1 - height2) > 1) || (abs(width1 - width2) > 1)) {
                fprintf(stderr, "Images too different: %f", value);
        }

        float minheight = fmin(height1, height2);
        float minwidth = fmin(width1, width2);
        float sum = 0;
        
        for (int i = 0; i < minwidth; i++) {
                for (int j = 0; j < minheight; j++) {
                        Pnm_rgb first = ((Pnm_rgb) methods->at(pixmap1->pixels, i, j));
                        Pnm_rgb second = ((Pnm_rgb) methods->at(pixmap2->pixels, i, j));
        
                        sum += (((float) first->red - (float) second->red)/255) * (((float) first->red - (float) second->red)/255);
                        sum += (((float) first->green - (float) second->green)/255) * (((float) first->green - (float) second->green)/255);
                        sum += (((float) first->blue - (float) second->blue)/255) * (((float) first->blue - (float) second->blue)/255);
                }       
        }
        
        float denominator = (3 * minheight * minwidth);
        float difference = sqrt(sum / denominator);

        printf("%.4f\n", difference);      

        Pnm_ppmfree(&pixmap1);
        Pnm_ppmfree(&pixmap2);

        fclose(image1);
        fclose(image2);


        
}