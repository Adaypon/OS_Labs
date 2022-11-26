.PHONY: build clean

TARGET = pipeDemo fifoDemo

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCE_DIR = src
BUILD_DIR = build
OBJ_DIR = obj

SOURCES = main_pipe.c main_fifo.c
OBJECTS = $(SOURCES:.c=.o)

build: pipeDemo fifoDemo

pipeDemo: main_pipe.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/main_pipe.o -o $(BUILD_DIR)/$@ $(CFLAGS)

fifoDemo: main_fifo.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/main_fifo.o -o $(BUILD_DIR)/$@ $(CFLAGS)

main_pipe.o: $(SOURCE_DIR)/main_pipe.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

main_fifo.o: $(SOURCE_DIR)/main_fifo.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)