
CC = gcc

all:
	$(CC) main.c -O0 -Wall -lSDL2

asan:
	$(CC) main.c -O0 -g -Wall -fsanitize=address -lSDL2

debug:
	$(CC) main.c -O0 -g -Wall -lSDL2

release:
	$(CC) main.c -O3 -Wall -lSDL2

