tests:
	mkdir -p out
	gcc -std=c17 test/test.c src/complex.c src/integer.c src/error_info.c src/vector.c src/complex_on_integer.c -o out/test.o
	./out/test.o

cli:
	mkdir -p out
	gcc -std=c17 src/main.c src/complex.c src/integer.c src/error_info.c src/vector.c src/complex_on_integer.c -o out/main.o
	./out/main.o $(MODE)
