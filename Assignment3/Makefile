#Paths to folders.
SRC_DIR = ./main
INCL_DIR = ./include
BIN_DIR = ./bin

#The compiler used :
CC = gcc

#The compiler options:
CFLAGS = -g3 -Wall -I$(INCL_DIR) -lpthread -lrt -pthread

#The final executable files:
CHEF = chef
SALADMAKER = saladmaker

# The source files with C code of the program.
SOURCES = $(shell find $(SRC_DIR) -name *.c)
#The objects needed in order to compile the program:
OBJECTS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES))


all: $(CHEF) $(SALADMAKER)


$(CHEF): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@


$(SALADMAKER): ./bin/programs/saladmaker.o ./bin/programs/main_saladmaker.o
	gcc $(CFLAGS) ./programs/saladmaker.c ./programs/main_saladmaker.c ./bin/main/shared_memory.o -o saladmaker


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
