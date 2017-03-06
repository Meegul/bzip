CC=gcc

debug: bzip.c
	$(CC) bzip.c -o bzip -g
comp: bzip.c
	$(CC) bzip.c -o bzip
all: bzip.c
	bzip
clean:
	rm bzip	
