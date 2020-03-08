#ifndef FAST_PRIMES_LL_H
#define FAST_PRIMES_LL_H

static int Max(const int, const int);
static int Min(const int, const int);
static bool Equal_strings(const char *, const char *);
static BITBOARD Now();
static BITBOARD Pps(const BITBOARD, const BITBOARD);
static void Print(const char *, ...);
static void Ok();
static void Go();
static void Token_add(const char *);
static void Token_reset();
static const char *Token_current();
static void Token_pop();
static bool Token_ok();
static bool Token_is(const char *);
static void Token_expect(const char *);
static bool Token_next(const char *);
static int Token_next_int();
static void Primestable_free_memory();
static void Primestable_set_size(const int);
static void Primestable_make_bigger();
static void System();
static void Print_version();
static void Print_help();
static void Command_isprime();
static void Command_primes();
static void Add_prime(const BITBOARD);
static int Last_prime();
static bool Is_prime_nogen(const BITBOARD);
static bool Is_prime(const BITBOARD);
static int Next_prime();
static void Insert_primes(const int);
static void Init_primes();
static void Bench();
static BITBOARD Nth_prime(const int);
static void Command_nthprime();
static void Print_primes(const int);
static void Commands();
static void Init_tokens(int, char **);

// !! i love you grandpa !!

#endif // FAST_PRIMES_LL_H
