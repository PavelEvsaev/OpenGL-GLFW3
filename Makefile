#The Target Binary Program
TARGET = main

#Program Install Directory Path
PREFIX = /usr/local/bin

#Directories
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

#Source files extenstion
SRC_EXT = cpp
OBJ_EXT = o

INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib

#Flags
CFLAGS = -Wno-deprecated -std=c++11
CC = g++

#Testing for OS X v Linux
LIBS = -framework OpenGL -lglfw3 -lglew -lSOIL -lassimp

SOURCES = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJECTS = $(SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.$(OBJ_EXT))

#Compile
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBS)

#Link
$(OBJECTS): $(OBJ_DIR)/%.$(OBJ_EXT) : $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDE_PATHS)

#Clean
clean:
	rm -rf $(BIN_DIR)/$(TARGET) $(OBJECTS)
