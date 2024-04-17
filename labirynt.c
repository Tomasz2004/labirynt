#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wyswietlacz.h"
#include "dane_labiryntu.h"
#include "oznaczenie.h"


int n = 0;   // Liczba wierszy
int m = 0;   // Liczba kolumn
int pw = -1;  // Numer wiersza P
int pk = -1;  // Numer kolumny P
int kw = -1;  // Numer wiersza K
int kk = -1;  // Numer kolumny K



int main(int argc, char* argv[]) {
    const char* filename = (argc >= 2) ? argv[1] : "maze.txt";
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Nie można odczytać pliku z labiryntem\n");
        return 2;
    }
    FILE* lab = fopen("lab.txt", "w+");
    if (!lab) {
        printf("Nie można utworzyć pliku\n");
        return 3;
    }

    int* wymiary = rozmiar(file, lab);
    n = wymiary[0];
    m = wymiary[1];
    free(wymiary);
    fclose(lab);

    lab = fopen("lab.txt", "r");
    printf("Labirynt: \n");
    
    czytaj(lab);
    rewind(lab);
    
    int* pozycjaP = ZnajdzP(lab, m);
    pw = pozycjaP[0];
    pk = pozycjaP[1];
    free(pozycjaP);

    rewind(lab);
    int* pozycjaK = ZnajdzK(lab, m);
    kw = pozycjaK[0];
    kk = pozycjaK[1];
    free(pozycjaK);
    
    fclose(lab);

    if (pw == -1 || pk == -1 || kw == -1 || kk == -1) {
        printf("Brak P lub K\n");
        return 4;
    }
    lab = fopen("lab.txt", "r+");
    oznacz(lab, pw, pk + 1, n, m);
    fclose(lab);
    lab = fopen("lab.txt", "r+");
    PoprawneOznaczenie(lab, kw, kk, m);
    fclose(lab);
    lab = fopen("lab.txt", "r");
    if (!lab) {
        printf("Nie można otworzyć pliku do odczytu\n");
        return 5;
    }
    printf("Oznaczony labirynt: \n");
    czytaj(lab);
    fclose(lab);
    lab = fopen("lab.txt", "r");
    printf("START\n");
    Instrukcja(lab, pw, pk + 1, m);
    printf("STOP\n");
    fclose(lab);
    fclose(file);
    return 0;
}


