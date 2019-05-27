#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    for (uint32_t i = 1; i < argc; i++) {
        if (i != 1) {
            printf(" ");
        }
        printf("%s", argv[i]);
    }
}
