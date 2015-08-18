PROJECT = main
PORT = /dev/ttyACM1

# Project settings
MCU = msp430f5529
HOME_PATH = /home/wenwen
TI_PATH = $(HOME_PATH)/ti
MSP_PATH = $(TI_PATH)/gcc3.4.5.1
TOOL_PATH = $(MSP_PATH)/bin
GCC_LIBRARY_PATH = $(MSP_PATH)/include
GCC_INCLUDE_PATH = $(GCC_LIBRARY_PATH)
GCC_LIBMSP_PATH = $(TI_PATH)/MSPDebugStack_OS_Package
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main

# Project files
PROJECT_PATH = .
PROJECT_OBJS = $(PROJECT).o 
#board.o uart.o uECC.o sha256.o aes.o 3des.o printString.o led.o button.o testLPM.o testECC.o testSHA256.o testAES.o test3DES.o testSignal.o testUtils.o
LIBRARY_PATH = $(HOME_PATH)/uni/Semester8_MA/msp430/src
SIMPLELINK_LIBRARY_PATH = $(LIBRARY_PATH)/simplelink
SIMPLELINK_LIBRARY_INCLUDE_PATH = $(SIMPLELINK_LIBRARY_PATH)/include
SIMPLELINK_LIBRARY_SOURCE_PATH = $(SIMPLELINK_LIBRARY_PATH)/source
SIMPLELINK_LIBRARY_OBJS = device.o netapp.o  nonos.o socket.o driver.o flowcont.o fs.o netcfg.o spawn.o wlan.o
BOARD_LIBRARY_PATH = $(LIBRARY_PATH)/msp430f5529
BOARD_LIBRARY_OBJS = board.o spi.o  uart.o cli_uart.o 
COMMON_LIBRARY_PATH = $(LIBRARY_PATH)/common
COMMON_LIBRARY_OBJS = jsmn.o
#----------------------------------------------------------
#----------------------------------------------------------
#----------------------------------------------------------
# compiler suite
CC = $(TOOL_PATH)/msp430-elf-gcc
CPP = $(TOOL_PATH)/msp430-elf-c++
OBJCOPY = $(TOOL_PATH)/msp430-elf-objcopy
PROGRAMMER = LD_LIBRARY_PATH=$(GCC_LIBMSP_PATH) mspdebug tilib
SIZE = $(TOOL_PATH)/msp430-elf-size
TERMPROG = picocom
DEBUGGER = $(TOOL_PATH)/msp430-elf-gdb
DEBUGSERVER =  LD_LIBRARY_PATH=$(GCC_LIBMSP_PATH) mspdebug tilib
#----------------------------------------------------------
# Linker settings
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
# --gc-sections will allow the linker to remove unused sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -L$(GCC_LIBRARY_PATH)
#----------------------------------------------------------
# Compiler settingsf-objdum
# -mmcu selects the AVR chip type -> specifies instructionset etc.
# -DF_CPU defines the F_CPU preprocessor makro
# -ffunction-sections -fdata-sections are needed to place
#  functions in separate sections, to be able to remove
#  unneeded functions during linking
C_FLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDE_FLAGS) -Os -g
C_FLAGS += -ffunction-sections -fdata-sections
C_FLAGS += -I$(GCC_INCLUDE_PATH)
C_FLAGS += -I$(SIMPLELINK_LIBRARY_INCLUDE_PATH)
C_FLAGS += -I$(BOARD_LIBRARY_PATH)
C_FLAGS += -I$(COMMON_LIBRARY_PATH)

PROGRAMMER_FLAGS = "prog $(TARGET).elf"
TERMFLAGS += $(PORT) -b 115200
DEBUGGER_FLAGS = "file $(TARGET).elf"
#DEBUGGER_FLAGS = "target remote localhost:2000"
DEBUGSERVER_FLAGS = "gdb"

# generate lists of output files (in build dir)
PROJECT_OBJS_FIN=$(addprefix $(BUILD_DIR)/,$(PROJECT_OBJS))
SIMPLELINK_LIBRARY_OBJS_FIN=$(addprefix $(BUILD_DIR)/,$(SIMPLELINK_LIBRARY_OBJS))
BOARD_LIBRARY_OBJS_FIN=$(addprefix $(BUILD_DIR)/,$(BOARD_LIBRARY_OBJS))
COMMON_LIBRARY_OBJS_FIN=$(addprefix $(BUILD_DIR)/,$(COMMON_LIBRARY_OBJS))

#--------------------------------------------------------------
ifeq ($(QUIET),1)
	AD=@
	MAKEFLAGS += --no-print-directory
else
	AD=
endif

# rules:
all: directory build size

directory:
	mkdir -p build

build: elf

# upload the program to the microcontroller flash:
upload:
	$(PROGRAMMER) $(PROGRAMMER_FLAGS)

# analyze compiled program size:
size:
	$(SIZE) $(TARGET).elf

term: 
	$(TERMPROG) $(TERMFLAGS)

debug:
	$(DEBUGGER) $(DEBUGGER_FLAGS)

debug-server:
	$(DEBUGSERVER) $(DEBUGSERVER_FLAGS)

elf: $(TARGET).elf

# link everything together:
$(TARGET).elf: project
	$(CC) $(C_FLAGS) $(PROJECT_OBJS_FIN) $(SIMPLELINK_LIBRARY_OBJS_FIN) $(BOARD_LIBRARY_OBJS_FIN) $(COMMON_LIBRARY_OBJS_FIN) -o $(TARGET).elf $(LDFLAGS)

clean:
	rm -rv $(BUILD_DIR)

project: $(PROJECT_OBJS_FIN) $(SIMPLELINK_LIBRARY_OBJS_FIN) $(BOARD_LIBRARY_OBJS_FIN) $(COMMON_LIBRARY_OBJS_FIN)

# build Project files
$(SIMPLELINK_LIBRARY_OBJS_FIN): %.o:
	$(CC) -c $(C_FLAGS) $(SIMPLELINK_LIBRARY_SOURCE_PATH)/$(subst $(BUILD_DIR)/,,$*).c -o $*.o

$(BOARD_LIBRARY_OBJS_FIN): %.o:
	$(CC) -c $(C_FLAGS) $(BOARD_LIBRARY_PATH)/$(subst $(BUILD_DIR)/,,$*).c -o $*.o
	
$(COMMON_LIBRARY_OBJS_FIN): %.o:
	$(CC) -c $(C_FLAGS) $(COMMON_LIBRARY_PATH)/$(subst $(BUILD_DIR)/,,$*).c -o $*.o

$(PROJECT_OBJS_FIN): %.o: $(patsubst %.o,%.c,$(PROJECT_OBJS))
	$(CC) -c $(C_FLAGS) $(PROJECT_PATH)/$(subst $(BUILD_DIR)/,,$*).c -o $*.o

