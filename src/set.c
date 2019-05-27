#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "set take arguments");
        exit(1);
    }
    FILE *f = fopen(argv[1], "w");
    for (int i = 2; i < argc; i++) {
        fprintf(f, "%s", argv[i]);
    }
    fclose(f);
}