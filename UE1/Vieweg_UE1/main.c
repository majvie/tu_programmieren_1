#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("********************************\n"
           "*        Programmieren 1       *\n"
           "*                              *\n"
           "*      Maximilian, Vieweg      *\n"
           "*           11806443           *\n"
           "*          12-05-1999          *\n"
           "********************************\n"
          );

    /* Definition von Konstanten */
    const double PI = 3.14159;

    /* Definition der Variablen */
    double hauptachse;
    double nebenachse;
    double flaeche;
    double flaeche_inkreis;
    double flaeche_umkreis;

    /* Wertezuweisungen */
    hauptachse = 3.9;
    nebenachse = 1.4;

    /* Berechnungen */
    flaeche = hauptachse * nebenachse * PI / 4.0;
    flaeche_inkreis = nebenachse * nebenachse * PI;
    flaeche_umkreis = hauptachse * hauptachse * PI;

    /* Ausgabe */
    printf("Ellipse:\n");
    printf("Laenge der %s = %f\n", "Hauptachse", hauptachse);
    printf("Laenge der %s = %f\n", "Nebenachse", nebenachse);
    printf("Flaeche = %lf\n", flaeche);
    printf("Flaeche Inkreis = %lf\n", flaeche_inkreis);
    printf("Flaeche Umkreis = %lf\n", flaeche_umkreis);

}
