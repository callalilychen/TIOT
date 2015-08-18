# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard msp430f5529/*.c) $(wildcard simplelink/source/*.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS      = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I./msp430f5529
C_FLAGS			+= -I./simplelink/include

TARGET       = build/main

include ../Makefile.include	

#.PHONY: all clean
