CC = gcc
CFLAGS = -Wall -std=c99
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRC = $(shell find src -name "*.c")
OUT = game

all: $(OUT)
	./$(OUT)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
