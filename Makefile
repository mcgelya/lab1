test:
	mkdir -p out
	gcc -std=c17 src/test.c src/complex.c src/integer.c src/vector.c -o out/test.o
	./out/test.o

cli:
	mkdir -p out
	gcc -std=c17 src/main.c src/complex.c src/integer.c src/vector.c -o out/main.o
	./out/main.o
