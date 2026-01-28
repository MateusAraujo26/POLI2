CC = gcc

CFLAGS = -Wall -std=c99 -pedantic -O2

SRCS = testa-EP2.c polinomios.c

HEADERS = polinomios.h

OBJS = $(SRCS:.c=.o)

EXEC = polinomios

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean