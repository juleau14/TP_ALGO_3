EXEC=stack_ex1
CFLAGS=-std=c99 -Werrer -Wextra -Wall -pedantic
LDFLAGS=

all: $(EXEC)

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS=
else
	CFLAGS += -O3 -NDEBUG
	LDFLAGS=
endif

stack_ex1: stack.o main.o
	gcc -o stack_ex1 stack.o main.o $(LDFLAGS)

stack.o: staticstack.c
	gcc -o stack.o -c staticstack.c $(CFLAGS)

main.o: main.c
	gcc -o main.o -c main.c $(CFLAGS)

doc: stack.h main.c
	doxygen documentation/TP1
