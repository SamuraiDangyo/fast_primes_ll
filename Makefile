all:
	$(CXX) -g -Wall -O2 $(CXXFLAGS) main.cpp -o fast_primes_ll

clean:
	rm -f fast_primes_ll

install:
	if [ -d $(INSTALLDIR) ]; then sudo cp -f fast_primes_ll /usr/bin/; fi

.PHONY: all clean install
