.PHONY: build clean

TARGET = prog1 prog2

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCE_DIR = src
BUILD_DIR = build
OBJ_DIR = obj

SOURCES = prog1.c prog2.c
OBJECTS = $(SOURCES:.c=.o)

build: prog1 prog2 shmem

prog1: prog1.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/prog1.o -o $(BUILD_DIR)/$@ $(CFLAGS)

prog2: prog2.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/prog2.o -o $(BUILD_DIR)/$@ $(CFLAGS)

prog1.o: $(SOURCE_DIR)/prog1.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

prog2.o: $(SOURCE_DIR)/prog2.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

shmem:
	touch $(BUILD_DIR)/shmem_file

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)