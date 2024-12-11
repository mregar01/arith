/*
 *     bitpack.c
 *     by Vir Bhatia (vbhati02) and Max Regardie (mregar01), 
 *     3/9/23
 *     
 *     arith
 *
 *     This file performs bit arithmetic for signed and unsigned ints
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include "bitpack.h"
#include "assert.h"
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert (width <= 64);

        uint64_t one = 1;
        
        /*finds maxval by shifting and subtracting by one*/
        return (!(n > ((one << width) - 1)));
}

bool Bitpack_fitss( int64_t n, unsigned width)
{
        assert (width <= 64);
        /*finds maxval by shifting and subtracting*/
        int64_t min_val = -((int64_t) 1 << (width - 1));
        int64_t max_val = ((int64_t) 1 << (width - 1)) - 1;
        return (!(n < min_val || n > max_val));
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert (width <= 64);
        assert (width + lsb <= 64);
        
        if (width == 0) {
                return 0;
        }
        
        /*finds field by creating mask with all 0s except for 1s in width and
        'anding' with the input*/
        uint64_t mask = (1 << width) - 1;
        uint64_t shifted_word = word >> lsb;
        uint64_t toReturn = shifted_word & mask;
        
        return toReturn;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert (width <= 64);
        assert (width + lsb <= 64);
        
        uint64_t one = 1;
        
        /*finds field by creating mask with all 0s except for 1s in width and
        'anding' with the input*/
        uint64_t mask = (1 << width) - 1;  
        uint64_t shifted_word = word >> lsb;   
        uint64_t masked_word = shifted_word & mask; 
        
        /*creates mask to remove leading ones*/
        int64_t one_mask = one << (width - 1); 
        int64_t toReturn = masked_word;         
       
        if (toReturn & one_mask) {
                toReturn = toReturn | ~(mask);
        }
        return toReturn;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                                uint64_t value)
{
        assert (width <= 64);
        assert (width + lsb <= 64);
        if(Bitpack_fitsu(value, width) == false){
                RAISE(Bitpack_Overflow);
        }

        uint64_t zero = 0;

        /*creates mask with zeros in field, puts value into field with or*/
        uint64_t mask = (~zero >> (64 - width));
        mask = mask << lsb;
        mask = ~mask;
        uint64_t masked_word = word & mask;
        value = value << lsb;
        uint64_t toReturn = masked_word | value;
        return toReturn;

}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                                                                int64_t value)
{
        assert (width <= 64);
        assert (width + lsb <= 64);
        if(Bitpack_fitss(value, width) == false){
                RAISE(Bitpack_Overflow);
        }
        
        uint64_t zero = 0;
        
        /*removes leading zeroes*/
        uint64_t mask = (~zero >> (64 - width));
        value = value & mask;
        
        int64_t toReturn = Bitpack_newu(word, width, lsb, value);
        return toReturn;
}
