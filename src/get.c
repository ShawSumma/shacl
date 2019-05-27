#include <stdio.h>

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        char got = fgetc(f);
        while (got != EOF) {
            putchar(got);
            got = fgetc(f);
        }
        fclose(f);
    }
}
