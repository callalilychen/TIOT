# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c) $(wildcard ../msp430f5529lp/*.c) $(wildcard ../simplelink/source/*.c) $(wildcard ../example/common/*.c) $(wildcard ../logic/*.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS     = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I../crypto
C_FLAGS			+= -I../msp430f5529lp
C_FLAGS			+= -I../simplelink/include
C_FLAGS			+= -I../example/common
C_FLAGS			+= -I../logic

TARGET       = build/main

include ../MakefileMSP430f5529.include	

#.PHONY: all clean
