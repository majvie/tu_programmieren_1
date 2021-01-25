#include <stdio.h>
#include <string.h>
#include "dbfunc/dbfunc.h"

#define ITEMS_LEN 10

int main(int argc, char* argv[]) {
  Data_t items[ITEMS_LEN];
  initItems(items, ITEMS_LEN);

  if (argc == 1) {
    while (1) {
      printf("\n1 - Alle Datensätze ausgeben\n"
             "2 - Eintrag hinzufügen\n"
             "3 - Eintrag löschen\n"
             "4 - Datei speichern\n"
             "5 - Datei laden\n"
             "6 - In Datei suchen\n"
             "7 - Datensätze sortieren\n"
             "8 - Eintraege aus Datei hinzufuegen\n"
             "0 - Programm beenden\n\n");
      long inputValue = readValue("Auswahl");
      printf("\n");

      char key[TEXT_LEN], filename[TEXT_LEN];
      switch(inputValue) {
        case 0: return 0;
        case 1: printDB(items, ITEMS_LEN); break;
        case 2: addItem(items, ITEMS_LEN); break;
        case 3: deleteItem(items, ITEMS_LEN); break;
        case 4: {
          readString("Datenbank Dateiname", filename, TEXT_LEN);
          saveDB(items, ITEMS_LEN, filename);
          break;
        }
        case 5: {
          readString("Datenbank Dateiname", filename, TEXT_LEN);
          loadDB(items, ITEMS_LEN, filename);
          break;
        }
        case 6: {
          readString("Datenbank Dateiname", filename, TEXT_LEN);
          readString("Keyword", key, TEXT_LEN);
          searchFile(filename, key, 1);
          break;
        }
        case 7: {
          long which = readValue("1 - Inventar-Nr.\n"
                                 "2 - Anschaffungsjahr\n");
          sortItems(items, ITEMS_LEN, which);
          break;
        }
        case 8: {
          readString("Datenbank Dateiname", filename, TEXT_LEN);
          readString("Keyword", key, TEXT_LEN);

          addFromFile(items, ITEMS_LEN, filename, key);
          break;
        }
        default: printf("Die Eingabe war ungueltig. Versuchen Sie es noch einmal.\n\n");
      }
    }
  }
  // Command Line
  else {
    char* key;
    if (strcmp(argv[1], "-i") == 0) {
      key = argv[2];
      for (int i = 3; i < argc; i++)
        searchFile(argv[i], key, 0);
    }
    else {
      key = argv[1];
      for (int i = 2; i < argc; i++)
        searchFile(argv[i], key, 1);
    }
  }
}
