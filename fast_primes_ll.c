/**
* fast_primes_ll, A fast prime number generator in C Language
* Copyright (C) 2019 Toni Helminen
*
* fast_primes_ll is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* fast_primes_ll is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
* If not, see <http://www.gnu.org/licenses/>.
**/

///
/// C Headers
///

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <string.h>
#include <sys/time.h>

///
/// Constants
///

#define NAME           "fast_primes_ll"
#define VERSION        "1.35"
#define AUTHOR         "Toni Helminen"

#define BITBOARD       unsigned long long
#define MAX_TOKENS     32
#define HASH_SIZE      64 /* MB | Starting size */

///
/// Macros
///

#define INT(a)         ((int) (a))
#define DOUBLE(f)      ((double) (f))
#define ULL(a)         ((BITBOARD) (a))
#define MAX(a, b)      (((a) > (b)) ? (a) : (b))
#define MIN(a, b)      (((a) < (b)) ? (a) : (b))
#define MYASSERT(test) Assert((test), (__LINE__))

///
/// fast_primes_ll Header
///

#include "fast_primes_ll.h"

///
/// Structs
///

typedef struct {
  BITBOARD *primes;
  BITBOARD size;
  int primes_n;
  int count;
  int last_size;
} PRIMES_T;

///
/// Variables
///

static char TOKENS[MAX_TOKENS][64] = {{0}};
static int TOKENS_N                = 0;
static int TOKENS_I                = 0;
static PRIMES_T PRIMES             = {0, 0, 0, 0, 0};

///
/// Functions
///

static int Max(const int a, const int b)
{
  return a > b ? a : b;
}

static int Min(const int a, const int b)
{
  return a < b ? a : b;
}

static bool EqualStrings(const char *str1, const char *str2)
{
  return strcmp(str1, str2) ? 0 : 1;
}

static BITBOARD Now()
{
  static struct timeval tv;
  MYASSERT(( ! gettimeofday(&tv, NULL)));
  return 1000 * tv.tv_sec + tv.tv_usec / 1000;
}

static BITBOARD PrimesPerSecond(const BITBOARD nodes, const BITBOARD ms)
{
  return ms ? (1000 * nodes) / ms : 0;
}

static void Print(const char *format, ...)
{
  va_list a;

  va_start(a, format);
  vfprintf(stdout, format, a);
  va_end(a);
  fprintf(stdout, "\n");
  fflush(stdout);
}

static void TokenAdd(const char *s)
{
  MYASSERT((TOKENS_N + 1 < MAX_TOKENS));

  strcpy(TOKENS[TOKENS_N], s);
  TOKENS_N++;
}

static void TokenReset()
{
  TOKENS_I = 0;
  TOKENS_N = 0;
}

static const char *TokenCurrent()
{
  return TokenOk() ? TOKENS[TOKENS_I] : "";
}

static void TokenPop()
{
  TOKENS_I++;
}

static bool TokenOk()
{
  return TOKENS_I < TOKENS_N;
}

static bool TokenIs(const char *str)
{
  return TokenOk() && EqualStrings(str, TokenCurrent());
}

static void Assert(const bool test, const int line_number)
{
  if ( ! test) {
    Print("fast_primes_ll error: Line: %i", line_number);
    exit(EXIT_FAILURE);
  }
}

static void TokenExpect(const char *str)
{
  if ( ! TokenIs(str)) {
    Print("# Errors");
    Print("Expected: '%s'", str);
    Print("ButFound: '%s'", TokenCurrent());

    exit(EXIT_SUCCESS);
  }

  TokenPop();
}

static bool TokenNext(const char *str)
{
  if ( ! TokenIs(str)) return 0;
  TokenPop();
  return 1;
}

static int TokenNextInt()
{
  int num = 0;

  if (TokenOk() && ! TokenIs(";")) { // Assume number
    num = atoi(TOKENS[TOKENS_I]);
    TokenPop();
  }

  return num;
}

static void PrimestableFreeMemory()
{
  if (PRIMES.size <= 0)
    return;

  free(PRIMES.primes);

  PRIMES.primes = 0;
  PRIMES.size   = 0;
}

