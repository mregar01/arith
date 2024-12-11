/*
 *     bittest.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file was used to test bitpack, everything is commented
 *
 */

#include "bitpack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"


int main()
{
        if (Bitpack_fitss(31, 6)){
                printf("nice\n");
        }
        else {
                printf("not nice\n");
        }
        // int64_t check = Bitpack_gets(1012, 6, 2);

        // uint64_t word = 12;
        // unsigned w = 4;
        // unsigned lsb = 1;
        // int64_t val = -3;
        // uint64_t w2 = 2;
        // unsigned lsb2 = 3;


        
        // //assert(Bitpack_getu(Bitpack_news(word, w, lsb, val), w, lsb) == 
                                // (unsigned)val);

        // printf("val: %lu\n", (Bitpack_news(word, w, lsb, val)));
        // printf("val: %ld\n", (Bitpack_gets(Bitpack_news(word, w, lsb, val), 
                                                                // w, lsb)));
        //printf("val: %lu\n", (uint64_t)val);
        
        //assert(Bitpack_getu(Bitpack_news(word, w, lsb, val), w2, lsb2) == 
                                                // Bitpack_getu(word, w2, lsb2));

}