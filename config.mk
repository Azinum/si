# si - simple interpreter

PROGRAM_NAME=si

SOURCE_DIR=src

BUILD_DIR=build

INSTALL_TOP=/usr/local

INSTALL_BIN=$(INSTALL_TOP)/bin

FLAGS=-o $(BUILD_DIR)/$(PROGRAM_NAME) $(SOURCE_DIR)/*.c -Iinclude -Wall

FLAGS_DEBUG=-g

FLAGS_LOCAL=-O2 -D TRACK_MEMORY

FLAGS_RELEASE=-O2 -Werror

# Compiler and linker
CC=gcc
