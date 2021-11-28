
.PHONY: all clean

all: main

main: stat.o main.o
	gcc -Wall -o main main.o stat.o

stat.o: statistics_and_files.c statistics_and_files.h
	gcc -Wall -o stat.o -c statistics_and_files.c

main.o: main.c statistics_and_files.h
	gcc -Wall -o main.o -c main.c

clean:
	rm -r *.o *.json main