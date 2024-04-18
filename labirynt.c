#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>

#include "wyswietlacz.h"
#include "dane_labiryntu.h"
#include "oznaczenie.h"


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
        FILE *pom = fopen("pomocniczy.txt","w");

        if (!file){
            fprintf(stderr,"Nie można odczytać pliku wejściowego z labiryntem\n");
            exit(0);
        }
        uint8_t separator;
        uint8_t value;
        uint8_t count;
        struct LaBin b;
        fread(&b.fileId, sizeof(b.fileId),1,file);
        fread(&b.escape, sizeof(b.escape),1,file);
        fread(&b.columns, sizeof(b.columns),1,file);
        fread(&b.lines, sizeof(b.lines),1,file);
        fread(&b.entry_x, sizeof(b.entry_x),1,file);
        fread(&b.entry_y, sizeof(b.entry_y),1,file);
        fread(&b.exit_x, sizeof(b.exit_x),1,file);
        fread(&b.exit_y, sizeof(b.exit_y),1,file);
        fread(&b.reserved, sizeof(b.reserved),1,file);
        fread(&b.counter, sizeof(b.counter),1,file);
        fread(&b.solutionOffset, sizeof(b.solutionOffset),1,file);
        fread(&b.separator, sizeof(b.separator),1,file);
        fread(&b.wall, sizeof(b.wall),1,file);
        fread(&b.path, sizeof(b.path),1,file);

        pk=b.entry_x-1;
        pw=b.entry_y-1;
        kk=b.exit_x-1;
        kw=b.exit_y-1;
        n=b.lines;
        m=b.columns;

        for (int i=0; i<=b.columns*b.lines;i+=count+1){
            fread(&separator,sizeof(uint8_t),1,file);
            fread(&value,sizeof(uint8_t),1,file);
            fread(&count,sizeof(uint8_t),1,file);

            if (value==b.path){
                for (int j=0;j<=count;j++){
                    fputc(' ', pom);
                }
            }
            else if (value==b.wall){
                for (int k=0;k<=count;k++){
                    fputc('X',pom);
                }
            }
            else {
                break;
            }

        }

        fclose(file);
        fclose(pom);
        pom=fopen("pomocniczy.txt","r");
        if (!pom){
            fprintf(stderr, "Nie udało się otworzyć pliku z zdekodowanym labiryntem\n");
            exit(0);
        }


        lab=fopen("lab.txt","w");
        char c;
        int x,y;
        for (y=0; y<b.lines;y++){
            for (x=0; x<b.columns;x++){
                c=getc(pom);
                if (x==b.entry_x-1 && y==b.entry_y-1){
                    fprintf(lab,"P");
                }
                else if (x==b.exit_x-1 && y==b.exit_y-1){
                    fprintf(lab,"K");
                }
                else {
                    fprintf(lab,"%c",c);
                }
            }
            fprintf(lab,"\n");
        }
        fclose(pom);
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

    lab = fopen("lab.txt", "r");
    printf("Labirynt: \n");
    czytaj(lab);

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


