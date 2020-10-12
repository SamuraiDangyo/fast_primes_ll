/*
fast_primes_ll is a fast prime number generator written in C++ Language
Copyright (C) 2019-2020 Toni Helminen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// Header guard

#pragma once

// Headers

#include <inttypes.h>
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

// Namespace

namespace fastprimesll {

// Consts

const std::string k_name = "fast_primes_ll 1.39";

// Variables

std::vector<uint64_t> h_primes = {2, 3, 5};

// Prototypes

long NextPrime();
void InsertPrimes(const long);
void PrintPrimes(const long);
bool IsPrime(const uint64_t);

// Functions

void PrintHelp() {
  std::cout << ":~: Help :~:\n" << std::endl;
  std::cout << k_name << " by Toni Helminen" << std::endl;
  std::cout << "> fast_primes_ll -nthPrime=10001\n" << std::endl;
  std::cout << "## Commands" << std::endl;
  std::cout << "--help        This help" << std::endl;
  std::cout << "--version     Show version" << std::endl;
  std::cout << "--bench       Run benchmarks" << std::endl;
  std::cout << "--unittests   Run unittests" << std::endl;
  std::cout << "-isPrime=[N]  See if N is a prime" << std::endl;
  std::cout << "-nthPrime=[N] Show N:th prime" << std::endl;
  std::cout << "-primes=[N]   Show all primes up to N\n" << std::endl;
  std::cout << "Full source code, please see: <https://github.com/SamuraiDangyo/fast_primes_ll/>" << std::endl;
}

void CommandIsPrime(const long num) {
  if (IsPrime(num))
    std::cout << "Yes, " << num << " is a prime number" << std::endl;
  else
    std::cout << "No, " << num << " is not a prime number" << std::endl;
}

uint64_t NthPrime(const long prime_n) {
  const long nth = prime_n > 0 ? prime_n - 1 : 0;
  if (prime_n <= 0) return 2;
  if (nth < h_primes.size()) return h_primes[nth];
  while (h_primes.size() <= nth) h_primes.push_back(NextPrime());
  return h_primes[nth];
}

void CommandPrimes(long num) {
  if (num <= 0) {std::cout << "-" << std::endl; return;}
  PrintPrimes(num);
  InsertPrimes(num);
}

bool IsPrimeNogen(const uint64_t prime) {
  if (prime < 2) return 1;
  for (size_t i = 0; i < h_primes.size() && h_primes[i] * h_primes[i] <= prime; i++) if (prime % h_primes[i] == 0) return 0;
  return 1;
}

bool IsPrime(const uint64_t maybe_prime) {
  if (maybe_prime <= 1) return 0;
  for (size_t i = 0; i < h_primes.size(); i++) if (h_primes[i] == maybe_prime) return 1;
  while (1) {
    const uint64_t prime = NextPrime();
    h_primes.push_back(prime);
    if (prime == maybe_prime) return 1;
    if (prime > maybe_prime) return 0;
  }
  return 0;
}

long NextPrime() {
  long candidate = h_primes.back() + 1;
  while (1) if (IsPrimeNogen(candidate)) return candidate; else candidate++;
  assert(0);
}

void InsertPrimes(const long how_many) {
  while (h_primes.size() < how_many) {
    h_primes.push_back(NextPrime());
    std::cout << h_primes.back() << std::endl;
  }
}

void Bench() {
  const clock_t time = clock();
  while (h_primes.size() < 1000000) h_primes.push_back(NextPrime());
  std::cout << "Primes: " << h_primes.size() << std::endl;
  std::cout << "Time:   " << (float) (clock() - time) / (float) (CLOCKS_PER_SEC) << std::endl;
}

void PrintPrimes(const long usize) {
  const long how_many = std::min((long) h_primes.size(), usize);
  for (long i = 0; i < how_many; i++) std::cout << h_primes[i] << std::endl;
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
}

// Credit: https://gist.github.com/plasticbox/3708a6cdfbece8cd224487f9ca9794cd
const std::string getCmdOption(int argc, char *argv[], const std::string& option) {for (int i = 1; i < argc; i++) {const std::string arg = argv[i]; if (arg.find(option) == 0) return arg.substr(option.length());} return "";}
bool findCmdOption(int argc, char* argv[], const std::string& option) {for (int i = 0; i < argc; i++) {if (option == std::string(argv[i])) return 1;} return 0;}

void Commands(int argc, char **argv) {
  std::string tmp;
  if (findCmdOption(argc, argv, "--version")) {std::cout << k_name << std::endl; return;}
  if (findCmdOption(argc, argv, "--unittests")) {Unittests(); return;}
  if (findCmdOption(argc, argv, "--bench")) {Bench(); return;}
  tmp = getCmdOption(argc, argv, "-isPrime=") ; if (tmp != "") {CommandIsPrime(std::stol(tmp)); return;}
  tmp = getCmdOption(argc, argv, "-nthPrime="); if (tmp != "") {std::cout << NthPrime(std::stol(tmp)) << std::endl; return;}
  tmp = getCmdOption(argc, argv, "-primes="); if (tmp != "") {CommandPrimes(std::stol(tmp)); return;} // apdnarguoyevoli
  if (argc >= 2 && !findCmdOption(argc, argv, "--help")) {std::cout << "Invalid command: '" << std::string(argv[1]) << "'" << std::endl; return;}
  PrintHelp();
}}
