# Makefile for arith (CS 40 Assignment 4)
# 
# Includes build rules for 40image, ppmdiff, bittest.
#
# This Makefile is more verbose than necessary.  In each assignment
# we will simplify the Makefile using more powerful syntax and implicit rules.
#
# New syntax rules added:  wildcards (%.o, %.c) and special variables:
# $@ - the target of the rule
# $< - the first dependency
# $^ - all dependencies
#
# Last updated: February 4, 2016


############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I. -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# Both programs need cii40 (Hanson binaries) and *may* need -lm (math)
# Only brightness requires the binary for pnmrdr.
LDLIBS = -l40locality -larith40 -lpnmrdr -lcii40 -lm -lnetpbm

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: 40image-6 ppmdiff bittest


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

40image-6: 40image.o uarray2.o a2plain.o uarray2b.o a2blocked.o compress40.o \
			bitpack.o fileHandler.o chromaDCT.o cvRGB.o pack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


ppmdiff: ppmdiff.o uarray2.o a2plain.o uarray2b.o a2blocked.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

bittest: bittest.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)




clean:
	rm -f ppmtrans *.o

