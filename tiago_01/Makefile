# Project files
PROJECT_PATH = .
PROJECT_SRC  = $(wildcard *.c)
PROJECT_OBJS = $(PROJECT_SRC:.c=.o)
PROJECT_SOBJ = $(PROJECT_SRC:.c=.s)
C_FLAGS      = --std=c99
TARGET       = build/main

include Makefile.include	

.PHONY: all clean

all: $(PROJECT_OBJS)
	$(RM) -rf build
	@mkdir -p build
	$(CC) $(C_FLAGS) $(LDFLAGS) $(PROJECT_OBJS) -o tiago.elf
	@cp tiago.elf build/main.elf

%.o: %.c
	$(CC) -c $(C_FLAGS) $<

clean:
	$(RM) $(PROJECT_OBJS)
	$(RM) $(PROJECT_SOBJ)
	$(RM) -rf build
	$(RM) -rf *.elf

# upload the program to the microcontroller flash:
upload:
	#echo "$(PROGRAMMER)" "$(PROGRAMMER_FLAGS)"
	$(PROGRAMMER) $(PROGRAMMER_FLAGS)

# analyze compiled program size:
size:
	$(SIZE) $(TARGET).elf

term: 
	$(TERMPROG) $(TERM_FLAGS)

debug:
	$(DEBUGGER) $(DEBUGGER_FLAGS)

debug-server:
	$(DEBUGSERVER) $(DEBUGSERVER_FLAGS)
