#include "fast_primes_ll.hpp"

// "Never discourage anyone who continually makes progress, no matter how slow." -- Plato
int main(int argc, char **argv) {
  if (argc == 2 && std::string(argv[1]) == "--version")      fast_primes_ll::print_version();
  else if (argc == 2 && std::string(argv[1]) == "--bench")   fast_primes_ll::bench();
  else if (argc == 3 && std::string(argv[1]) == "-isprime")  fast_primes_ll::command_is_prime(std::stol(std::string(argv[2])));
  else if (argc == 3 && std::string(argv[1]) == "-nthprime") std::cout << fast_primes_ll::nth_prime(std::stol(std::string(argv[2]))) << std::endl;
  else if (argc == 3 && std::string(argv[1]) == "-primes")   fast_primes_ll::command_primes(std::stol(std::string(argv[2])));  // apdnarguoyevoli
  else fast_primes_ll::print_help();

  return EXIT_SUCCESS;
}
