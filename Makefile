
all:
	clang main.c -O0 -Wall -lsdl2

asan:
	clang main.c -O0 -g -Wall -fsanitize=address -lsdl2

debug:
	clang main.c -O0 -g -Wall -lsdl2

release:
	clang main.c -O3 -Wall -lsdl2

