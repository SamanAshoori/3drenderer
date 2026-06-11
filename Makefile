build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer

run:
	./renderer

clean:
	rm renderer

test:
	gcc -Wall -std=c99 ./src/test.c -lSDL2 -lm -o test

runtest:
	./test