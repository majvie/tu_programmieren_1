#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 100 //max line length

void my_getLine(char input[], long len);
long get_letters(char text[]);
long get_others(char text[]);
void analyze_text(char text[]);
char* get_letters_ptr(char text[]);
char* get_others_ptr(char text[]);
void analyze_text_ptr(char text[]);
void trim_text(char text[]);

int main()
{
    while(1) {
      char textBuffer[N];
      printf("Texteingabe: ");
      my_getLine(textBuffer, N);

      long alphaNo = get_letters(textBuffer);
      long notAlphaNo = get_others(textBuffer);

      // Nur *eine* Anzeige, je nach was am Anfang steht
      if (alphaNo) {
        printf("Buchstaben am Anfang: %ld", alphaNo);
      } else {
        printf("Andere Zeichen am Anfang: %ld", notAlphaNo);
      }
      analyze_text_ptr(textBuffer);
      trim_text(textBuffer);

      char answer[10];
      printf("\nNochmal (j|n)? ");
      fgets(answer, sizeof(answer), stdin);
      printf("\n");
      if (*answer == 'n') break;
      else if (*answer == 'j') continue;
      else {
        printf("Es tut mir Leid, ich habe das nicht verstanden. \n");
        break;
      }
    }
    printf("\n------PROGRAMM-ENDE-------\n");
    return 0;
}

void my_getLine(char input[], long len) {
    char inChar;
    for (int i = 0; i < len-1; i++) {
        inChar = getchar();

        if (inChar == '\n') {
          input[i] = '\0';
          return;
        }
        input[i] = inChar;
    }
    input[len-1] = '\0';

    // Empty buffer
    while (getchar() != '\n') continue;
}

char* get_letters_ptr(char text[]) {
  char *letter = &text[0];
  while (isalpha(*letter) && *letter != '\0') letter++;
  return letter;
}

long get_letters(char text[]) {
  long textLen = 0;
  char letter;
  for (int i = 0; letter = text[i], letter != '\0'; i++) {
    if (isalpha(letter)) textLen++;
    else break;
  }
  return textLen;
}

long get_others(char text[]) {
  long notAlphaLen = 0;
  char letter;
  for (int i = 0; letter = text[i], letter != '\0'; i++) {
    if (!isalpha(letter)) notAlphaLen++;
    else break;
  }
  return notAlphaLen;
}

char* get_others_ptr(char text[]) {
  char *symbol = &text[0];
  while (!isalpha(*symbol) && *symbol != '\0') symbol++;
  return symbol;
}

void analyze_text(char text[]) {
    long snippets[11] = {0}; // laenge -> haeufigkeit
    long wordCount = 0, snippetLen = 0;
    int i = 0;

    while (text[i] != '\0') {
      long snippetLen = get_letters(&text[i]);

      if (snippetLen) {
        wordCount += 1;

        if (snippetLen > 9)
          snippets[10] += 1;
        snippets[snippetLen] += 1;

        i += snippetLen;
      }
      else {
        i += get_others(&text[i]);
      }
    }

    printf("\nWortzahl: %ld", wordCount);
    printf("\nStatistik: ");
    printf("\n-----------------------------");
    printf("\nLaenge\t | \tHaeufigkeit\n");
    for (int i = 1; i < 10; i++) {
      printf("  %d\t | \t%ld", i, snippets[i]);
    }
    printf(">=10\t | \t%ld\n", snippets[10]);
    printf("\n-----------------------------");

}

void analyze_text_ptr(char text[]) {
  long snippets[11] = {0}; // laenge -> haeufigkeit
  long wordCount = 0, wordLen = 0;
  char *currentField = text, *nextField;

  while (*currentField != '\0') {
    nextField = get_letters_ptr(currentField);

    if (currentField != nextField) {
      wordCount += 1;
      wordLen = nextField-currentField;

      if (wordLen > 9)
        snippets[10] += 1;
      snippets[wordLen] += 1;

      currentField = nextField;
    }
    else {
      nextField = get_others_ptr(currentField);
      currentField = nextField;
    }
  }

  printf("\nWortzahl: %ld", wordCount);
  printf("\nStatistik: ");
  printf("\n-----------------------------");
  printf("\nLaenge\t | \tHaeufigkeit\n");
  for (int i = 1; i < 10; i++) {
    printf("  %d\t | \t%ld\n", i, snippets[i]);
  }
  printf(">=10\t | \t%ld\n", snippets[10]);
  printf("\n-----------------------------");
}

void trim_text(char text[]) {
  char* textPointer = &text[0];
  char swap_b[N];
  char* nextLetter_ptr;

  while (*textPointer != '\0') {
    if (isalpha(*textPointer))
      textPointer = get_letters_ptr(textPointer);
    else {
      *textPointer = ' ';
      nextLetter_ptr = get_others_ptr(textPointer);

      strncpy(swap_b, nextLetter_ptr, (text + N) - nextLetter_ptr+1);
      strncpy(textPointer + 1, swap_b, (text + N) - nextLetter_ptr+1);

      textPointer = get_others_ptr(textPointer); //nextLetter_ptr not valid anymore
    }
  }

  printf("\nNur Woerter: %s", text);
}
