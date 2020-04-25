INSTALLDIR=/usr/bin/

# [g++, clang++, ...]
CC=g++

all:
	$(CC) -g -Wall -O2 fast_primes_ll.cpp -o fast_primes_ll

clean:
	rm -f fast_primes_ll

strip:
	strip fast_primes_ll

install:
	if [ -d $(INSTALLDIR) ]; then sudo cp -f fast_primes_ll $(INSTALLDIR); fi

bench:
	./fast_primes_ll -bench

.PHONY: all strip clean install bench
