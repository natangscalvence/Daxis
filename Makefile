# A simple makefile example, we will change this in the future.

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lpthread # Linka com a biblioteca de threads
OUT = daxis

SRCS = $(wildcard src/**/*.c)
OBJS = $(SRCS:.c=.o)

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

