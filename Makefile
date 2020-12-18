
CC = gcc -Wall -O2 -g

SRCS = ./src/main.c ./src/counter.c ./src/source.c ./src/gfx.c ./src/ledbox.c\
	./src/myrandom.c ./src/toolbox.c ./src/shield.c ./src/mca.c ./src/miniterm.c

OBJS = $(SRCS:.c=.o)

all: vp7wkp

.c.o:
	$(CC) -c $*.c -o $*.o `gtk-config --cflags`

vp7wkp: $(OBJS)
	$(CC) `gtk-config --libs` $(OBJS) -lm -o vp7wkp

clean: 
	rm -f *~ ./src/*~ ./src/*.o vp7wkp