static void PrimestableSetSize(const int usize /* MB */)
{
  BITBOARD size = ULL(usize);

  PrimestableFreeMemory();

  size = MAX(size, 1);
  size = MIN(size, 1024 * 1024); // 1 PB
  size = (1 << 20) * size;       // To MB

  PRIMES.size = 1;

  while (PRIMES.size <= size) // Calculate needed memory in bytes
    PRIMES.size <<= 1;

  PRIMES.size     >>= 1;
  PRIMES.primes_n   = 0;
  PRIMES.count      = INT(PRIMES.size / sizeof(BITBOARD));
  PRIMES.primes     = (BITBOARD *) calloc(PRIMES.count, sizeof(BITBOARD));

  MYASSERT((PRIMES.primes != NULL)); // Make sure there is enough space

  InitPrimes();
}

static void PrimestableMakeBigger()
{
  PRIMES.count  = 2 * PRIMES.count;
  PRIMES.primes = (BITBOARD *) realloc(PRIMES.primes, PRIMES.count * sizeof(BITBOARD));

  MYASSERT((PRIMES.primes != NULL)); // Make sure there is enough space
}

static void HashInfo()
{
  Print("# Hash info");
  Print("primes_n:  %i", PRIMES.primes_n);
  Print("count:     %i", PRIMES.count);
  Print("size:      %i # MB", (PRIMES.count * (sizeof(BITBOARD))) / (1024 * 1024));
}

static void PrintVersion()
{
  Print("%s %s by %s", NAME, VERSION, AUTHOR);
}

static void PrintHelp()
{
  Print("# Help");
  Print("%s - A fast prime number generator", NAME);
  Print("> fast_primes_ll [OPTION]... [PARAM]...");
  Print("> fast_primes_ll -nthPrime 10001");
  Print("");

  Print("## Commands");
  Print("-help          This help");
  Print("-version       Show version");
  Print("-bench         Run benchmarks");
  Print("-hash          Show hash information");
  Print("-isPrime  [N]  See if N is a prime");
  Print("-nthPrime [N]  Show Nth prime");
  Print("-primes   [N]  Show all primes up to N");
  Print("");

  Print("## Full source code, please see:");
  Print("  <https://github.com/SamuraiDangyo/fast_primes_ll/>");

  exit(EXIT_SUCCESS);
}

static void CommandIsPrime()
{
  const int num        = TokenNextInt();
  const BITBOARD start = Now();

  if (IsPrime(num))
    Print("Result: Yes, %i is a prime number", num);
  else
    Print("Result: No, %i is not a prime number", num);

  Print("Time:   %.3fs", 0.001 * DOUBLE(Now() - start));
}

static BITBOARD NthPrime(const int prime_n)
{
  const int nth = Max(0, prime_n - 1);

  if (prime_n <= 0)
    return 2;

  if (nth < PRIMES.primes_n)
    return PRIMES.primes[nth];

  while (PRIMES.primes_n <= nth)
    AddPrime(NextPrime());

  return PRIMES.primes[nth];
}

static void CommandNthPrime()
{
  const int nth        = Max(0, TokenNextInt());
  const BITBOARD start = Now();

  Print("#:      %i", nth);
  Print("Prime:  %llu", NthPrime(nth));
  Print("Time:   %.3fs", 0.001 * DOUBLE(Now() - start));
}

static void CommandPrimes()
{
  const int num = Max(2, TokenNextInt());

  Print("# First %i primes generated by %s v%s", num, NAME, VERSION);
  PrintPrimes(num);
  InsertPrimes(num);
}

static void AddPrime(const BITBOARD prime)
{
  if (PRIMES.primes_n >= PRIMES.count)
    PrimestableMakeBigger();

  PRIMES.primes[PRIMES.primes_n] = prime;
  PRIMES.primes_n++;
}

static bool IsPrimeNogen(const BITBOARD prime)
{
  int i;

  if (prime < 2)
    return 1;

  for (i = 0; i < PRIMES.primes_n && PRIMES.primes[i] * PRIMES.primes[i] <= prime; i++)
    if (prime % PRIMES.primes[i] == 0)
      return 0;

  return 1;
}

