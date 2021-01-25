#include <stdio.h>
#include <string.h>
#include "dbfunc.h"

void initItems(Data_t *items, long len) {
  Data_t newElement;
  newElement.name[0] = '\0';
  newElement.brand[0] = '\0';
  newElement.invNr[0] = '\0';
  newElement.year = 0;
  for (int i = 0; i < len; i++) {
    items[i] = newElement;
  }
}

void printItem(Data_t *item) {
  printf("Name:\t\t\t%-10s\n", item->name);
  printf("Marke:\t\t\t%-10s\n", item->brand);
  printf("Inventar-Nr.:\t\t%-10s\n", item->invNr);
  printf("Anschaffungsjahr:\t%-4ld\n", item->year);
}

void printDB(Data_t *items, long len) {
  printf("Datenbankeintraege: \n");
  for (int i = 0; i < len; i++) {
    printf("-------------Eintrag %d -------------\n", i+1);
    printItem(items+i);
  }
  printf("\n");
}

long readValue(char *name) {
  long number;
  char text_b[TEXT_LEN];

  printf("%s: ", name);
  if (fgets(text_b, sizeof(text_b), stdin) != NULL) {
    if (sscanf(text_b, "%ld", &number) == 0) {
      fprintf(stderr, "Der Input konnte nicht gelesen werden.\n");
      number = readValue(name);
    }
  }
  return number;
}

void readString(char *name, char *text, long len) {
  char text_b[TEXT_LEN];

  while (1) {
    printf("%s: ", name);
    if (fgets(text_b, sizeof(text_b), stdin) != NULL) {
      if (sscanf(text_b, "%s", text) == 0) {
        fprintf(stderr, "Der Input konnte nicht gelesen werden.\n");
        continue;
      }
      // Buffer Overflow == no newLine written
      else if (!strchr(text_b, '\n')) {
        fprintf(stderr, "Der Input war zu lange.\n");
        while ((fgetc(stdin)) != '\n'); // Flush rest of input buffer
        continue;
      }
    }
    break;
  }
}

long addItem(Data_t *items, long len) {
  Data_t newItem;
  for (int i = 0; i < len; i++) {
    if (items[i].invNr[0] == '\0') {
      printf("\n----------Neuen Eintrag eingeben----------\n");
      readString("Name", newItem.name, TEXT_LEN);
      readString("Marke", newItem.brand, TEXT_LEN);
      readString("Inventar-Nr.", newItem.invNr, TEXT_LEN);
      newItem.year = readValue("Anschaffungsjahr");
      items[i] = newItem;

      printf("Der Eintrag wurde hinzugefuegt.\n");
      return 0;
    }
  }
  printf("Der neue Eintrag konnte nicht hinzugefuegt werden.\n");
  return -1;
}

long deleteItem(Data_t *items, long len) {
  long i_delete = readValue("Index des zu loeschenden Eintrags") - 1;
  printf("\n");

  if (items[i_delete].invNr[0] == '\0' || i_delete > len) {
    fprintf(stderr, "Der Index oder Eintrag ist ungueltig.\n");
    return -1;
  }
  else {
    // Swapping elements
    for (int i = i_delete; i < len-1; i++)
      items[i] = items[i + 1];

    items[len-1].name[0] = '\0';
    items[len-1].brand[0] = '\0';
    items[len-1].invNr[0] = '\0';
    items[len-1].year = 0;

    printf("Der Eintrag konnte entfernt werden.\n");
    return 0;
  }
}

long saveDB(Data_t *items, long len, char *outputFile) {
  FILE* fp = fopen(outputFile, "w");
  if (fp == NULL) {
    fprintf(stderr, "Die Datei konnte nicht geoeffnet werden.\n");
    fclose(fp);
    return -1;
  }

  fprintf(fp, "#Name\t#Marke\t#Inventar-Nr.\t#Jahr\n");
  for (int i = 0; i < len; i++) {
    if (items[i].invNr[0] == '\0') break; //Last element reached
    fprintf(fp, "%s %s %s %ld\n", items[i].name,
                                  items[i].brand,
                                  items[i].invNr,
                                  items[i].year);
  }
  printf("Die Datenbank wurde unter %s abgespeichert.\n", outputFile);
  fclose(fp);
  return 0;
}

long loadDB(Data_t *items, long len, char *inputFile) {
  FILE* fp = fopen(inputFile, "r");
  char line[TEXT_LEN];
  Data_t newItem;

  if (fp == NULL) {
    fprintf(stderr, "Die Datei konnte nicht geoeffnet werden.\n");
    fclose(fp);
    return -1;
  }

  // Discard first header line
  fgets(line, TEXT_LEN, fp);

  // parse subsequent lines
  for (int i = 0; i < len; i++) {
    if (fgets(line, TEXT_LEN, fp) != NULL) {
      if(sscanf(line, "%s %s %s %ld", newItem.name,
                                      newItem.brand,
                                      newItem.invNr,
                                      &newItem.year) == 0) {
        fprintf(stderr, "Die Datei konnte nicht gelesen werden.\n");
        initItems(items, len);
        return -1;
      }

      items[i] = newItem;
    }
  }
  printf("Die Eintraege konnten erfolgreich aus der Datei %s gelesen werden.\n", inputFile);
  fclose(fp);
  return 0;
}

