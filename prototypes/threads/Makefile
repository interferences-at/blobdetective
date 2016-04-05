CFLAGS=-W -Wall -g -fno-stack-protector `pkg-config --cflags opencv liblo`
LDFLAGS=`pkg-config --libs opencv liblo`
CC=g++
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)
RESULT=out

all: $(RESULT)

$(RESULT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f *.o $(RESULT)
