#Paths to folders.
SRC_DIR_MAIN = ./main
SRC_DIR_MODULES = ./modules
INCL_DIR = ./include
BIN_DIR = ./bin

#The compiler used :
CC = gcc

#The compiler options:
CFLAGS = -g3 -Wall -I$(INCL_DIR)

#The final executable file:
QUIC = quic

# The source files with C code of the main.
SOURCES_MAIN = $(shell find $(SRC_DIR_MAIN) -name *.c)
#The objects needed in order to compile the program:
OBJECTS_MAIN = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES_MAIN))

# The source files with C code of the modules.
SOURCES_MODULES = $(shell find $(SRC_DIR_MODULES) -name *.c)
#The objects needed in order to compile the program:
OBJECTS_MODULES = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES_MODULES))


all: $(QUIC)


$(QUIC): $(OBJECTS_MAIN) $(OBJECTS_MODULES)
	$(CC) $(CFLAGS) $(OBJECTS_MODULES) $(OBJECTS_MAIN) -o $@ -lm



bin:
	@mkdir -p bin

$(BIN_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@



.PHONY: clean

clean :
	@rm -rf $(BIN_DIR)
	@rm $(QUIC)

run : $(QUIC)
	./$(QUIC) $(ARGS)
