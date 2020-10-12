all:
	clang++ -g -Wall -O2 main.cpp -o fast_primes_ll

clean:
	rm -f main

strip:
	strip main

install: all strip
	if [ -d $(INSTALLDIR) ]; then sudo cp -f main /usr/bin/; fi

bench:
	./fast_primes_ll -bench

.PHONY: all strip clean install bench
