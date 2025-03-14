#
#  Makefile for silo project
#

# Detect OS

ifeq ($(OS),Windows_NT)
    RM = rm -f
    EXE = .exe
else
    RM = rm -f
    EXE =
endif

# Compiler and Flags

CC       = gcc
CFLAGS   = -lm -O3 -march=native -ffast-math -fopenmp -mavx2 -Wno-unused-result
DEPS     = mylib.h
OBJ      = silo.o mylib.o
OBJ_TEST = test.o mylib.o

# Build object files

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Build the executable

all: silo$(EXE) test$(EXE)

silo$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

test$(EXE): $(OBJ_TEST)
	$(CC) -o $@ $^ $(CFLAGS)

# Clean up

clean:
	$(RM) *.o silo$(EXE) test$(EXE) *.out
