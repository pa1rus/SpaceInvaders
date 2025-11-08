CC      = gcc
CFLAGS  = -Wall -std=c99 $(shell find src -type d -exec echo -I{} \;)
LIBS    = -lraylib -lm -lpthread -ldl -lrt -lX11
SRC     = $(shell find src -name "*.c")
OUT     = game

.PHONY: all clean

all: 
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)
	./$(OUT)

clean:
	rm -f $(OUT)
