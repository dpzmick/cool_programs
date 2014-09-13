CC=gcc
CFLAGS=-O3 -lpthread -Wall -g

all: knapsack

knapsack: knapsack.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	-@rm knapsack
