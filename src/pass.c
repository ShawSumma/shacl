#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        exit(0);
    }
    exit(atol(argv[1]));
}
