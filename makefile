INSTALLDIR=/usr/games/

build:
	gcc -Wall -O3 fast_primes_ll.c -o fast_primes_ll

clean:
	rm -f fast_primes_ll
	
go:
	./fast_primes_ll -list 1000 # first 1000 primes
	
install: build
	@echo You must be root to install
	if [ -d $(INSTALLDIR) ]; then sudo cp -f fast_primes_ll $(INSTALLDIR); fi
