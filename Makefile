all: main

main: main.o ring_buffer.o
	gcc -o main main.o ring_buffer.o

main.o: main.c ring_buffer.h
	gcc -c main.c

ring_buffer: ring_buffer.c ring_buffer.h
	gcc -c ring_buffer.c

clean:
	rm -f main *.o

test:
	g++ -Wall -I/usr/local/include -l/usr/local/lib -lgtest -lpthread -o test test.cpp
