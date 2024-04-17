#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>

int n;   // Liczba wierszy
int m;   // Liczba kolumn
int pw=-1;  // Numer wiersza P
int pk=-1;  // Numer kolumny P
int kw=-1;  // Numer wiersza K
int kk=-1;  // Numer kolumny K

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
    fputc(' ', file); // Przywrócenie oryginalnego znaku na pozycji (w, k)
    return false;
}


void ZnajdzP(FILE *file){
    int w=0;
    char s[2048];
    while (fgets(s, sizeof(s), file)) {
        for (int k=0; k<m; k++){
           if (s[k]=='P'){
                pw=w;
                pk=k;
                return;
           }
        }
        w++;
    }
}

void ZnajdzK(FILE *file){
    int w=0;
    char s[2048];
    while (fgets(s, sizeof(s), file)) {
        for (int k=0; k<m; k++){
           if (s[k]=='K'){
                kw=w;
                kk=k;
                return;
           }
        }
        w++;
    }
}

void PoprawneOznaczenie(FILE *file, int w, int k){
    fseek(file, w*(m+2)+k,SEEK_SET);
    char c=fgetc(file);
    int p, g, d, l;
    while (c!='P'){
        p=0;
        g=0;
        d=0;
        l=0;

        //w prawo
        fseek(file, w*(m+2)+k+1, SEEK_SET);
        c=fgetc(file);
        if (c=='1'){
            fseek(file, -1, SEEK_CUR);
            fputc('l',file);
            p=1;
        }

        //w dół
        fseek(file, (w+1)*(m+2)+k,SEEK_SET);
        c=fgetc(file);
        if (c=='1'){
            fseek(file, -1,SEEK_CUR);
            fputc('g',file);
            g=1;
        }

        //w górę
        fseek(file, (w-1)*(m+2)+k,SEEK_SET);
        c=fgetc(file);
        if (c=='1'){
            fseek(file, -1,SEEK_CUR);
            fputc('d',file);
            d=1;

        }

        //w lewo
        fseek(file, w*(m+2)+k-1, SEEK_SET);
        c=fgetc(file);
        if (c=='1'){
            fseek(file, -1, SEEK_CUR);
            fputc('p',file);
            l=1;
        }
        if (p==1){
            k=k+1;
        }
        if (g==1){
            w=w+1;
        }
        if (d==1){
            w=w-1;
        }
        if (l==1){
            k=k-1;
        }
    }
}


void Instrukcja(FILE *file, int w, int k){
    char c;
    int kroki=1;
    int p = 1;
    int l = 0;
    int g = 0;
    int d = 0;
    while (c!='K'){
        fseek(file, w*(m+2)+k,SEEK_SET);
        c=fgetc(file);
        switch(c){
            case 'p':
                if (p==1){
                    kroki+=1;
                } else{
                    if (g==1){
                        printf("FORWARD %d\n", kroki);
                        printf("TURNRIGHT\n");
                        g=0;
                        p=1;
                        kroki=1;
                    } else {
                        printf("FORWARD %d\n", kroki);
                        printf("TURNLEFT\n");
                        d=0;
                        p=1;
                        kroki=1;
                    }
                }
                k=k+1;
                break;
            case 'l':
                if (l==1){
                    kroki+=1;
                } else{
                    if (g==1){
                        printf("FORWARD %d\n", kroki);
                        printf("TURNLEFT\n");
                        g=0;
                        l=1;
                        kroki=1;
                    } else {
                        printf("FORWARD %d\n", kroki);
                        printf("TURNRIGHT\n");
                        d=0;
                        l=1;
                        kroki=1;
                    }
                }
                k=k-1;
                break;
            case 'g':
                if (g==1){
                    kroki+=1;
                } else{
                    if (p==1){
                        printf("FORWARD %d\n", kroki);
                        printf("TURNLEFT\n");
                        p=0;
                        g=1;
                        kroki=1;
                    } else {
                        printf("FORWARD %d\n", kroki);
                        printf("TURNRIGHT\n");
                        l=0;
                        g=1;
                        kroki=1;
                    }
                }
                w=w-1;
                break;
            case 'd':
                if (d==1){
                    kroki+=1;
                } else{
                    if (p==1){
                        printf("FORWARD %d\n", kroki);
                        printf("TURNRIGHT\n");
                        p=0;
                        d=1;
                        kroki=1;
                    } else {
                        printf("FORWARD %d\n", kroki);
                        printf("TURNLEFT\n");
                        l=0;
                        d=1;
                        kroki=1;
                    }
                }
                w=w+1;
                break;
        }
    }
    printf("FORWARD %d\n", kroki);
}


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
        if (!file){
            fprintf(stderr,"Nie można odczytać pliku wejściowego z labiryntem\n");
            exit(1);
        }
        lab = fopen("lab.txt", "w");

        rozmiar(file, lab);
        fclose(lab);
        fclose(file);
        lab = fopen("lab.txt", "r");
        if (!lab){
           fprintf(stderr, "Nie można odczytać pliku z labiryntem");
           exit(1);
        }
        ZnajdzP(lab);
        rewind(lab);
        ZnajdzK(lab);
        fclose(lab);
        if (pw==-1 || pk==-1 || kw==-1 || kk==-1){
            printf("Brak P lub K w pliku tekstowym\n");
            exit(1);
        }
    }
    lab = fopen("lab.txt", "r");
    printf("Labirynt: \n");
    czytaj(lab);
    fclose(lab);
    printf("\n>>>>>>>>>>>>P %d %d     K %d %d<<<<<<<<< wiersze %d kolumny %d\n" ,pk,pw,kk,kw,n,m);
    lab = fopen("lab.txt", "r+");
    oznacz(lab, pw, pk+1);
    fclose(lab);

    lab = fopen("lab.txt", "r+");
    PoprawneOznaczenie(lab,kw,kk);
    fclose(lab);

    lab = fopen("lab.txt", "r");
    if (!lab) {
        printf("Nie można otworzyć pliku z oznaczonym labiryntem\n");
        exit(1);
    }
    printf("Oznaczony labirynt: \n");
    czytaj(lab);
    fclose(lab);

    lab = fopen("lab.txt", "r");
    printf("START\n");
    Instrukcja(lab, pw, pk+1);
    printf("STOP\n");

    fclose(lab);
    return 0;
}
