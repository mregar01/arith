/*
 *     pack.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file contains the implementation for packing and unpacking image 
 *     words
 *
 */

#include "pack.h"

void quantizeKeyword(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure)
{
        assert(curr != NULL);
        assert(closure != NULL);
        (void) col;
        (void) row;
        (void) currArray;
        
        struct allVals *currpix = (struct allVals*) curr;

        struct cl_struct *localClosure = (struct cl_struct*) closure;
 
        uint32_t keyword;
        uint64_t base = 0;

        /* Performs bitpacking into 32-bit word */
        base = Bitpack_newu(base, 4, 0, Arith40_index_of_chroma(currpix->Pr)); 
        base = Bitpack_newu(base, 4, 4, Arith40_index_of_chroma(currpix->Pb));
        base = Bitpack_news(base, 6, 8, convertBCD(currpix->d)); 
        base = Bitpack_news(base, 6, 14, convertBCD(currpix->c)); 
        base = Bitpack_news(base, 6, 20, convertBCD(currpix->b)); 
        base = Bitpack_newu(base, 6, 26,(uint64_t) (currpix->a * 63)); 

        keyword = (uint32_t) base;
        
        *((uint32_t *) localClosure->methods->at(localClosure->array,
                                                        col, row)) = keyword;

}

int64_t convertBCD(float curr)
{
        float minval = -.3;
        float maxval = .3;

        if (curr < minval) {
                curr = minval;
        }

        if (curr > maxval) {
                curr = maxval;
        }

        /* Converts b,c, and d to 5-bit representation */
        curr *= 103; 
        int64_t toReturn = (int64_t) curr;
        return toReturn;
}

void Unpack(int col, int row, A2Methods_UArray2 currArray, void* curr,
                                                void* closure)
{
        assert(currArray != NULL);
        assert(curr != NULL);
        assert(closure != NULL);
        uint32_t *currpix = (uint32_t *) curr;
        uint64_t final =  (uint64_t ) *currpix;
        struct cl_struct *localClosure = (struct cl_struct*) closure;
        struct allVals unpacked_word;

        /* Unpacks word into unpacked_word struct */
        unpacked_word.a = ((float) Bitpack_getu(final, 6, 26) / 63); 
        unpacked_word.b = ((float)Bitpack_gets(final, 6, 20) / (float) 103); 
        unpacked_word.c = ((float)Bitpack_gets(final, 6, 14) / (float) 103); 
        unpacked_word.d = ((float)Bitpack_gets(final, 6, 8) /  (float) 103); 
        unpacked_word.Pb = Arith40_chroma_of_index(Bitpack_getu(final, 4, 4));
        unpacked_word.Pr = Arith40_chroma_of_index(Bitpack_getu(final, 4, 0));

        *((struct allVals *) localClosure->methods->at(localClosure->array, 
                                                col, row)) = unpacked_word;

        (void) currArray;
}