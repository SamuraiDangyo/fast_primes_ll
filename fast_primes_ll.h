#ifndef FAST_PRIMES_LL_GUARD_H
#define FAST_PRIMES_LL_GUARD_H

static int Max(const int, const int);
static int Min(const int, const int);
static bool EqualStrings(const char *, const char *);
static BITBOARD Now(void);
static BITBOARD PrimesPerSecond(const BITBOARD, const BITBOARD);
static void Print(const char *, ...);
static void Yeah(void);
static void Nope(void);
static void Go(void);
static void TokenAdd(const char *);
static void TokenReset(void);
static const char *TokenCurrent(void);
static void TokenPop(void);
static bool TokenOk(void);
static bool TokenIs(const char *);
static void TokenExpect(const char *);
static bool TokenNext(const char *);
static int TokenNextInt(void);
static void PrimestableFreeMemory(void);
static void PrimestableSetSize(const int);
static void PrimestableMakeBigger(void);
static void HashInfo(void);
static void PrintVersion(void);
static void PrintHelp(void);
static void CommandIsPrime(void);
static void CommandPrimes(void);
static void Assert(const bool, const int);
static void AddPrime(const BITBOARD);
static int LastPrime(void);
static bool IsPrimeNogen(const BITBOARD);
static bool IsPrime(const BITBOARD);
static inline int NextPrime(void);
static void InsertPrimes(const int);
static void InitPrimes(void);
static void Bench(void);
static BITBOARD NthPrime(const int);
static void CommandNthPrime(void);
static void PrintPrimes(const int);
static void Commands(void);
static void InitTokens(int, char **);

// !! i love you grandpa !!

#endif // FAST_PRIMES_LL_GUARD_H
