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

#include "fast_primes_ll.h"

#define NAME    "fast_primes_ll"
#define VERSION "1.21"
#define AUTHOR  "Toni Helminen"

#define HASH_SIZE 64 /* MB / Starting size */

typedef struct {
  int *primes;
  BITBOARD size;
  int primes_n;
  int count;
  int last_size;
} HASHTABLE_T;

static char TOKENS[MAX_TOKENS][256] = {{0}};
static int TOKENS_N = 0;
static int TOKENS_I = 0;
static HASHTABLE_T HASH = {0};

static inline /* <- make me faster! */ int Max(const int a, const int b)
{
  return a > b ? a : b;
}

static inline int Min(const int a, const int b)
{
  return a < b ? a : b;
}

static bool Equal_strings(const char *a, const char *b)
{
  return strcmp(a, b) ? 0 : 1;
}

static BITBOARD Now()
{
  static struct timeval t;
  FCP_ASSERT(gettimeofday(&t, NULL) == 0)
  return 1000 * t.tv_sec + t.tv_usec / 1000;
}

static BITBOARD Nps(const BITBOARD nodes, const BITBOARD ms)
{
  return ms < 1 ? 0 : (1000 * nodes) / ms;
}

static void P(const char *format, ...)
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
  FCP_ASSERT(TOKENS_N + 2 < MAX_TOKENS)
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

static bool Token_is(const char *s)
{
  return Token_ok() && Equal_strings(s, Token_current());
}

static void Token_expect(const char *s)
{
  if ( ! Token_is(s)) {
    P("{ Error: Unexpected Token: '%s' }", Token_current());
    exit(0);
  }
  Token_pop();
}

static bool Token_next(const char *s)
{
  if ( ! Token_is(s))
    return 0;
  Token_pop();
  return 1;
}

static int Token_next_int()
{
  int r = 0;
  if (Token_ok() && ! Token_is(";")) { // Assume number
    r = atoi(TOKENS[TOKENS_I]);
    Token_pop();
  }
  return r;
}

static void Hashtable_free_memory()
{
  if (HASH.size > 0) {
    free(HASH.primes);
    HASH.primes = 0;
    HASH.size = 0;
  }
}

static void Free_memory()
{
  Hashtable_free_memory();
}

static void Hashtable_set_size(const int usize /* MB */)
{
  BITBOARD size = ULL(usize);
  Hashtable_free_memory();
  size = MAX(size, 16);
  size = MIN(size, 1024 * 1024); // 1 PB
  size = (1 << 20) * size; // To MB
  HASH.size = 1;
  while (HASH.size <= size) // Calculate needed memory in bytes
    HASH.size <<= 1;
  HASH.size >>= 1;
  HASH.primes_n = 0;
  HASH.count = INT(HASH.size / sizeof(BITBOARD));
  HASH.primes = (int*) calloc(HASH.count, sizeof(int)); // <- Cast for g++
  FCP_ASSERT(HASH.primes != NULL) // Make sure there is enough space
  Init_primes();
}

static void Hashtable_make_bigger()
{
  HASH.count = 2 * HASH.count;
  HASH.primes = (int*) realloc(&HASH, HASH.count * sizeof(int)); // <- Cast for g++
}

static void System()
{
  P("{ # Hash");
  P("  HASH.primes_n = %i,", HASH.primes_n);
  P("  HASH.size     = %i # MB,", (HASH.count * (sizeof(BITBOARD))) / (1024 * 1024));
  P("  HASH.count    = %i,", HASH.count);
  P("}");
}

static void Print_version()
{
  P("{ # Version");
  P("  name    = %s,", NAME);
  P("  version = %s,", VERSION);
  P("  author  = %s,", AUTHOR);
  P("  description = fast_primes_ll, A fast prime number generator in C Language");
  P("}");
}

static void Print_help()
{
  Print_version();
  P("");
  P("{ # How to use");
  P("  fast_primes_ll [COMMAND]... [OPTION]...,");
  P("  fast_primes_ll -is 882 # is it a prime?");
  P("}");
  P("");
  P("{ # Commands");
  P("  -h(elp)       = This help,");
  P("  -v(ersion)    = Version,");
  P("  -bench        = Benchmark fast_primes_ll,");
  P("  -system       = System info,");
  P("  -is(prime) N  = Check if whether N is a prime,");
  P("  -nth(prime) N = Print Nth prime,");
  P("  -list N       = Print all primes up to N");
  P("}");
  P("");
  P("{ # Full source code, please see:");
  P("  <https://github.com/SamuraiDangyo/fast_primes_ll/>");
  P("}");
  exit(0);
}

