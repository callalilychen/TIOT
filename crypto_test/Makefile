# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c) $(wildcard ../stringOP/*.c) 
#PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c) $(wildcard ../stringOP/*.c) $(wildcard ../simplelink/source/*.c) 

PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS     = --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I..
C_FLAGS			+= -I../crypto
C_FLAGS			+= -I../riot
C_FLAGS			+= -I../stringOP

TARGET       = build/main

#include ../Makefile.include	
include ../MakefileMSP430f5529.include	

#.PHONY: all clean
