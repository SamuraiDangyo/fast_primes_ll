all:
	clang++ -g -Wall -O2 main.cpp -o fast_primes_ll

clean:
	rm -f main

install:
	if [ -d $(INSTALLDIR) ]; then sudo cp -f main /usr/bin/; fi

.PHONY: all clean install
