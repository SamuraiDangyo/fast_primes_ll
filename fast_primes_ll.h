#ifndef FAST_PRIMES_LL_GUARD_H
#define FAST_PRIMES_LL_GUARD_H

static int Max(const int, const int);
static int Min(const int, const int);
static bool EqualStrings(const char *, const char *);
static BITBOARD Now();
static BITBOARD PrimesPerSecond(const BITBOARD, const BITBOARD);
static void Print(const char *, ...);
static void Yeah();
static void Nope();
static void Go();
static void TokenAdd(const char *);
static void TokenReset();
static const char *TokenCurrent();
static void TokenPop();
static bool TokenOk();
static bool TokenIs(const char *);
static void TokenExpect(const char *);
static bool TokenNext(const char *);
static int TokenNextInt();
static void PrimestableFreeMemory();
static void PrimestableSetSize(const int);
static void PrimestableMakeBigger();
static void HashInfo();
static void PrintVersion();
static void PrintHelp();
static void CommandIsPrime();
static void CommandPrimes();
static void Assert(const bool, const int);
static void AddPrime(const BITBOARD);
static int LastPrime();
static bool IsPrimeNogen(const BITBOARD);
static bool IsPrime(const BITBOARD);
static int NextPrime();
static void InsertPrimes(const int);
static void InitPrimes();
static void Bench();
static BITBOARD NthPrime(const int);
static void CommandNthPrime();
static void PrintPrimes(const int);
static void Commands();
static void InitTokens(int, char **);

// !! i love you grandpa !!

#endif // FAST_PRIMES_LL_GUARD_H
