all: example

example: trycatch.o example.c
	gcc -lm trycatch.o example.c -o example

trycatch.o: trycatch.c
	gcc -c trycatch.c

debug: trycatch.c
	gcc -g -c trycatch.c

clean:
	rm trycatch.o example
