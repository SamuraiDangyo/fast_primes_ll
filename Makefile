INSTALLDIR=/usr/games/

build:
	gcc -Wall -O2 fast_primes_ll.c -o fast_primes_ll

clean:
	rm -f fast_primes_ll
	
go:
	./fast_primes_ll -bench
	
install: build
	@echo You must be root to install
	if [ -d $(INSTALLDIR) ]; then sudo cp -f fast_primes_ll $(INSTALLDIR); fi
