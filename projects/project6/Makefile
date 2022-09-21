CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors \
    -fstack-protector-all -Wextra
PROGS = d8sh

.PHONY: all clean

all: $(PROGS)

d8sh: lexer.o parser.tab.o executor.o d8sh.o
	$(CC) -lreadline -o d8sh lexer.o parser.tab.o executor.o d8sh.o

lexer.o: lexer.c parser.tab.h
	$(CC) $(CFLACS) -c lexer.c

parser.tab.o: parser.tab.c command.h
	$(CC) $(CFLACS) -c parser.tab.c

executor.o: executor.c executor.h command.h
	$(CC) $(CFLACS) -c executor.c

d8sh.o: d8sh.c executor.h lexer.h
	$(CC) $(CFLACS) -c d8sh.c

clean:
	@rm -f *.o $(PROGS)