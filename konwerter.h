#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/resource.h>


typedef struct Wym {
    int pk;
    int pw;
    int kk;
    int kw;
    int n;
    int m;
} *Wymiary ;

Wymiary przekonwertuj(FILE* in, FILE* out);