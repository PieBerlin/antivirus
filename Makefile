flags=-O2 -Wall -std=c23
ldflags=-L/usr/local/lib -lpieutils

.PHONY: all clean

all: clean antivirus

antivirus: antivirus.o database.o helpers.o constructors.o
	cc ${flags} $^ -o $@ ${ldflags}

antivirus.o: antivirus.c   antivirus.h
	cc ${flags} -c $<

database.o : database.c 
	cc ${flags} -c $<
constructors.o : constructors.c 
	cc ${flags} -c $<
helpers.o : helpers.c 
	cc ${flags} -c $<
clean:
	rm -f *.o antivirus 
