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

#define NAME       "fast_primes_ll"
#define VERSION    "1.34"
#define AUTHOR     "Toni Helminen"

#define BITBOARD   unsigned long long

#define MAX_TOKENS 32
#define HASH_SIZE  64 /* MB | Starting size */

///
/// Macros
///

#define INT(a)           ((int) (a))
#define DOUBLE(f)        ((double) (f))
#define ULL(a)           ((BITBOARD) (a))
#define MAX(a, b)        (((a) > (b)) ? (a) : (b))
#define MIN(a, b)        (((a) < (b)) ? (a) : (b))
#define MYASSERT(test)   if ( ! (test)) {Print("fast_primes_ll Error: Line: %i", __LINE__); exit(EXIT_FAILURE);}

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
static PRIMES_T PRIMES             = {0};

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

static bool Equal_strings(const char *str1, const char *str2)
{
  return strcmp(str1, str2) ? 0 : 1;
}

static BITBOARD Now()
{
  static struct timeval tv;
  MYASSERT(gettimeofday(&tv, NULL) == 0)
  return 1000 * tv.tv_sec + tv.tv_usec / 1000;
}

static BITBOARD Pps(const BITBOARD nodes, const BITBOARD ms)
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

static void Token_add(const char *s)
{
  MYASSERT(TOKENS_N + 2 < MAX_TOKENS)

  strcpy(TOKENS[TOKENS_N], s);
  TOKENS_N++;
}

static void Token_reset()
{
  TOKENS_I = 0;
  TOKENS_N = 0;
}

static const char *Token_current()
{
  return Token_ok() ? TOKENS[TOKENS_I] : "";
}

static void Token_pop()
{
  TOKENS_I++;
}

static bool Token_ok()
{
  return TOKENS_I < TOKENS_N;
}

static bool Token_is(const char *str)
{
  return Token_ok() && Equal_strings(str, Token_current());
}

static void Token_expect(const char *str)
{
  if ( ! Token_is(str)) {
    Print("{ # Errors");
    Print("  expected  : '%s'", str);
    Print("  but_found : '%s'", Token_current());
    Print("}");

    exit(EXIT_SUCCESS);
  }

  Token_pop();
}

static bool Token_next(const char *str)
{
  if ( ! Token_is(str))
    return 0;

  Token_pop();

  return 1;
}

static int Token_next_int()
{
  int num = 0;

  if (Token_ok() && ! Token_is(";")) { // Assume number
    num = atoi(TOKENS[TOKENS_I]);
    Token_pop();
  }

  return num;
}

static void Primestable_free_memory()
{
  if (PRIMES.size <= 0)
    return;

  free(PRIMES.primes);

  PRIMES.primes = 0;
  PRIMES.size   = 0;
}

static void Primestable_set_size(const int usize /* MB */)
{
  BITBOARD size = ULL(usize);

  Primestable_free_memory();

  size = MAX(size, 1);
  size = MIN(size, 1024 * 1024); // 1 PB
  size = (1 << 20) * size;       // To MB

  PRIMES.size = 1;

  while (PRIMES.size <= size) // Calculate needed memory in bytes
    PRIMES.size <<= 1;

  PRIMES.size     >>= 1;
  PRIMES.primes_n = 0;
  PRIMES.count    = INT(PRIMES.size / sizeof(BITBOARD));
  PRIMES.primes   = calloc(PRIMES.count, sizeof(BITBOARD));

  MYASSERT(PRIMES.primes != NULL) // Make sure there is enough space

  Init_primes();
}

static void Primestable_make_bigger()
{
  PRIMES.count  = 2 * PRIMES.count;
  PRIMES.primes = realloc(PRIMES.primes, PRIMES.count * sizeof(BITBOARD));

  MYASSERT(PRIMES.primes != NULL) // Make sure there is enough space
}

static void System()
{
  Print("{ # Hash");
  Print("  .primes_n : %i", PRIMES.primes_n);
  Print("  .count    : %i", PRIMES.count);
  Print("  .size     : %i # MB", (PRIMES.count * (sizeof(BITBOARD))) / (1024 * 1024));
  Print("}");
}

static void Print_version()
{
  Print("{ # Version");
  Print("  name         : %s", NAME);
  Print("  version      : %s", VERSION);
  Print("  author       : %s", AUTHOR);
  Print("  description  : A fast prime number generator in C Language");
  Print("}");
}

static void Print_help()
{
  Print("{ # Help %s v%s ", NAME, VERSION);
  Print("  usage         : > fast_primes_ll [OPTION]... [PARAM]...");
  Print("}\n");

  Print("{ # General Information");
  Print("  what          : fast_primes_ll is a fast prime number generator");
  Print("  usage         : fast_primes_ll [COMMAND]... [OPTION]...");
  Print("  example       : > fast_primes_ll -nthprime 10001 # What's the 10,001st prime?");
  Print("}\n");

  Print("{ # Commands");
  Print("  -help         : This help");
  Print("  -version      : Show version");
  Print("  -bench        : Run benchmarks");
  Print("  -system       : Show system info");
  Print("  -isprime [N]  : See if N is a prime");
  Print("  -nthprime [N] : Show Nth prime");
  Print("  -primes [N]   : Show all primes up to N");
  Print("}\n");

  Print("{ # Full source code, please see:");
  Print("  <https://github.com/SamuraiDangyo/fast_primes_ll/>");
  Print("}");

  exit(EXIT_SUCCESS);
}

static void Command_isprime()
{
  const int num        = Token_next_int();
  const BITBOARD start = Now();

  Print("{ # isprime");

  if (Is_prime(num))
    Print("  result  : Yes, %i is a prime number.", num);
  else
    Print("  result  : No, %i is not a prime number.", num);

  Print("  time    : %.3fs", 0.001f * DOUBLE(Now() - start));
  Print("}");
}

