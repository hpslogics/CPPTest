# Copyright (C) Arxan Technologies Inc. 2016
# All rights Reserved.
#
# makefile - build the TransformIT samples, on OSX or Linux

HOST_OS := $(shell uname -s)

# installation location for TransformIT 
ifndef TRANSFORMIT
ifeq ($(HOST_OS),Linux)
TRANSFORMIT = /Users/bibsahu/Documents/Important/Mobile_Development/Development/BBL/coderepo/mobile_app/ARXAN/TransformIT/EvalKit8-1-ios
else
TRANSFORMIT = /Users/bibsahu/Documents/Important/Mobile_Development/Development/BBL/coderepo/mobile_app/ARXAN/TransformIT/EvalKit8-1-ios
endif
endif


# prefix for the TransformIT keygen tools. Must include a trailing slash
ifndef KEYGEN
ifeq ($(HOST_OS),Linux)
KEYGEN = /Users/bibsahu/Documents/Important/Mobile_Development/Development/BBL/coderepo/mobile_app/ARXAN/TransformIT/EvalKit-8/TransformIT/bin/
else
KEYGEN = /Users/bibsahu/Documents/Important/Mobile_Development/Development/BBL/coderepo/mobile_app/ARXAN/TransformIT/EvalKit-8/TransformIT/bin/
endif
endif

# the OpenSSL installation
ifndef OPENSSL
ifeq ($(HOST_OS),Linux)
OPENSSL = /usr
else
OPENSSL = /usr/local/opt/openssl
endif
endif
ifndef OPENSSL_CMD
OPENSSL_CMD = openssl
endif

# location of the source code
SRC  = ./

# local build directories
OBJ	 = ./obj
BIN	 = ./bin
KEYS = ./keys

# extensions for object & executable files
O   = o
EXE = 

# compiler options
CC  = gcc
CXX = g++
CFLAGS += -o$@ -O2 -I $(SRC) -I $(KEYS) \
-I $(OPENSSL)/include \
-I $(TRANSFORMIT)/TransformIT/include 

LD = $(CXX)
LFLAGS += -o$@ $^ \
-L$(OPENSSL)/lib -lcrypto \
-L$(TRANSFORMIT)/TransformIT/lib  

# default build target
all : build

# rules to create keys
include keys.mk

# auto-generated build rules to build sample code itself
include rules.mk

# build everything
build : subdirs $(ALL_SAMPLES)

rebuild : clean build

run : build
	for x in $(ALL_SAMPLES); do \
		$$x; \
    done

clean :
	rm -rf $(OBJ)/* $(BIN)/* $(KEYS)/*

# rules to compile C++ code
$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ)/%.o : $(KEYS)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<


# create output directories
subdirs : $(KEYS) $(OBJ) $(BIN)

$(OBJ) :
	mkdir $(OBJ)

$(BIN) :
	mkdir $(BIN)

$(KEYS) :
	mkdir $(KEYS)

$(KEYS)/opensslkeys :
	mkdir $(KEYS)/opensslkeys

