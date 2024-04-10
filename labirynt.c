#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int n;   // Liczba wierszy
int m;   // Liczba kolumn
int pw=-1;  // Numer wiersza P
int pk=-1;  // Numer kolumny P
int kw=-1;  // Numer wiersza K
int kk=-1;  // Numer kolumny K
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

int main(int argc, char** argv[]) {
    const char* filename = (argc >= 2) ? argv[1] : "maze.txt";
    FILE* file = fopen(filename, "r");
    if (!file){
        printf("Nie można odczytać pliku z labiryntem");
        return 2;
    }
    FILE *lab = fopen("lab.txt", "w+");
    if (!lab) {
        printf("Nie można utworzyć pliku\n");
        return 3;
    }

    rozmiar(file, lab);
    fclose(lab);
    lab = fopen("lab.txt", "r");
    printf("Labirynt: \n");
    czytaj(lab);
    rewind(lab);
    ZnajdzP(lab);
    rewind(lab);
    ZnajdzK(lab);
    fclose(lab);
    if (pw==-1 || pk==-1 || kw==-1 || kk==-1){
        printf("Brak P lub K\n");
        return 4;
    }
    lab = fopen("lab.txt", "r+");
    oznacz(lab, pw, pk+1);
    fclose(lab);
    /*lab = fopen("lab.txt", "r");
    if (!lab) {
        printf("Nie można otworzyć pliku do odczytu\n");
        return 4;
    }
    printf("Oznaczony labirynt: \n");
    czytaj(lab);*/
    lab = fopen("lab.txt", "r+");
    PoprawneOznaczenie(lab,kw,kk);
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
    Instrukcja(lab, pw, pk+1);
    printf("STOP\n");
    fclose(lab);
    fclose(file);
    return 0;
}


