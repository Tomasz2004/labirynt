#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int n;   // Liczba wierszy
int m;   // Liczba kolumn

void rozmiar(FILE* filename, FILE *tym) {
    if (!filename) {
        printf("Nie mo¿na otworzyæ pliku.\n");
        exit(1);
    }

    int znak;
    char s[2048];
    int i = 0; // Licznik wierszy
    int j = 0; // Licznik kolumn
    while ((znak = fgetc(filename)) == 'X' && znak != EOF && znak!='\n') {
        j++;
    }

    rewind(filename); // Przewijamy do pocz¹tku pliku

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
        printf("Nie mo¿na otworzyæ pliku.\n");
        exit(1);
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        printf("%c", c);
    }
    printf("\n");
}

bool oznacz(FILE *file, int w, int k) {
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesunięcie kursora na odpowiednią pozycję w pliku
    char c = fgetc(file); // Odczyt znaku z pozycji (w, k)
    if (c == 'X') {
        return false;
    }
    if (c == 'P') {
        fseek(file, -1, SEEK_CUR); // Przesunięcie kursora o 1 wstecz, aby zapisać znak
        fputc('P', file); // Zapis 'P' na pozycji (w, k)
        return true;
    }
    if (c == 'K') {
        fseek(file, -1, SEEK_CUR); // Przesunięcie kursora o 1 wstecz, aby zapisać znak
        fputc('K', file); // Zapis 'K' na pozycji (w, k)
        return true;
    }
    if (c == '1') {
        return false;
    }
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesunięcie kursora o 1 wstecz, aby zapisać znak
    fputc('1', file); // Zapis '1' na pozycji (w, k)
    // Próba poruszenia się na pole powyżej
    if (w > 0) {
        if (oznacz(file, w - 1, k))
            return true;
        fseek(file, w * (m + 2) + k, SEEK_SET); // Przesunięcie kursora na dół
    }

    // Próba poruszenia się na pole poniżej
    if (w < n - 1) {
        if (oznacz(file, w + 1, k))
            return true;
        fseek(file, w * (m + 2) + k, SEEK_SET); // Przesunięcie kursora na górę
    }

    // Próba poruszenia się na pole po lewej
    if (k > 0 && oznacz(file, w, k - 1))
        return true;
    fseek(file, w * (m + 2) + k, SEEK_SET);

    // Próba poruszenia się na pole po prawej
    if (k < m - 1 && oznacz(file, w, k + 1))
        return true;

    // Jeśli żadna ścieżka nie prowadzi do wyjścia, cofamy się
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesunięcie kursora na odpowiednią pozycję w pliku
    fputc('X', file); // Przywrócenie oryginalnego znaku na pozycji (w, k)
    return false;
}



int main() {
    const char* filename = "smallmaze.txt";
    FILE* file = fopen(filename, "r");
    FILE *lab = fopen("lab.txt", "w+");
    if (!lab) {
        printf("Nie można utworzyć pliku\n");
        return 3;
    }

    rozmiar(file, lab);
    oznacz(lab, 1, 1); // Rozpoczęcie rekurencyjnej eksploracji labiryntu
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


