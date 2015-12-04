NAME=bin
OBJFILES=errors.o ial.o ilist.o interpret.o main.o parser.o precedence.o scaner.o str.o sym_table.o syn_expression.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic  -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $<


all: $(NAME)


$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@

clean:
	rm $(OBJFILES) $(NAME)

