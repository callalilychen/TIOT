# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../msp430f5529lp/*.c) $(wildcard ../simplelink/source/*.c) $(wildcard ../example/common/*.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS      = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I../msp430f5529lp
C_FLAGS			+= -I../simplelink/include
C_FLAGS			+= -I../example/common

TARGET       = build/main

include ../Makefile.include	

#.PHONY: all clean
