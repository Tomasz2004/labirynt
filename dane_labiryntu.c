#include <stdio.h>
#include <stdlib.h>

#include "dane_labiryntu.h"



 int* rozmiar(FILE* filename, FILE* tym) {
    if (!filename) {
        printf("Nie mo¿na otworzyæ pliku.\n");
        exit(1);
    }
    int znak;
    char s[2051];
    int i = 0; // Licznik wierszy
    int j = 0; // Licznik kolumn
    int* rozmiar = malloc(2 * sizeof(int));
    while ((znak = fgetc(filename)) == 'X' && znak != EOF && znak != '\n') {
        j++;
    }

    rewind(filename); // Przewijamy do pocz¹tku pliku

    while (fgets(s, sizeof(s), filename)) {
        fputs(s, tym);
        i++; // Inkrementujemy licznik wierszy
    }

    rozmiar[0] = i;
    rozmiar[1] = j;
    return rozmiar;
}


int* ZnajdzP(FILE* file, int m) {
    int w = 0;
    char s[2051];
    int k;
    while (fgets(s, sizeof(s), file)) {
        for (k = 0; k < m; k++) {
            if (s[k] == 'P') {
                int* pozycja = malloc(2 * sizeof(int));
                pozycja[0] = w;
                pozycja[1] = k;
                return pozycja;
            }
        }
        w++;
    }
    return NULL;
}


int* ZnajdzK(FILE* file, int m) {
    int w = 0;
    char s[2051];
    int k;
    while (fgets(s, sizeof(s), file)) {
        for (k = 0; k < m; k++) {
            if (s[k] == 'K') {
                int* pozycja = malloc(2 * sizeof(int));
                pozycja[0] = w;
                pozycja[1] = k;
                return pozycja;
            }
        }
        w++;
    }
    return NULL;
}

