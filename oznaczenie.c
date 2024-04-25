#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "oznaczenie.h"

#define SCIANA 'X'


int x = 0;
int y = 0;


void przekaz(int a, int b) {
    x = a;
    y = b;
}


bool oznacz(FILE* file, int w, int k) {
    fseek(file, w * (y + 1) + k, SEEK_SET); // Przesuni?cie kursora na odpowiedni? pozycj? w pliku
    char c = fgetc(file); // Odczyt znaku z pozycji (w, k)
    if (c == SCIANA) {
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
    fseek(file, w * (y + 1) + k, SEEK_SET); // Przesuni?cie kursora o 1 wstecz, aby zapisa? znak
    fputc('1', file); // Zapis '1' na pozycji (w, k)
    // Pr�ba poruszenia si? na pole powy?ej
    if (w > 0) {
        if (oznacz(file, w - 1, k))
            return true;
        fseek(file, w * (y + 1) + k, SEEK_SET); // Przesuni?cie kursora na d�?
    }

    // Pr�ba poruszenia si? na pole poni?ej
    if (w < x - 1) {
        if (oznacz(file, w + 1, k))
            return true;
        fseek(file, w * (y + 1) + k, SEEK_SET); // Przesuni?cie kursora na g�r?
    }

    // Pr�ba poruszenia si? na pole po prawej
    if (k < y - 1 && oznacz(file, w, k + 1))
        return true;

    // Pr�ba poruszenia si? na pole po lewej
    if (k > 0 && oznacz(file, w, k - 1))
        return true;
    fseek(file, w * (y + 1) + k, SEEK_SET);



    // Je?li ?adna ?cie?ka nie prowadzi do wyj?cia, cofamy si?
    fseek(file, w * (y + 1) + k, SEEK_SET); // Przesuni?cie kursora na odpowiedni? pozycj? w pliku
    fputc(' ', file); // Przywr�cenie oryginalnego znaku na pozycji (w, k)
    return false;
}



void PoprawneOznaczenie(FILE* file, int w, int k) {
    fseek(file, w * (y + 1) + k, SEEK_SET);
    char c = fgetc(file);
    int p, g, d, l;
    while (c != 'P') {
        p = 0;
        g = 0;
        d = 0;
        l = 0;

        //w prawo
        fseek(file, w * (y + 1) + k + 1, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('l', file);
            p = 1;
        }

        //w d�?
        fseek(file, (w + 1) * (y + 1) + k, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('g', file);
            g = 1;
        }

        //w g�r?
        fseek(file, (w - 1) * (y + 1) + k, SEEK_SET);
        c = fgetc(file);
        if (c == '1') {
            fseek(file, -1, SEEK_CUR);
            fputc('d', file);
            d = 1;

        }

        //w lewo
        fseek(file, w * (y + 1) + k - 1, SEEK_SET);
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
