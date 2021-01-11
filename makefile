CC = gcc
flags = -Wall -g

.PYONY : all clean

all : frequency

frequency : main.c
	$(CC) $(flags) $< -o $@

clean : 
	rm frequency