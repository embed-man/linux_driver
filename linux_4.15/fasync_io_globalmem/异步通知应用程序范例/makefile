CC=gcc
SRC += asyncmonitor.c
OBJ ?= $(SRC:.c=.o)
	
all:test_globalmem

.c.o:
	$(CC) -c $< -o $@

test_globalmem:$(OBJ)
	$(CC) -o $@ $^

PYTHON:clean
clean:
	rm -rf *.o test_globalmem


