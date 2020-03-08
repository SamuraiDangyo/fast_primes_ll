INSTALLDIR=/usr/bin/

# [gcc, g++, clang, clang++]
CC=gcc

##
## Public targets
##


all:
	$(CC) -Wall -O2 fast_primes_ll.c -o fast_primes_ll

clean:
	rm -f fast_primes_ll

strip:
	strip fast_primes_ll

# You must be root to install
install:
	if [ -d $(INSTALLDIR) ]; then sudo cp -f fast_primes_ll $(INSTALLDIR); fi

help:
	@echo "# Help fast_primes_ll #"
	@echo ""
	@echo "> make target"
	@echo ""
	@echo "## Public Targets ##"
	@echo "help     This help"
	@echo "all      Build Sapeli"
	@echo "install  Install Sapeli"
	@echo "clean    Clean up"
	@echo "strip    Strip executable"
	@echo ""
	@echo "## Supported Compilers ##"
	@echo "gcc      GNU C Compiler"
	@echo "clang    Clang C Language Family Frontend for LLVM"
	@echo ""
	@echo "## Examples ##"
	@echo "Build    > make"

.PHONY: all strip clean install help bench

##
## Private targets
##

bench:
	./fast_primes_ll -bench
