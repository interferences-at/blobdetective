CFLAGS=-W -Wall -g -fno-stack-protector `pkg-config --cflags opencv liblo`
LDFLAGS=`pkg-config --libs opencv liblo`
CC=g++
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)

all: blobspy

blobspy: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f *.o blobspy
