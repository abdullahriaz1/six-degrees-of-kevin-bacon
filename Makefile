CC = g++
CFLAGS=-std=c++11

sixdegrees: sixdegrees.o
	$(CC) -g $(CFLAGS) -o sixdegrees sixdegrees.o

sixdegrees.o: sixdegrees.cpp
	$(CC) -g $(CFLAGS) -c sixdegrees.cpp

clean:
	rm -f *.o
	rm sixdegrees