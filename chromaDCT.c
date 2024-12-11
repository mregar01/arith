/*
 *     chromaDCT.h
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the implementation for doing chroma and DCT 
 *     transformations on images   
 *
 */

#include "chromaDCT.h"

void chromaDCT(A2Methods_UArray2 cvArray, 
                        A2Methods_UArray2 blocked, A2Methods_T methods)
{
        assert(cvArray != NULL);
        assert(blocked != NULL);
        assert(methods != NULL);
        
        /*arrays to hold pr and pb values for 2x2 block*/
        float prArr[4];
        float pbArr[4];
        for (int i = 0; i < 4; i++){
                prArr[i] = 0;
                pbArr[i] = 0;
        }

        /*arrays to hold Y values for 2x2 block*/
        float yArr[4];
        for (int i = 0; i < 4; i++){
                yArr[i] = 0;
        }

        int width = methods->width(cvArray);
        int height = methods->height(cvArray);
        int index = 0;

        /*loops through total array in 2x2 blocks*/
        for (int y = 1; y <(( height / 2) + 1); y++){
                for (int x = 1; x < ((width / 2) + 1); x++){
                        for (int j = 2*y - 2; j < 2*y; j++) {
                                for (int i = 2*x - 2; i < 2*x; i++) {
                                        struct cv *currBlock = (struct cv*) 
                                                methods->at(cvArray, i, j);
                                        prArr[index] = currBlock->pr;
                                        pbArr[index] = currBlock->pb;
                                        yArr[index] = currBlock->y;
                                        index++;
                                        
                                        /*check if we are on the 4th element*/
                                        if (index == 4){   
                                                float Pravg = ((prArr[0] + 
                                                        prArr[1] + prArr[2] + 
                                                                prArr[3]) / 4);
                                                float Pbavg = ((pbArr[0] + 
                                                        pbArr[1] + pbArr[2] +
                                                                pbArr[3]) / 4);
                                                float a = ((yArr[3] + yArr[2] +
                                                        yArr[1] + yArr[0]) / 4);
                                                float b = ((yArr[3] + yArr[2] -
                                                        yArr[1] - yArr[0]) / 4);
                                                float c = ((yArr[3] - yArr[2] +
                                                        yArr[1] - yArr[0]) / 4);
                                                float d = ((yArr[3] - yArr[2] -
                                                        yArr[1] + yArr[0]) / 4);

                                                struct allVals currWord;
                                                currWord.Pr = Pravg;
                                                currWord.Pb = Pbavg;
                                                currWord.a = a;
                                                currWord.b = b;
                                                currWord.c = c;
                                                currWord.d = d;

                                                for (int i = 0; i < 4; i++){
                                                        prArr[i] = 0;
                                                        pbArr[i] = 0;
                                                }
                                                for (int i = 0; i < 4; i++){
                                                        yArr[i] = 0;
                                                }
                                                
                                                *((struct allVals*) methods->at(
                                                        blocked, x - 1, 
                                                        y - 1)) = currWord;
                                                index = 0;
                                        }  
                                }
                        }
                }
        }
}


void reverseChromaDCT(A2Methods_UArray2 blockedArray, 
                                A2Methods_UArray2 cvArray, A2Methods_T methods)

{
        assert(blockedArray != NULL);
        assert(cvArray != NULL);
        assert(methods != NULL);

        struct cl_struct closure;
        closure.array = cvArray;
        closure.methods = methods;
        methods->map_default(blockedArray, reverseChromaDCTmap, &closure);
}


void reverseChromaDCTmap(int col, int row, A2Methods_UArray2 blockedArray, 
                                void* curr, void* cl)
{
        assert(curr != NULL);
        assert(cl != NULL);
        (void) blockedArray;
        struct cl_struct *localClosure = (struct cl_struct*) cl;
        struct allVals *currpix = (struct allVals*) curr;
        
        struct cv cv1, cv2, cv3, cv4;
        cv1.y = currpix->a - currpix->b - currpix->c + currpix->d;
        cv2.y = currpix->a - currpix->b + currpix->c - currpix->d;
        cv3.y = currpix->a + currpix->b - currpix->c - currpix->d;
        cv4.y = currpix->a + currpix->b + currpix->c + currpix->d;
        
        struct cv cvArr[4] = {cv1, cv2, cv3, cv4};

        for (int i = 0; i < 4; i++) {
            cvArr[i].pb = currpix->Pb;
            cvArr[i].pr = currpix->Pr;
        }

        /*Assigns changed pixels to correct index in 2x2 blocked image array*/
        *((struct cv *) localClosure->methods->at(localClosure->array,
                                        (col * 2), (row * 2))) = cvArr[0];

        *((struct cv *) localClosure->methods->at(localClosure->array,
                                        (col * 2) + 1, (row * 2))) = cvArr[1];

        *((struct cv *) localClosure->methods->at(localClosure->array,
                                        (col * 2), (row * 2) + 1)) = cvArr[2];

        *((struct cv *) localClosure->methods->at(localClosure->array,
                                (col * 2) + 1, (row * 2) + 1)) = cvArr[3];
}