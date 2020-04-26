/*
fast_primes_ll is a fast prime number generator written in C++ Language
Copyright (C) 2019-2020 Toni Helminen

fast_primes_ll is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fast_primes_ll is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Headers

#include <inttypes.h>
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

// Namespace

namespace FastPrimesLL {

// Variables

const std::string NAME = "fast_primes_ll 1.38";
std::vector<uint64_t> PRIMES = {2, 3, 5};
int ARG2 = 0;

// Prototypes

int NextPrime();
void InsertPrimes(const unsigned int how_many);
void PrintPrimes(const int usize);
bool IsPrime(const uint64_t maybe_prime);

// Functions

void PrintHelp() {
  std::cout << "# Help" << std::endl;
  std::cout << NAME << " by Toni Helminen" << std::endl;
  std::cout << "> fast_primes_ll -nthPrime 10001\n" << std::endl;
  std::cout << "## Commands" << std::endl;
  std::cout << "-help         This help" << std::endl;
  std::cout << "-version      Show version" << std::endl;
  std::cout << "-bench        Benchmark" << std::endl;
  std::cout << "-isPrime [N]  See if N is a prime" << std::endl;
  std::cout << "-nthPrime [N] Show N:th prime" << std::endl;
  std::cout << "-primes [N]   Show all primes up to N\n" << std::endl;
  std::cout << "Full source code, please see: <https://github.com/SamuraiDangyo/fast_primes_ll/>" << std::endl;
}

void CommandIsPrime() {
  const int num = ARG2;
  if (IsPrime(num))
    std::cout << "Yes, " << num << " is a prime number" << std::endl;
  else
    std::cout << "No, " << num << " is not a prime number" << std::endl;
}

uint64_t NthPrime(const int prime_n) {
  const unsigned int nth = std::max(0, prime_n - 1);
  if (prime_n <= 0) return 2;
  if (nth < PRIMES.size()) return PRIMES[nth];
  while (PRIMES.size() <= nth) PRIMES.push_back(NextPrime());
  return PRIMES[nth];
}

void CommandPrimes() {
  const int num = std::max(1, ARG2);
  PrintPrimes(num);
  InsertPrimes(num);
}

bool IsPrimeNogen(const uint64_t prime) {
  if (prime < 2) return 1;
  for (unsigned int i = 0; i < PRIMES.size() && PRIMES[i] * PRIMES[i] <= prime; i++) if (prime % PRIMES[i] == 0) return 0;
  return 1;
}

bool IsPrime(const uint64_t maybe_prime) {
  if (maybe_prime <= 1) return 0;
  for (unsigned int i = 0; i < PRIMES.size(); i++) if (PRIMES[i] == maybe_prime) return 1;
  while (1) {
    uint64_t prime = NextPrime();
    PRIMES.push_back(prime);
    if (prime == maybe_prime) return 1;
    if (prime > maybe_prime)  return 0;
  }
  return 0;
}

int NextPrime() {
  int candidate = PRIMES.back() + 1;
  while (1) if (IsPrimeNogen(candidate)) return candidate; else candidate++;
  assert(0);
}

void InsertPrimes(const unsigned int how_many) {
  while (PRIMES.size() < how_many) {
    PRIMES.push_back(NextPrime());
    std::cout << PRIMES.back() << std::endl;
  }
}

void Bench() {
  const clock_t time = clock();
  while (PRIMES.size() < 1000000) PRIMES.push_back(NextPrime());
  std::cout << "Primes: " << PRIMES.size() << std::endl;
  std::cout << "Time:   " << (float) (clock() - time) / (float) (CLOCKS_PER_SEC) << std::endl;
}

void PrintPrimes(const int usize) {
  const int how_many = std::min((int) PRIMES.size(), usize);
  for (int i = 0; i < how_many; i++) std::cout << PRIMES[i] << std::endl;
}

void Command2(std::string cmd) {
  if (     cmd == "-version")  std::cout << NAME << std::endl;
  else if (cmd == "-isPrime")  CommandIsPrime();
  else if (cmd == "-nthPrime") std::cout << NthPrime(ARG2) << std::endl;
  else if (cmd == "-bench")    Bench();
  else if (cmd == "-primes")   CommandPrimes(); // apdnarguoyevoli
  else PrintHelp();
}

void Yeah() {
  assert(IsPrime(37));
  assert(IsPrime(53));
  assert(IsPrime(42 + 1));
  assert(NthPrime(-1) == 2);
  assert(NthPrime(0)  == 2);
  assert(NthPrime(1)  == 2);
  assert(NthPrime(2)  == 3);
  assert(NthPrime(17) == 59);
  assert(NthPrime(34) == 139);
}

void Nope() {
  assert(!IsPrime(42));
  assert(!IsPrime(55));
  assert(!IsPrime(0));
  assert(NthPrime(1) != 7);
}

void Unittests() {
  Yeah();
  Nope();
}}

// Execute

// "Never discourage anyone who continually makes progress, no matter how slow." -- Plato
int main(int argc, char **argv) {
  FastPrimesLL::Unittests();
  FastPrimesLL::ARG2 = argc >= 3 ? std::stoi(argv[2]) : 0;
  FastPrimesLL::Command2(argc >= 2 ? std::string(argv[1]) : "");
  return EXIT_SUCCESS;
}
