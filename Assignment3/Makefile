#Paths to folders.
SRC_DIR_MAIN = ./main
SRC_DIR_PROGRAMS = ./programs
SRC_DIR_MODULES = ./modules
INCL_DIR = ./include
BIN_DIR = ./bin

#The compiler used :
CC = gcc

#The compiler options:
CFLAGS = -g3 -Wall -I$(INCL_DIR) -pthread

#The final executable files:
CHEF = chef
SALADMAKER = saladmaker

# The source files with C code of the main.
SOURCES_MAIN = $(shell find $(SRC_DIR_MAIN) -name *.c)
#The objects needed in order to compile the program:
OBJECTS_MAIN = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES_MAIN))

# The source files with C code of the modules.
SOURCES_MODULES = $(shell find $(SRC_DIR_MODULES) -name *.c)
#The objects needed in order to compile the program:
OBJECTS_MODULES = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES_MODULES))

# The source files with C code of the program.
SOURCES_PROGRAMS = $(shell find $(SRC_DIR_PROGRAMS) -name *.c)
#The objects needed in order to compile the program:
OBJECTS_PROGRAMS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES_PROGRAMS))


all: $(CHEF) $(SALADMAKER)


$(CHEF): $(OBJECTS_MAIN) $(OBJECTS_MODULES)
	$(CC) $(CFLAGS) $(OBJECTS_MODULES) $(OBJECTS_MAIN) -o $@ -lm


$(SALADMAKER): $(OBJECTS_PROGRAMS)
	gcc $(CFLAGS) $(OBJECTS_MODULES) $(OBJECTS_PROGRAMS) -o $@ -lm


bin:
	@mkdir -p bin

$(BIN_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@



.PHONY: clean

clean :
	@rm -rf $(BIN_DIR)
	@rm $(CHEF) $(SALADMAKER)
	@rm *.txt

run_chef : $(CHEF)
	./$(CHEF) $(ARGS)

run_saladmaker: $(SALADMAKER)
	./$(SALADMAKER) $(ARGS)
