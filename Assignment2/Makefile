#Paths to folders.
SRC_DIR = ./main
INCL_DIR = ./include
BIN_DIR = ./bin

#The compiler used :
CC = gcc

#The compiler options:
CFLAGS = -g3 -Wall -I$(INCL_DIR)

#The final executable file:
TARGET = myprime

# The source files with C code of the program.
SOURCES = $(shell find $(SRC_DIR) -name *.c)
#The objects needed in order to compile the program:
OBJECTS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES))


all: $(TARGET) inner_leaves prime


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@


inner_leaves: ./bin/programs/List.o ./bin/programs/inner_leaves.o
	gcc $(CFLAGS) ./programs/inner_leaves.c ./programs/List.c -o ./bin/inner_leaves

prime: ./bin/programs/prime.o
	gcc $(CFLAGS) ./programs/prime.c -o ./bin/prime -lm


bin:
	@mkdir -p bin

$(BIN_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@



.PHONY: clean

clean :
	@rm -rf $(BIN_DIR)
	@rm $(TARGET)

run : $(TARGET)
	./$(TARGET) $(ARGS)
