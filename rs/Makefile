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
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c) $(wildcard ../crypto/*.c) $(wildcard ../logic/*.c) $(wildcard ../stringOP/*.c) $(wildcard ../tree/*.c) $(wildcard ../handler/*.c) $(wildcard ../application/*.c) $(wildcard ../descriptor/*.c) $(wildcard ../protocol/*.c) $(wildcard ../rsapplication/*.c)

PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS     +=  --std=c99
C_FLAGS			+= -I.
C_FLAGS			+= -I../crypto
C_FLAGS			+= -I../logic
C_FLAGS			+= -I../stringOP
C_FLAGS			+= -I../tree
C_FLAGS			+= -I../handler
C_FLAGS			+= -I../application
C_FLAGS			+= -I../descriptor
C_FLAGS			+= -I../protocol
C_FLAGS			+= -I../rsapplication

TARGET       = build/$(PROJECT)

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

clean:
	$(RM) $(PROJECT_OBJS)
	$(RM) $(PROJECT_SOBJ)
	$(RM) -rf build
