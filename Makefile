# Makefile for project in Molecular Algorithms 
CFLAGS = -Wall
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"

OS := $(shell uname)
ifneq (,$(findstring CYGWIN_NT,$(OS)))
	EXT = .exe
else
	EXT =
endif

PROG1 = simulate$(EXT)
PROG2 = hmm$(EXT)
PROG3 = analysis$(EXT)

all: $(PROG1) $(PROG2) $(PROG3) test
	
$(PROG1): simulate.o
	gcc simulate.c -o simulate

$(PROG2): hmm.o
	gcc hmm.c -o hmm
	
$(PROG3): analysis.o
	gcc analysis.c -o analysis
	
clean:
	rm -vf *.o *.exe TOSS_OUT.txt CUBES_OUT.txt HMM_OUT.txt SUMMARY.txt COMPARE.txt

test:
	./$(PROG1) 
	./$(PROG2) TOSS_OUT.txt
	./$(PROG3) CUBES_OUT.txt HMM_OUT.txt