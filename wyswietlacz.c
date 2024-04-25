#include <stdio.h>
#include <stdlib.h>


#include "wyswietlacz.h"


void czytaj(FILE* file) {
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


void Instrukcja(FILE* file, int w, int k, int m) {
    char c = 0;
    int kroki = 1;
    int p = 1;
    int l = 0;
    int g = 0;
    int d = 0;
    while (c != 'K') {
        fseek(file, w * (m + 1) + k, SEEK_SET);
        c = fgetc(file);
        switch (c) {
        case 'p':
            if (p == 1) {
                kroki += 1;
            }
            else {
                if (g == 1) {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNRIGHT\n");
                    g = 0;
                    p = 1;
                    kroki = 1;
                }
                else {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNLEFT\n");
                    d = 0;
                    p = 1;
                    kroki = 1;
                }
            }
            k = k + 1;
            break;
        case 'l':
            if (l == 1) {
                kroki += 1;
            }
            else {
                if (g == 1) {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNLEFT\n");
                    g = 0;
                    l = 1;
                    kroki = 1;
                }
                else {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNRIGHT\n");
                    d = 0;
                    l = 1;
                    kroki = 1;
                }
            }
            k = k - 1;
            break;
        case 'g':
            if (g == 1) {
                kroki += 1;
            }
            else {
                if (p == 1) {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNLEFT\n");
                    p = 0;
                    g = 1;
                    kroki = 1;
                }
                else {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNRIGHT\n");
                    l = 0;
                    g = 1;
                    kroki = 1;
                }
            }
            w = w - 1;
            break;
        case 'd':
            if (d == 1) {
                kroki += 1;
            }
            else {
                if (p == 1) {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNRIGHT\n");
                    p = 0;
                    d = 1;
                    kroki = 1;
                }
                else {
                    printf("FORWARD %d\n", kroki);
                    printf("TURNLEFT\n");
                    l = 0;
                    d = 1;
                    kroki = 1;
                }
            }
            w = w + 1;
            break;
        }
    }
    printf("FORWARD %d\n", kroki);
}