static bool IsPrime(const BITBOARD maybe_prime)
{
  int i;
  BITBOARD prime;

  if (maybe_prime <= 1)
    return 0;

  for (i = 0; i < PRIMES.primes_n; i++)
    if (PRIMES.primes[i] == maybe_prime)
      return 1;

  while (1) {
    prime = NextPrime();

    AddPrime(prime);

    if (prime == maybe_prime)
      return 1;

    if (prime > maybe_prime)
      return 0;
  }

  return 0;
}

static int LastPrime()
{
  return PRIMES.primes[PRIMES.primes_n - 1];
}

static int NextPrime()
{
  int candidate = LastPrime() + 1;

  while (1)
    if (IsPrimeNogen(candidate))
      return candidate;
    else
      candidate++;

  MYASSERT((0));
}

static void InsertPrimes(const int how_many)
{
  while (PRIMES.primes_n < how_many) {
    AddPrime(NextPrime());
    Print("%llu", LastPrime());
  }
}

static void InitPrimes()
{
  int i;

  PRIMES.primes_n = 0;

  AddPrime(2);
  AddPrime(3);

  for (i = 0; i < 10; i++)
    AddPrime(NextPrime());
}

static void Bench()
{
  BITBOARD diff;
  const BITBOARD start = Now();

  PRIMES.primes_n = 0;

  AddPrime(2);
  AddPrime(3);

  while (PRIMES.primes_n < 1000000) {
    AddPrime(NextPrime());

    if (PRIMES.primes_n % 100000 == 0)
      Print("Progress ... %i%%", PRIMES.primes_n / 10000);
  }

  diff = Now() - start;

  Print("");
  Print("## Benchmarks");
  Print("Time:             %.3fs", 0.001 * DOUBLE(diff));
  Print("PrimesPerSecond:  %llu",  PrimesPerSecond(PRIMES.primes_n, diff));
  Print("Primes:           %i", PRIMES.primes_n);
}

static void PrintPrimes(const int usize)
{
  int i;
  const int how_many = Min(PRIMES.primes_n, usize);

  for (i = 0; i < how_many; i++)
    Print("%llu", PRIMES.primes[i]);
}

static void Commands()
{
  TokenExpect(";");

  if (TokenNext("version"))       PrintVersion();
  else if (TokenNext("help"))     PrintHelp();
  else if (TokenNext("isPrime"))  CommandIsPrime();
  else if (TokenNext("nthPrime")) CommandNthPrime();
  else if (TokenNext("bench"))    Bench();
  else if (TokenNext("hash"))     HashInfo();
  else if (TokenNext("primes"))   CommandPrimes();

  TokenExpect(";");

  if (TOKENS_N <= 2)
    PrintHelp();
}

static void InitTokens(int argc, char **argv)
{
  int i;

  TokenReset();
  TokenAdd(";");

  for (i = 1 /* skip ./fast_primes_ll */; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1)
      TokenAdd(argv[i] + 1);
    else
      TokenAdd(argv[i]);
  }

  TokenAdd(";");
}

static void Yeah()
{
  MYASSERT((IsPrime(37)));
  MYASSERT((IsPrime(53)));
  MYASSERT((IsPrime(42 + 1)));

  MYASSERT((NthPrime(-1) == 2));
  MYASSERT((NthPrime(0)  == 2));
  MYASSERT((NthPrime(1)  == 2));
  MYASSERT((NthPrime(2)  == 3));
  MYASSERT((NthPrime(17) == 59));
  MYASSERT((NthPrime(34) == 139));
}

static void Nope()
{
  MYASSERT(( ! IsPrime(42)));
  MYASSERT(( ! IsPrime(55)));
  MYASSERT(( ! IsPrime(0)));

  MYASSERT((NthPrime(1) != 7));
}

static void Go()
{
  PrimestableSetSize(HASH_SIZE);
  InitPrimes();
  Yeah();
  Nope();
  Commands();
}

int main(int argc, char **argv)
{
  atexit(PrimestableFreeMemory); // No memory leaks
  InitTokens(argc, argv);
  Go();

  return EXIT_SUCCESS;
}
