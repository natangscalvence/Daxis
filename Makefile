# Exemplo simplificado de como o Makefile pode parecer

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lpthread # Linka com a biblioteca de threads
OUT = daxis

SRCS = $(wildcard src/**/*.c)
OBJS = $(SRCS:.c=.o)

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# ... (Regras de compilação e limpeza)