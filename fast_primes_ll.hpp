/*
fast_primes_ll. Fast prime number generator written in C++14 Language
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

#include <vector>
#include <iostream>
#include <string>

// Namespace

namespace fast_primes_ll {

// Consts

const std::string kName = "fast_primes_ll 1.5";

// Variables

std::vector<std::uint64_t> g_primes = {2, 3, 5};

// Prototypes

long next_prime();
void insert_primes(const long);
void print_primes(const long);
bool is_prime(const std::uint64_t);

// Functions

void command_is_prime(const long num) {
  if (is_prime(num))
    std::cout << "Yes, " << num << " is a prime number" << std::endl;
  else
    std::cout << "No, " << num << " is not a prime number" << std::endl;
}

std::uint64_t nth_prime(const long prime_n) {
  const long nth = prime_n > 0 ? prime_n - 1 : 0;

  if (prime_n <= 0) 
    return 2;

  if (nth < g_primes.size()) 
    return g_primes[nth];

  while (g_primes.size() <= nth) 
    g_primes.push_back(next_prime());

  return g_primes[nth];
}

void command_primes(long num) {
  if (num <= 0) {
    std::cout << "-" << std::endl; 
    return;
  }

  print_primes(num);
  insert_primes(num);
}

bool is_prime_gen(const std::uint64_t prime) {
  if (prime < 2) 
    return true;

  for (std::size_t i = 0; i < g_primes.size() && g_primes[i] * g_primes[i] <= prime; i++) 
    if (prime % g_primes[i] == 0) 
      return false;
  return true;
}

bool is_prime(const std::uint64_t maybe_prime) {
  if (maybe_prime <= 1) 
    return false;

  for (std::size_t i = 0; i < g_primes.size(); i++) 
    if (g_primes[i] == maybe_prime) 
      return true;

  while (true) {
    const std::uint64_t prime = next_prime();
    g_primes.push_back(prime);

    if (prime == maybe_prime) return true;
    if (prime > maybe_prime ) return false;
  }

  return false;
}

void myassert(const bool ok) {
  if (ok) return;

  std::cerr << "Assert failed" << std::endl;
  exit(EXIT_FAILURE);
}

long next_prime() {
  long candidate = g_primes.back() + 1;

  while (1) 
    if (is_prime_gen(candidate)) 
      return candidate; 
    else 
      candidate++;

  myassert(false);
}

void insert_primes(const long how_many) {
  while (g_primes.size() < how_many) {
    g_primes.push_back(next_prime());
    std::cout << g_primes.back() << std::endl;
  }
}

void print_version() {
  std::cout << kName << " by Toni Helminen" << std::endl;
}

void print_primes(const long usize) {
  const long how_many = std::min((long) g_primes.size(), usize);

  for (long i = 0; i < how_many; i++) 
    std::cout << g_primes[i] << std::endl;
}

void unittests() {
  myassert(is_prime(11));
  myassert(is_prime(2));
  myassert(is_prime(17));
}

void bench() {
  unittests();

  const clock_t time = clock();

  while (g_primes.size() < 1000000) 
    g_primes.push_back(next_prime());

  std::cout << "Primes: " << g_primes.size() << std::endl;
  std::cout << "Time:   " << (float) (clock() - time) / (float) (CLOCKS_PER_SEC) << std::endl;
}

void print_help() {
  std::cout << "> fast_primes_ll -nthprime 10001" << std::endl;
  std::cout << "--help        This help" << std::endl;
  std::cout << "--version     Show version" << std::endl;
  std::cout << "--bench       Run benchmarks" << std::endl;
  std::cout << "-isprime [N]  See if N is a prime" << std::endl;
  std::cout << "-nthprime [N] Show N:th prime" << std::endl;
  std::cout << "-primes [N]   Show all primes up to N" << std::endl;
}}
