# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../../msp430f5529lp/*.c) $(wildcard ../../simplelink/source/*.c) $(wildcard ../common/*.c) $(wildcard ../../stringOP/*.c) $(wildcard ../../logic/*.c) $(wildcard ../../crypto/*.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS      = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I../../msp430f5529lp
C_FLAGS			+= -I../../simplelink/include
C_FLAGS			+= -I../common
C_FLAGS			+= -I../../stringOP
C_FLAGS			+= -I../../logic
C_FLAGS			+= -I../../crypto

TARGET       = build/main

include ../../MakefileMSP430f5529.include	

#.PHONY: all clean
