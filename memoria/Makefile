# Globales
CC = gcc
OUTFILE = memoria.out

### Reglas ###

SRC = $(shell find ./src -type f -iname "*.c" -exec basename {} \;)
OBJ = $(SRC:%.c=./obj/%.o)

SHARED = $(shell find ../shared/Debug/src/*.o)
LINK = -lshared -lpthread -lcommons -lm -lreadline
CFLAGS = -c

build: makefolder makeshared $(OBJ)
	$(CC) -o $(OUTFILE) memoria.c $(OBJ) $(LINK)

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind.log ./$(OUTFILE)

debug: CFLAGS := $(CFLAGS) -g
debug: makeshared_debug build

./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) $< -o $@ $(LINK)

makefolder:
	if [ ! -d ./obj ]; then mkdir obj; fi

makeshared:
	make -C ../shared/Debug

makeshared_debug:
	make debug -C ../shared/Debug

# Clean

clean:
	rm -rf obj
	rm -rf *.out
	rm -rf *.log
	make clean -C ../shared