static BITBOARD Nth_prime(const int prime_n)
{
  const int n = Max(0, prime_n - 1);

  if (prime_n <= 0)
    return 2;

  if (n < PRIMES.primes_n)
    return PRIMES.primes[n];

  while (PRIMES.primes_n <= n)
    Add_prime(Next_prime());

  return PRIMES.primes[n];
}

static void Command_nthprime()
{
  const int n          = Max(0, Token_next_int());
  const BITBOARD start = Now();

  Print("{ # nthprime");
  Print("  nth      : %i", n);
  Print("  prime    : %llu", Nth_prime(n));
  Print("  time     : %.3fs", 0.001f * DOUBLE(Now() - start));
  Print("}");
}

static void Command_primes()
{
  const int num = Max(2, Token_next_int());

  Print("{ # First %i primes generated by %s v%s", num, NAME, VERSION);
  Print_primes(num);
  Insert_primes(num);
  Print("}");
}

static void Add_prime(const BITBOARD prime)
{
  if (PRIMES.primes_n >= PRIMES.count)
    Primestable_make_bigger();

  PRIMES.primes[PRIMES.primes_n] = prime;
  PRIMES.primes_n++;
}

static bool Is_prime_nogen(const BITBOARD prime)
{
  int i;

  if (prime < 2)
    return 1;

  for (i = 0; i < PRIMES.primes_n && PRIMES.primes[i] * PRIMES.primes[i] <= prime; i++)
    if (prime % PRIMES.primes[i] == 0)
      return 0;

  return 1;
}

static bool Is_prime(const BITBOARD maybe_prime)
{

  int i;
  BITBOARD prime;

  if (maybe_prime <= 1)
    return 0;

  for (i = 0; i < PRIMES.primes_n; i++)
    if (PRIMES.primes[i] == maybe_prime)
      return 1;

  while (1) {
    prime = Next_prime();

    Add_prime(prime);

    if (prime == maybe_prime)
      return 1;

    if (prime > maybe_prime)
      return 0;
  }

  return 0;
}

static int Last_prime()
{
  return PRIMES.primes[PRIMES.primes_n - 1];
}

static int Next_prime()
{
  int candidate = Last_prime() + 1;

  while (1)
    if (Is_prime_nogen(candidate))
      return candidate;
    else
      candidate++;

  MYASSERT(0)
}

static void Insert_primes(const int how_many)
{
  int counter        = 0;
  const int primes_n = PRIMES.primes_n;

  while (PRIMES.primes_n < how_many) {
    Add_prime(Next_prime());
    Print("  %i: %llu", counter + 1 + primes_n, Last_prime());
    counter++;
  }
}

static void Init_primes()
{
  int i;

  PRIMES.primes_n = 0;

  Add_prime(2);
  Add_prime(3);

  for (i = 0; i < 10; i++)
    Add_prime(Next_prime());
}

static void Bench()
{
  BITBOARD diff;
  const BITBOARD start = Now();

  Print("{ # Benching ... }\n");

  PRIMES.primes_n = 0;

  Add_prime(2);
  Add_prime(3);

  while (PRIMES.primes_n < 1000000) {
    Add_prime(Next_prime());

    if (PRIMES.primes_n % 100000 == 0)
      Print("{ Progress : %i%% }", PRIMES.primes_n / 10000);
  }

  diff = Now() - start;

  Print("\n{ # Benchmarks");
  Print("  time              : %.3fs", 0.001f * DOUBLE(diff));
  Print("  primes_per_second : %llu",  Pps(PRIMES.primes_n, diff));
  Print("  primes            : %i",    PRIMES.primes_n);
  Print("}");
}

static void Print_primes(const int usize)
{
  int i;
  const int how_many = Min(PRIMES.primes_n, usize);

  for (i = 0; i < how_many; i++)
    Print("  %i: %llu", i + 1, PRIMES.primes[i]);
}

static void Commands()
{
  Token_expect(";");

  if (Token_next("version"))       Print_version();
  else if (Token_next("help"))     Print_help();
  else if (Token_next("isprime"))  Command_isprime();
  else if (Token_next("bench"))    Bench();
  else if (Token_next("system"))   System();
  else if (Token_next("primes"))   Command_primes();
  else if (Token_next("nthprime")) Command_nthprime();

  Token_expect(";");

  if (TOKENS_N <= 2)
    Print_help();
}

static void Init_tokens(int argc, char **argv)
{
  int i;

  Token_reset();
  Token_add(";");

  for (i = 1 /* skip ./fast_primes_ll */; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1)
      Token_add(argv[i] + 1);
    else
      Token_add(argv[i]);
  }

  Token_add(";");
}

static void Ok()
{
  // Yes

  MYASSERT(Is_prime(37));
  MYASSERT(Is_prime(53));

  MYASSERT(Nth_prime(-1) == 2);
  MYASSERT(Nth_prime(0)  == 2);
  MYASSERT(Nth_prime(1)  == 2);
  MYASSERT(Nth_prime(2)  == 3);
  MYASSERT(Nth_prime(17) == 59);
  MYASSERT(Nth_prime(34) == 139);

  // Nope

  MYASSERT( ! Is_prime(55));
  MYASSERT( ! Is_prime(0));

  MYASSERT(Nth_prime(1) != 7);
}

static void Go()
{
  Primestable_set_size(HASH_SIZE);
  Init_primes();
  Ok();
  Commands();
}

int main(int argc, char **argv)
{
  atexit(Primestable_free_memory); // No memory leaks
  Init_tokens(argc, argv);
  Go();

  return EXIT_SUCCESS;
}
