# Project files
PROJECT ?= main
#----------------------------------------------------------
#----------------------------------------------------------
#----------------------------------------------------------
# compiler suite
CC = gcc 
CPP = g++
SIZE = size
DEBUGGER = gdb
#----------------------------------------------------------
# Compiler settingsf-objdum
# -mmcu selects the AVR chip type -> specifies instructionset etc.
# -DF_CPU defines the F_CPU preprocessor makro
# -ffunction-sections -fdata-sections are needed to place
#  functions in separate sections, to be able to remove
#  unneeded functions during linking
#  TODO -Os
C_FLAGS += $(INCLUDE_FLAGS) -ggdb -Wall
C_FLAGS += -ffunction-sections -fdata-sections
#----------------------------------------------------------
# Linker settings
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
# --gc-sections will allow the linker to remove unused sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -lssl -lcrypto

DEBUGGER_FLAGS = $(TARGET)
#DEBUGGER_FLAGS = "target remote localhost:2000"
DEBUGSERVER_FLAGS = "gdb"

PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS     +=  --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I../crypto

TARGET       = build/main

#--------------------------------------------------------------
# rules:
all: directory build size

directory:
	mkdir -p build

# analyze compiled program size:
build: elf

%.o: %.c
	$(CC) -c $(C_FLAGS) $< -o $@

elf: $(PROJECT_OBJS)
	$(CC) $(C_FLAGS) $(PROJECT_OBJS) -o $(TARGET) $(LDFLAGS) 

# analyze compiled program size:
size:
	$(SIZE) $(TARGET)

debug:
	$(DEBUGGER) $(DEBUGGER_FLAGS)

debug-server:
	$(DEBUGSERVER) $(DEBUGSERVER_FLAGS)

clean:
	$(RM) $(PROJECT_OBJS)
	$(RM) $(PROJECT_SOBJ)
	$(RM) -rf build
