#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/resource.h>

#include "wyswietlacz.h"
#include "dane_labiryntu.h"
#include "oznaczenie.h"
#include "konwerter.h"


int n = 0;   // Liczba wierszy
int m = 0;   // Liczba kolumn
int pw = -1;  // Numer wiersza P
int pk = -1;  // Numer kolumny P
int kw = -1;  // Numer wiersza K
int kk = -1;  // Numer kolumny K

struct LaBin {
    uint32_t fileId;
    uint8_t escape;
    uint16_t columns;
    uint16_t lines;
    uint16_t entry_x;
    uint16_t entry_y;
    uint16_t exit_x;
    uint16_t exit_y;
    uint16_t reserved[6];
    uint32_t counter;
    uint32_t solutionOffset;
    uint8_t separator;
    uint8_t wall;
    uint8_t path;
};

int main(int argc, char* argv[]) {
    struct rlimit stack_limit;
    stack_limit.rlim_cur = RLIM_INFINITY;
    stack_limit.rlim_max = RLIM_INFINITY;
    if (setrlimit(RLIMIT_STACK, &stack_limit) != 0) {
        perror("Błąd ustawiania limitu stosu");
        exit(EXIT_FAILURE);
    }
    FILE *file;
    FILE *lab;
    int bflag=0;
    int tflag=0;
    int pflag=0;
    char *nazwa = NULL;
    int c;
    opterr=0;
    while ((c=getopt(argc,argv,"btp:"))!=-1){
        switch(c){
            case 'b':
                bflag=1;
                break;
            case 't':
                tflag=1;
                break;
            case 'p':
                pflag=1;
                nazwa=optarg;
                break;
            case '?':
                if(optopt=='p'){
                    fprintf(stderr, "-%c wymaga podania nazwy pliku wejściowego.\n",optopt);
                }
                else if (isprint(optopt)){
                    fprintf(stderr, "Nieznana flaga '-%c'.\n", optopt);
                }
                else{
                    fprintf(stderr, "Nieznana opcja '\\x%x'. \n",optopt);
                }
                return 1;
            default:
                abort();
        }
    }
    for (int index=optind;index<argc;index++){
        fprintf(stderr, "Podałeś niestandardowy argument: %s.\n", argv[index]);
        return 2;
    }
    if (pflag==0){
        nazwa="maze.txt";
    }
    if (bflag==1 && tflag==1){
        fprintf(stderr,"Argumentów -b i -t nie można używać naraz");
        return 3;
    }
    if (bflag==0 && tflag==0){
        tflag=1;
    }

    if (bflag==1){
        file = fopen(nazwa,"rb");
        lab = fopen("lab.txt", "w+");
        if (!file) {
            fprintf(stderr, "Nie można odczytać pliku wejściowego z labiryntem\n");
            exit(0);
        }
        Wymiary wymiary = przekonwertuj(file, lab);
        n = wymiary->n;
        m = wymiary->m;
        pw = wymiary->pw;
        pk = wymiary->pk;
        kw = wymiary->kw;
        kk = wymiary->kk;

        free(wymiary);
        fclose(lab);
    }
    if (tflag==1){
        file = fopen(nazwa, "r");
        if (!file) {
            printf("Nie można odczytać pliku z labiryntem\n");
            return 2;
        }
        lab = fopen("lab.txt", "w+");
        if (!lab) {
            printf("Nie można utworzyć pliku\n");
            return 3;
        }
        int* wymiary = rozmiar(file, lab);
        n = wymiary[0];
        m = wymiary[1];
        free(wymiary);
        fclose(lab);
        lab=fopen("lab.txt","r");
        int* pozycjaP = ZnajdzP(lab, m);
        pw = pozycjaP[0];
        pk = pozycjaP[1];
        rewind(lab);
        int* pozycjaK = ZnajdzK(lab, m);
        kw = pozycjaK[0];
        kk = pozycjaK[1];
        free(pozycjaK);
        free(pozycjaP);
        fclose(lab);
    }
    printf("Liczba kolumn %d i liczba wierszy %d\n", m, n);
    lab = fopen("lab.txt", "r");
    printf("Labirynt: \n");
    czytaj(lab);
    fclose(lab);
    if (pw == -1 || pk == -1 || kw == -1 || kk == -1) {
        printf("Brak P lub K\n");
        return 4;
    }
    lab = fopen("lab.txt", "r+");
    przekaz(n, m);
    oznacz(lab, pw, pk + 1);
    fclose(lab);
    lab = fopen("lab.txt", "r+");
    PoprawneOznaczenie(lab, kw, kk);
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


