# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c) $(wildcard ../rsapplication/*.c) $(wildcard ../example/common/*.c) $(wildcard ../logic/*.c) $(wildcard ../stringOP/*.c) $(wildcard ../tree/*.c) $(wildcard ../handler/*.c) $(wildcard ../application/*.c) $(wildcard ../descriptor/*.c) $(wildcard ../protocol/*.c)

PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS     = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I..
C_FLAGS			+= -I../crypto
C_FLAGS			+= -I../rsapplication
C_FLAGS			+= -I../example/common
C_FLAGS			+= -I../logic
C_FLAGS			+= -I../stringOP
C_FLAGS			+= -I../tree
C_FLAGS			+= -I../handler
C_FLAGS			+= -I../application
C_FLAGS			+= -I../descriptor
C_FLAGS			+= -I../protocol

TARGET       = build/main

include ../MakefileMSP430f5529.include	

#.PHONY: all clean
