#ifndef FAST_PRIMES_LL_H
#define FAST_PRIMES_LL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdbool.h>

#define BITBOARD unsigned long long
#define MAX_TOKENS 128

#define INT(a) ((int) (a))
#define DOUBLE(f) ((double) (f))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define ULL(a) ((BITBOARD) (a))
#define FCP_ASSERT(test) if ( ! (test)) {P("fast_primes_ll Error: Line: %i", __LINE__); exit(0);}

static inline /* <- make me faster! */ int Max(const int a, const int b);
static inline int Min(const int a, const int b);
static bool Equal_strings(const char *a, const char *b);
static BITBOARD Now();
static BITBOARD Nps(const BITBOARD nodes, const BITBOARD ms);
static void P(const char *format, ...);
static void Ok();
static void Go();
static void Token_add(const char *s);
static void Token_reset();
static const char *Token_current();
static void Token_pop();
static bool Token_ok();
static bool Token_is(const char *s);
static void Token_expect(const char *s);
static bool Token_next(const char *s);
static int Token_next_int();
static void Hashtable_free_memory();
static void Free_memory();
static void Hashtable_set_size(const int usize /* MB */);
static void Hashtable_make_bigger();
static void System();
static void Print_version();
static void Print_help();
static void Command_isprime();
static void Command_list();
static void Add_prime(const int prime);
static int Last_prime();
static bool Is_prime(const int prime);
static bool User_is_prime(const int prime);
static int Next_prime();
static void Insert_primes(const int usize);
static void Init_primes();
static void Bench();
static int Nth_prime(const int prime_n);
static void Command_nthprime();
static void Print_primes(const int usize);
static void FCP_commands();
static void Init_tokens(int argc, char **argv);

// !! i love you grandpa !!

#endif // FAST_PRIMES_LL_H