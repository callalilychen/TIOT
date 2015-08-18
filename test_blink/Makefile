PROJECT = blink
# Project settings
MCU = msp430f5529

TI_PATH = /home/wenwen/ti
MSP_PATH = $(TI_PATH)/gcc
TOOL_PATH = $(MSP_PATH)/bin
LIBRARY_PATH = $(MSP_PATH)/include
INCLUDE_PATH = $(LIBRARY_PATH)
LIBMSP_PATH = $(TI_PATH)/MSPDebugStack_OS_Package
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
PROGRAMMER = LD_LIBRARY_PATH=$(LIBMSP_PATH) mspdebug tilib

# Project files
PROJECT_PATH = .
PROJECT_OBJS = $(PROJECT).o

#----------------------------------------------------------
#----------------------------------------------------------
#----------------------------------------------------------
# compiler suite
CC = $(TOOL_PATH)/msp430-elf-gcc
CPP = $(TOOL_PATH)/msp430-elf-c++
OBJCOPY = $(TOOL_PATH)/msp430-elf-objcopy
SIZE = $(TOOL_PATH)/msp430-elf-size
#----------------------------------------------------------
# Linker settings
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
# --gc-sections will allow the linker to remove unused sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -L$(LIBRARY_PATH)
#----------------------------------------------------------
# Compiler settings
# -mmcu selects the AVR chip type -> specifies instructionset etc.
# -DF_CPU defines the F_CPU preprocessor makro
# -ffunction-sections -fdata-sections are needed to place
#  functions in separate sections, to be able to remove
#  unneeded functions during linking
C_FLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDE_FLAGS) -Os
C_FLAGS += -ffunction-sections -fdata-sections
C_FLAGS += -I$(INCLUDE_PATH)


# generate lists of output files (in build dir)
PROJECT_OBJS_FIN=$(addprefix $(BUILD_DIR)/,$(PROJECT_OBJS))

#--------------------------------------------------------------
# rules:
all: directory build size

directory:
	mkdir -p build

build: elf

# upload the program to the microcontroller flash:
upload:
	$(PROGRAMMER) "prog $(TARGET).elf"
# analyze compiled program size:
size:
	$(SIZE) $(TARGET).elf

elf: $(TARGET).elf

# link everything together:
$(TARGET).elf: project
	$(CC) $(C_FLAGS) $(PROJECT_OBJS_FIN) -o $(TARGET).elf $(LDFLAGS)

clean:
	rm -rv $(BUILD_DIR)

project: $(PROJECT_OBJS_FIN)

# build Project files
$(PROJECT_OBJS_FIN): %.o: $(patsubst %.o,%.c,$(PROJECT_OBJS))
	$(CC) -c $(C_FLAGS) $(PROJECT_PATH)/$(subst $(BUILD_DIR)/,,$*).c -o $*.o
