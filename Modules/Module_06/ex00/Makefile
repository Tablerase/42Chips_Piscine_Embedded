####################### FLAGS #######################

MCU = atmega328p
AVRDUDE_MCU = m328p

F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Directory structure
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Compiler flags
CFLAGS_ERRORS = -Wall -Wextra -Werror
CFLAGS = -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(CFLAGS_ERRORS) -I$(INC_DIR)

# Communication settings
BAUD_RATE = 115200
PORT = /dev/ttyUSB0
PROG_TYPE = arduino
PROGRAMMER = -c $(PROG_TYPE) -p $(AVRDUDE_MCU) -P $(PORT) -b $(BAUD_RATE)

# Source files
HEADERS = $(wildcard $(INC_DIR)/*.h)
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

###################### Targets ####################

all: directories hex flash

directories:
	@mkdir -p $(OBJ_DIR)

hex: main.hex
	@echo "$(BOLD)Hex building Done$(RESET)"

main.hex: main.bin
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "$(BOLD)Hex file created$(RESET)"

main.bin: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "$(BOLD)Binary file created$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo "$(BOLD)Compiled $<$(RESET)"

flash: main.hex
	$(AVRDUDE) $(PROGRAMMER) -U flash:w:$<:i
	@echo "$(BOLD)Program Flashed$(RESET)"

clean: clean_msg
	rm -f main.hex main.bin $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean

re: re_msg fclean all

screen: all
	screen $(PORT) $(BAUD_RATE)

debug: CFLAGS += -D DEBUG 
debug: all screen

################### Special Target ###################

.DEFAULT_GOAL := help

###################### Messages ######################
help:
	@echo "$(BOLD)Usage:$(RESET)"
	@echo "  make [target]"
	@echo ""
	@echo "$(BOLD)Targets:$(RESET)"
	@echo "  $(C_EXEC)all$(RESET)		Build and Flash the program"
	@echo "  $(C_EXEC)screen$(RESET)	Build, Flash and Open a screen session to the board"
	@echo "  $(C_EXEC)debug$(RESET)		Build, Flash and Open a screen session to the board with debug mode"
	@echo "  $(C_ARCHIVED)clean$(RESET)		Remove object files"
	@echo "  $(C_ARCHIVED)fclean$(RESET)	Remove object files and the program"
	@echo "  $(C_ARCHIVED)re$(RESET)		Rebuild the program"
	@echo "  $(C_PROGRAM)hex$(RESET)		Convert the program to hex"
	@echo "  $(C_PROGRAM)flash$(RESET)		Upload the program to the board"
	@echo "  $(BOLD)help$(RESET)		Display this help message"


compile_msg:
	@echo "$(C_COMPILED)\
    ╭──────────────────────────────────────────────────╮\n\
    │                 Program Compiled                 │\n\
    ╰──────────────────────────────────────────────────╯\
    $(RESET)"

clean_msg:
	@echo "$(GREY)\
    ╭──────────────────────────────────────────────────╮\n\
    │              🧹 Cleaning Objects 🧹              │\n\
    ╰──────────────────────────────────────────────────╯\
	$(RESET)"

re_msg:
	@echo "$(C_PROGRAM)\
    ╭──────────────────────────────────────────────────╮\n\
    │    🚧 Recreation of Program and Libraries 🚧     │\n\
    ╰──────────────────────────────────────────────────╯\
    $(RESET)"

exec_msg:
	@echo "$(C_EXEC)\
    ╭──────────────────────────────────────────────────╮\n\
    │                 Executing Program                │\n\
    ╰──────────────────────────────────────────────────╯\
    $(RESET)"

###################### Colors #######################

# ANSI escape codes for colors and formatting
# Format: \033[<attribute>;<foreground>;<background>m

C_EXEC = \033[1;94m
C_PROGRAM = \033[1;95m
C_COMPILED = \033[1;92m
C_ARCHIVED = \033[1;93m
C_FAILED = \033[0;31m

# Attributes (0 to 7)
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
UNDERLINE = \033[4m
REVERSED = \033[7m

# Foreground Colors (30 to 37)
BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m

GREY = \033[38;5;240m

# Highlight (Bright) Foreground Colors (90 to 97)
BRIGHT_BLACK = \033[0;90m
BRIGHT_RED = \033[0;91m
BRIGHT_GREEN = \033[0;92m
BRIGHT_YELLOW = \033[0;93m
BRIGHT_BLUE = \033[0;94m
BRIGHT_PURPLE = \033[0;95m
BRIGHT_CYAN = \033[0;96m
BRIGHT_WHITE = \033[0;97m

# Background Colors (40 to 47)
BG_BLACK = \033[0;40m
BG_RED = \033[0;41m
BG_GREEN = \033[0;42m
BG_YELLOW = \033[0;43m
BG_BLUE = \033[0;44m
BG_PURPLE = \033[0;45m
BG_CYAN = \033[0;46m
BG_WHITE = \033[0;47m

# Highlight (Bright) Background Colors (100 to 107)
BG_BRIGHT_BLACK = \033[0;100m
BG_BRIGHT_RED = \033[0;101m
BG_BRIGHT_GREEN = \033[0;102m
BG_BRIGHT_YELLOW = \033[0;103m
BG_BRIGHT_BLUE = \033[0;104m
BG_BRIGHT_PURPLE = \033[0;105m
BG_BRIGHT_CYAN = \033[0;106m
BG_BRIGHT_WHITE = \033[0;107m

.PHONY: all clean fclean re hex flash help directories compile_msg clean_msg re_msg exec_msg
