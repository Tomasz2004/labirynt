#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n;   // Liczba wierszy
int m;   // Liczba kolumn

void rozmiar(const char* filename, FILE *tym) {
        FILE* file = fopen(filename, "r");
        if (!file) {
                printf("Nie można otworzyć pliku.\n");
                exit(1);
        }
        char znak;
        char s[2048];
        int i = 0, j = 0; // Liczniki wierszy i kolumn

        while ((znak = fgetc(file)) != '\n') {
                j++; // Zliczanie kolumn w pierwszym wierszu
        }

        rewind(file); // Przewijamy do pocz¹tku pliku3
        while (fgets(s, sizeof(s), file)) {
            fputs(s,tym);
            i++; // Inkrementujemy licznik wierszy
        }
        m = j;  // Liczba kolumn w L
        n = i;  // Liczba wierszy w L
        printf("Liczba kolumn %d i liczba wierszy %d\n",m,n);

        fclose(file);
        fclose(tym);
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
    fclose(file);
}

int main() {
    const char* filename = "maze.txt";
    FILE *lab = fopen("lab.txt", "w");
    if (!lab) {
        printf("Nie można utworzyć pliku\n");
        return 3;
    }
    
    rozmiar(filename, lab);
    lab = fopen("lab.txt", "r");
    rewind(lab);
    if (!lab) {
        printf("Nie można otworzyć pliku do odczytu\n");
        return 4;
    }
    printf("Labirynt: \n");
    czytaj(lab);
    fclose(lab);
    return 0;
}

