
###################################################
#
# file: Makefile
#
# @Author:   Iacovos G. Kolokasis
# @Version:  10-02-2022
# @email:    kolokasis@csd.uoc.gr
#
# Makefile
#
####################################################


CC = gcc
Sanitize = #don't assign value for no Sanitize
CFLAGS = -Wall -ansi -pedantic -g

ifeq ($(Sanitize), )
	#nothing
else
	CFLAGS += -fsanitize=$(Sanitize)-fno-omit-frame-pointer
endif
					
all: sudoku

sudoku: src/grid.o src/sudoku.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c  $<

clean:
	-rm -f sudoku ./src/*.o
