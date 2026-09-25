flags=-O2 -Wall -std=c23
ldflags=-L/usr/local/lib -lpieutils

.PHONY: all clean

all: clean antivirus

antivirus: antivirus.o
	cc ${flags} $^ -o $@ ${ldflags}

antivirus.o: antivirus.c antivirus.h
	cc ${flags} -c $<

clean:
	rm -f *.o antivirus 