static void Command_isprime()
{
  int n = Token_next_int();
  if (User_is_prime(n))
    P("{ Yes, %i is a prime number. }", n);
  else
    P("{ No, %i is not a prime number. }", n);
}

static int Nth_prime(const int prime_n)
{
  int n = Max(0, prime_n);
  if (n < HASH.primes_n)
    return HASH.primes[n];
  while (1) {
    Add_prime(Next_prime());
    if (HASH.primes_n == n)
      return Last_prime();
  }
}

static void Command_nthprime()
{
  int n = Token_next_int();
  P("{ # Results nthprime");
  P("  N        = %i,", n);
  P("  nthprime = %i", Nth_prime(n - 1));
  P("}");
}

static void Command_list()
{
  int n = Max(10, Token_next_int());
  Insert_primes(n);
  Print_primes(n);
}

static void Add_prime(const int prime)
{
  if (HASH.primes_n >= HASH.count)
    Hashtable_make_bigger();
  HASH.primes[HASH.primes_n] = prime;
  HASH.primes_n++;
}

static int Last_prime()
{
  return HASH.primes[HASH.primes_n - 1];
}

static bool Is_prime(const int prime)
{
  int i;
  if (prime < 2)
    return 1;
  for (i = 0; i < HASH.primes_n; i++)
    if (prime % HASH.primes[i] == 0)
      return 0;
  return 1;
}

static bool User_is_prime(const int prime)
{

  int i, p;
  for (i = 0; i < HASH.primes_n; i++)
    if (HASH.primes[i] == prime) return 1;
  while (1) {
    p = Next_prime();
    Add_prime(p);
    if (p == prime)
      return 1;
    if (p > prime)
      return 0;
  }
  return 0;
}

static int Next_prime()
{
  int candidate = Last_prime() + 1;
  while (1) {
    if (Is_prime(candidate))
      return candidate;
    else
      candidate++;
  }
  FCP_ASSERT(0)
}

static void Insert_primes(const int usize)
{
  while (HASH.primes_n < usize) Add_prime(Next_prime());
}

static void Init_primes()
{
  int i;
  HASH.primes_n = 0;
  Add_prime(2);
  Add_prime(3);
  for (i = 0; i < 10; i++)
    Add_prime(Next_prime());
}

static void Bench()
{
  int i;
  P("> Benching ...\n");
  BITBOARD diff;
  BITBOARD start = Now();
  HASH.primes_n = 0; // Just for benching
  Add_prime(2);
  Add_prime(3);
  for (i = 0; i < 100000; i++) {
    if ((i + 1) % 10000 == 0)
      P("{ done = %i%% }", (i + 1) / 1000);
    Add_prime(Next_prime());
  }
  diff = Now() - start;
  P("");
  P("{ # Benchmarks");
  P("  time              = %.3fs,", 0.001f * DOUBLE(diff));
  P("  primes_per_second = %llu", Nps(HASH.primes_n, diff));
  P("}");
}

static void Print_primes(const int usize)
{
  int i;
  int n = Min(HASH.primes_n, usize);
  P("{ # List first %i primes", usize);
  for (i = 0; i < n; i++)
    P("  %i = %llu%s", i + 1, HASH.primes[i], i + 1 < n ? ",": "");
  P("}");
}

static void FCP_commands()
{
  Token_expect(";");
  while (Token_ok()) {
    if (Token_next("v") || Token_next("version"))
      Print_version();
    else if (Token_next("h") || Token_next("help"))
      Print_help();
    else if (Token_next("isprime") || Token_next("is"))
      Command_isprime();
    else if (Token_next("bench"))
      Bench();
    else if (Token_next("system"))
      System();
    else if (Token_next("list"))
      Command_list();
    else if (Token_next("nthprime") || Token_next("nth"))
      Command_nthprime();
    Token_expect(";");
  }
  if (TOKENS_N < 2)
    Print_help();
}

static void Init_tokens(int argc, char **argv)
{
  int i;
  Token_reset();
  for (i = 1 /* skip ./fast_primes_ll */; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
      Token_add(";");
      Token_add(argv[i] + 1);
    } else {
      Token_add(argv[i]);
    }
  }
  Token_add(";");
}

static void Ok()
{
  FCP_ASSERT(User_is_prime(37));
  FCP_ASSERT(User_is_prime(53));
  FCP_ASSERT(Nth_prime(17) == 59);
  FCP_ASSERT(Nth_prime(34) == 139);
}

static void Go()
{
  Hashtable_set_size(HASH_SIZE);
  Init_primes();
  Ok();
  FCP_commands();
}

int main(int argc, char **argv)
{
  atexit(Free_memory); // No memory leaks
  Init_tokens(argc, argv);
  Go();
  return EXIT_SUCCESS;
}
