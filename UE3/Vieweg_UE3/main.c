#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long readNumber();
long isPrime(long num);
long isPrimeFast(long num);
long *buildPrimeList(long upperBound);
void primeFactors(long);
void primeFactors_helper(long num, long factor, long *prime_p);
void expandNumber(long num);

static long* primeList;
static bool isFirstRun = true;
static int primeCalcBound = 100;

int main() {
  bool isNumberPrime = false;

  while(true) {
    long numberInput = readNumber();
    if (numberInput == -1) break;

    isNumberPrime = isPrimeFast(numberInput);
    isFirstRun = false;
    if (isNumberPrime) {
      printf("%ld ist prim.\n", numberInput);
    }
    else {
      printf("%ld ist nicht prim.\n", numberInput);

      if (numberInput != 1) {
        printf("Primfaktorzerlegung:\t");
        primeFactors(numberInput);
      }
    }
    if (numberInput > 2 && numberInput % 2 == 0) {
      printf("Goldbach-Zerlegung:\t");
      expandNumber(numberInput);
    }
    printf("-----------------------\n");

    char answer[10];
    printf("Nochmal (y|n)? ");
    fgets(answer, sizeof(answer), stdin);
    printf("\n");
    if (*answer == 'n') break;
    else if (*answer == 'y') continue;
    else {
      printf("Es tut mir Leid, ich habe das nicht verstanden. \n");
      break;
    }
  }
  printf("-----PROGRAMM-ENDE-----");
  return 0;
}

long readNumber() {
  // Reads in number from the keyboard
  double rawInput;
  char line[30];
  printf("Bitte geben Sie eine ganze positive Zahl ein: ");
  if (fgets(line, sizeof(line), stdin)) {
    if (sscanf(line, "%lf", &rawInput) == 1) {
      if (rawInput > 0 && (int) rawInput == rawInput) return (long) rawInput;
    }
  }
  printf("Falsche Eingabe: Das war keine ganze positive Zahl.\n");
  return -1;
}

long isPrime(long num) {
  if (num < 1) return -1;
  if (num == 1) return 0;
  for (int i = 2; i < num/2+1; i++) {
    if (num % i == 0) return 0;
  }
  return 1;
}

long isPrimeFast(long num) {
  // Urspruenglich primeList als Argument, aber aufgrund Angabe mit statischen Variablen
  if (isFirstRun) primeList = buildPrimeList(primeCalcBound);
  if (num < 101) return primeList[num] == 1;
  return isPrime(num);
}

long *buildPrimeList(long upperBound) {
  // key: Prime -> value: Bool isPrimeOrNot
  // malloc damit Werte im heap sind; sonst: stack mit return garbage
  long *primeNumbers = malloc(sizeof(long)*upperBound);
  for (int i = 0; i < upperBound; i++) {
    primeNumbers[i] = isPrime(i);
  }
  return primeNumbers;
}

void primeFactors(long num) {
  long primes[50];

  primeFactors_helper(num, 2, &primes[0]);

  printf("%ld =", num);
  for (int i = 0; i < 30; i++) {
    if (primes[i+1] == -1) {
      printf(" %ld \n", primes[i]);
      break;
    }
    else  printf(" %ld *", primes[i]);
  }
}

void primeFactors_helper(long num, long factor, long *prime_p) {
  if (factor > num) {
    *prime_p = -1; return; //Sentinel value to terminate array
  }
  else if (num % factor == 0) {
    *prime_p = factor;
    primeFactors_helper(num/factor, factor, prime_p+1);
  }
  else primeFactors_helper(num, factor + 1, prime_p);
}

void expandNumber(long num) {
  for (long i = 0; i < num; i++) {
    if (isPrime(i) && isPrime(num-i)) {
      printf("%ld = %ld + %ld\n", num, i, num-i);
      return;
    }
  }
}
