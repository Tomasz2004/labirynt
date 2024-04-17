#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "oznaczenie.h"


bool oznacz(FILE* file, int w, int k, int n, int m) {
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesuni?cie kursora na odpowiedni? pozycj? w pliku
    char c = fgetc(file); // Odczyt znaku z pozycji (w, k)
    if (c == 'X') {
        return false;
    }
    if (c == 'P') {
        fseek(file, -1, SEEK_CUR); // Przesuni?cie kursora o 1 wstecz, aby zapisa? znak
        fputc('P', file); // Zapis 'P' na pozycji (w, k)
        return true;
    }
    if (c == 'K') {
        fseek(file, -1, SEEK_CUR); // Przesuni?cie kursora o 1 wstecz, aby zapisa? znak
        fputc('K', file); // Zapis 'K' na pozycji (w, k)
        return true;
    }
    if (c == '1') {
        return false;
    }
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesuni?cie kursora o 1 wstecz, aby zapisa? znak
    fputc('1', file); // Zapis '1' na pozycji (w, k)
    // Próba poruszenia si? na pole powy?ej
    if (w > 0) {
        if (oznacz(file, w - 1, k, n, m))
            return true;
        fseek(file, w * (m + 2) + k, SEEK_SET); // Przesuni?cie kursora na dó?
    }

    // Próba poruszenia si? na pole poni?ej
    if (w < n - 1) {
        if (oznacz(file, w + 1, k, n, m))
            return true;
        fseek(file, w * (m + 2) + k, SEEK_SET); // Przesuni?cie kursora na gór?
    }

    // Próba poruszenia si? na pole po prawej
    if (k < m - 1 && oznacz(file, w, k + 1, n, m))
        return true;

    // Próba poruszenia si? na pole po lewej
    if (k > 0 && oznacz(file, w, k - 1, n, m))
        return true;
    fseek(file, w * (m + 2) + k, SEEK_SET);



    // Je?li ?adna ?cie?ka nie prowadzi do wyj?cia, cofamy si?
    fseek(file, w * (m + 2) + k, SEEK_SET); // Przesuni?cie kursora na odpowiedni? pozycj? w pliku
    fputc(' ', file); // Przywrócenie oryginalnego znaku na pozycji (w, k)
    return false;
}



void PoprawneOznaczenie(FILE* file, int w, int k, int m) {
    fseek(file, w * (m + 2) + k, SEEK_SET);
    char c = fgetc(file);
    int p, g, d, l;
    while (c != 'P') {
        p = 0;
        g = 0;
        d = 0;
        l = 0;

        //w prawo
        fseek(file, w * (m + 2) + k + 1, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('l', file);
            p = 1;
        }

        //w dó?
        fseek(file, (w + 1) * (m + 2) + k, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('g', file);
            g = 1;
        }

        //w gór?
        fseek(file, (w - 1) * (m + 2) + k, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('d', file);
            d = 1;

        }

        //w lewo
        fseek(file, w * (m + 2) + k - 1, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('p', file);
            l = 1;
        }
        if (p == 1) {
            k = k + 1;
        }
        if (g == 1) {
            w = w + 1;
        }
        if (d == 1) {
            w = w - 1;
        }
        if (l == 1) {
            k = k - 1;
        }
    }
}