#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n;   // Liczba wierszy
int m;   // Liczba kolumn

void rozmiar(FILE* filename, FILE *tym) {
    if (!filename) {
        printf("Nie można otworzyć pliku.\n");
        exit(1);
    }

    int znak;
    char s[2048];
    int i = 0; // Licznik wierszy
    int j = 0; // Licznik kolumn
    while ((znak = fgetc(filename)) == 'X' && znak != EOF) {
        j++;
    }

    rewind(filename); // Przewijamy do początku pliku

    while (fgets(s, sizeof(s), filename)) {
        fputs(s, tym);
        i++; // Inkrementujemy licznik wierszy
    }

    m = j; // Liczba kolumn w L
    n = i; // Liczba wierszy w L
    printf("Liczba kolumn %d i liczba wierszy %d\n", m, n);
}
void czytaj(FILE *file) {
    if (!file) {
        printf("Nie można otworzyć pliku.\n");
        exit(1);
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
}

int main() {
    const char* filename = "smallmaze.txt";
    FILE* file = fopen(filename, "r");
    FILE *lab = fopen("lab.txt", "w");
    if (!lab) {
        printf("Nie można utworzyć pliku\n");
        return 3;
    }

    rozmiar(file, lab);
    fclose(lab);
    lab = fopen("lab.txt", "r");
    if (!lab) {
        printf("Nie można otworzyć pliku do odczytu\n");
        return 4;
    }
    printf("Labirynt: \n");
    czytaj(lab);
    fclose(lab);
    fclose(file);
    return 0;
}