long searchFile(char *inputFile, char *key, long ignoreCase) {
  FILE* fp = fopen(inputFile, "r");
  char line[TEXT_LEN];
  char* charPosition;

  if (fp == NULL) {
    fprintf(stderr, "Die Datei konnte nicht geoeffnet werden.\n");
    fclose(fp);
    return -1;
  }

  // Read first header line
  fgets(line, TEXT_LEN, fp);

  // parse subsequent lines
  int lineIndex = 1;
  while (fgets(line, TEXT_LEN, fp) != NULL) {
    if (ignoreCase == 0) charPosition = strcasestr(line, key);
    else charPosition = strstr(line, key);

    if(charPosition != NULL)
      printf("%s Zeile: %d, %s\n", inputFile, lineIndex, line);

    lineIndex++;
  }
  fclose(fp);
  return 0;
}

static long _compareInvNr(Data_t a, Data_t b) {
  // nr1 < nr2: -1
  // nr1 > nr2: 1
  // nr1 == nr2: 0
  for (int i = 0; a.invNr[i] != '\0'; i++) {
    if (a.invNr[i] == b.invNr[i]) continue;
    else if (a.invNr[i] < b.invNr[i]) return -1;
    else return 1;
  }
  return 0;
}

static long _compareYear(Data_t a, Data_t b) {
  if (a.year < b.year) return -1;
  else if (a.year > b.year) return 1;
  return 0;
}

static void _selectionSort(Data_t* items, long len, long (*comparisonFunc)(Data_t a, Data_t b)) {
  // len: Length until empty element
  Data_t* smallest, temp;
  for (int i = 0; i < len-1; i++) {
    smallest = &items[i];
    for (int j = i+1; j < len; j++)
      if (comparisonFunc(items[j], *smallest) == -1 && items[j].invNr[0] != '\0')
        smallest = &items[j];
    // Swap
    temp = items[i];
    items[i] = *smallest;
    *smallest = temp;
  }
}

long sortItems(Data_t *items, long len, long which) {
  int itemsNo = 0;
  while (items[itemsNo].invNr[0] != '\0' && itemsNo < len) itemsNo++;
  switch(which) {
    // inventar Nr
    case 1: {
      _selectionSort(items, itemsNo, _compareInvNr);
      printf("Die Daten wurden nach der Inventarnummer sortiert.\n");
      break;
    }
    // Anschaffungsjahr
    case 2: {
      _selectionSort(items, itemsNo, _compareYear);
      printf("Die Daten wurden nach dem Anschaffungsjahr sortiert.\n");
      break;
    }
    default:
      fprintf(stderr, "Sie haben eine falsche Option gewaehlt.\n");
      return -1;
  }
  return 0;
}

static long _appendToDB(Data_t *items, Data_t newItem, long len) {
  // Append to the end of items
  for (int i = 0; i < len; i++) {
    // Check if Inv Nr already exists
    if (strcmp(newItem.invNr, items[i].invNr) == 0) {
      fprintf(stderr, "Die Inventarnummer existiert schon.\n");
      return -1;
    }
    // Check if field empty
    if (items[i].invNr[0] == '\0') {
      items[i] = newItem;
      return 0;
    }
  }
  // Nothing added yet
  fprintf(stderr, "Die Datenbank ist voll. Der Eintrag konnte nicht hinzugefuegt werden.\n");
  return -1;
}

long addFromFile(Data_t *items, long len, char *inputFile, char *key) {
  FILE* fp = fopen(inputFile, "r");
  Data_t newItem;
  char line[TEXT_LEN];
  char* charPosition;

  if (fp == NULL) {
    fprintf(stderr, "Die Datei konnte nicht geoeffnet werden.\n");
    return -1;
  }

  // Discard first header line
  fgets(line, TEXT_LEN, fp);

  // parse subsequent data lines
  while (fgets(line, TEXT_LEN, fp) != NULL) {
    charPosition = strstr(line, key);

    // If found key
    if (charPosition != NULL) {
        if (sscanf(line, "%s %s %s %ld", newItem.name,
                                   newItem.brand,
                                   newItem.invNr,
                                   &newItem.year) == 0) {
        fprintf(stderr, "Die Datei konnte nicht gelesen werden.\n");
        return -1;
      }

      if (_appendToDB(items, newItem, len) == -1)
        return -1;

      printf("%s %s wurde beigefuegt.\n", newItem.name, newItem.brand);
    }
  }
  fclose(fp);
  return 0;
}
