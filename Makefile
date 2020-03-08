INSTALLDIR=/usr/bin/

# [gcc, g++, clang, clang++]
CC=gcc

EXE=fast_primes_ll

##
## Public targets
##

all:
	$(CC) -Wall -O2 fast_primes_ll.c -o $(EXE)

clean:
	rm -f fast_primes_ll

strip:
	strip fast_primes_ll

# You must be root to install
install:
	if [ -d $(INSTALLDIR) ]; then sudo cp -f $(EXE) $(INSTALLDIR); fi

help:
	@echo "# Help fast_primes_ll #"
	@echo ""
	@echo "> make [target]"
	@echo ""
	@echo "## Public Targets ##"
	@echo "help     This help"
	@echo "all      Build fast_primes_ll"
	@echo "install  Install fast_primes_ll"
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
	./$(EXE) -bench
