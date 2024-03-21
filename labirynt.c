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

void oznacz(FILE *file) {
    char s[m+1];
    char pom[m+1];
    int czydroga=1;
    int i;
    char temp_filename[] = "/tmp/tempXXXXXX"; // Szablon nazwy pliku tymczasowego w katalogu /tmp

    // Tworzymy plik tymczasowy w katalogu /tmp
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) {
        perror("Błąd: nie można utworzyć pliku tymczasowego");
        exit(2);
    }

    FILE *temp_file = fdopen(temp_fd, "wb+"); // Tworzymy strumień pliku z deskryptora pliku
    if (temp_file == NULL) {
        perror("Błąd: nie można otworzyć strumienia pliku tymczasowego");
        exit(2);
    }

    for (i=0; i<n; i++){
        fseek(file, (m+2)*i, SEEK_SET);
        fgets(s, sizeof(s), file);
        for (int k = 0; k < m; k++){ //k to kolumny
            if (s[k]==' '){
                czydroga=0;
                if (s[k+1]==' ' || s[k+1]=='K'){
                    czydroga=czydroga+1;
                }
                if (s[k-1]==' ' || s[k-1]=='1' || s[k-1]=='2' || s[k-1]=='3' || s[k-1]=='4' || s[k-1]=='P'){
                    czydroga=czydroga+1;
                }
                fseek(file, (m+2)*(i-1), SEEK_SET);
                fgets(pom,sizeof(pom),file);
                if (pom[k]==' ' || pom[k]=='1' || pom[k]=='2' || pom[k]=='3' || pom[k]=='4'){
                    czydroga=czydroga+1;
                }
                fseek(file, (m+2)*(i+1), SEEK_SET);
                fgets(pom,sizeof(pom),file);
                if (pom[k]==' '){
                    czydroga=czydroga+1;
                }
                s[k]=czydroga + '0';
            }
        }
        fputs(s,temp_file); // Zapisujemy oznaczone linie do tymczasowego pliku
        fprintf(temp_file,"\n");
    }

    // Kopiujemy zawartość tymczasowego pliku do oryginalnego
    rewind(temp_file);
    int c;
    fclose(file);
    file=fopen("lab.txt","w");
    while ((c = fgetc(temp_file)) != EOF) {
        fputc(c, file);
    }

    fclose(temp_file);
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
    lab = fopen("lab.txt","r+");
    if (!lab){
        printf("Nie można otworzyć pliku");
        return 5;
    }
    oznacz(lab);
    fclose(lab);
    lab = fopen("lab.txt","r");
    printf("Oznaczony labirynt: \n");
    czytaj(lab);
    fclose(lab);
    fclose(file);
    return 0;
}

