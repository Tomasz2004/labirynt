#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/resource.h>

#include "konwerter.h"


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


Wymiary przekonwertuj(FILE* in, FILE* out) {
    FILE* pom = fopen("pomocniczy.txt", "w+");
    
    uint8_t separator;
    uint8_t value;
    uint8_t count;
    struct LaBin b;
    fread(&b.fileId, sizeof(b.fileId), 1, in);
    fread(&b.escape, sizeof(b.escape), 1, in);
    fread(&b.columns, sizeof(b.columns), 1, in);
    fread(&b.lines, sizeof(b.lines), 1, in);
    fread(&b.entry_x, sizeof(b.entry_x), 1, in);
    fread(&b.entry_y, sizeof(b.entry_y), 1, in);
    fread(&b.exit_x, sizeof(b.exit_x), 1, in);
    fread(&b.exit_y, sizeof(b.exit_y), 1, in);
    fread(&b.reserved, sizeof(b.reserved), 1, in);
    fread(&b.counter, sizeof(b.counter), 1, in);
    fread(&b.solutionOffset, sizeof(b.solutionOffset), 1, in);
    fread(&b.separator, sizeof(b.separator), 1, in);
    fread(&b.wall, sizeof(b.wall), 1, in);
    fread(&b.path, sizeof(b.path), 1, in);

    Wymiary wymiary = malloc(sizeof(struct Wym));
    wymiary->pk = b.entry_x - 1;
    wymiary->pw = b.entry_y - 1;
    wymiary->kk = b.exit_x - 1;
    wymiary->kw = b.exit_y - 1;
    wymiary->n = b.lines;
    wymiary->m = b.columns;

    for (int i = 0; i <= b.columns * b.lines; i += count + 1) {
        fread(&separator, sizeof(uint8_t), 1, in);
        fread(&value, sizeof(uint8_t), 1, in);
        fread(&count, sizeof(uint8_t), 1, in);
        if (value == b.path) {
            for (int j = 0; j <= count; j++) {
                fprintf(pom, " ");
            }
        }
        else if (value == b.wall) {
            for (int k = 0; k <= count; k++) {
                fprintf(pom, "X");
            }
        }
        else {
            break;
        }

    }

    fclose(pom);
    pom = fopen("pomocniczy.txt", "r");
    if (!pom) {
        fprintf(stderr, "Nie udało się otworzyć pliku z zdekodowanym labiryntem\n");
        exit(0);
    }

    char c;
    int x, y;
    for (y = 0; y < b.lines; y++) {
        for (x = 0; x < b.columns; x++) {
            c = getc(pom);
            if (x == wymiary->pk && y == wymiary->pw) {
                fprintf(out, "P");
            }
            else if (x == wymiary->kk && y == wymiary->kw) {
                fprintf(out, "K");
            }
            else {
                fprintf(out, "%c", c);
            }
        }
        fprintf(out, "\n");
    }
    fclose(pom);
}