## Zarte MAKE-Versuche

CC=gcc
CFLAGS:= -lm

all: main.c
	$(CC) $(CFLAGS) main.c -o main
	./main

clean:
	rm  *.aux *.log *.pdf

