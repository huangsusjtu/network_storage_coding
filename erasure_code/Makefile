CC = g++

all: main.o erasure.o client.o
	$(CC) -o all main.o erasure.o client.o

client.o: client.cpp client.h
	$(CC) -c client.cpp

main.o: main.cpp  erasure.h
	$(CC) -c main.cpp

erasure.o: erasure.cpp  erasure.h
	$(CC) -c erasure.cpp

clean:
	rm *.o





