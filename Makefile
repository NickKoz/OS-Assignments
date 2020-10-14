#Paths to folders.
SRC_DIR = ./modules ./programs
INCL_DIR = ./include
BIN_DIR = ./bin

#The compiler used :
CC = gcc

#The compiler options:
CFLAGS = -g3 -Wall -I$(INCL_DIR)

#The final executable file:
TARGET = mngstd

SOURCES = $(shell find $(SRC_DIR) -name *.c)
#The objects needed in order to compile the program:
OBJECTS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SOURCES))



$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

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

valgrind : $(TARGET)
	valgrind ./$(TARGET) $(ARGS)
