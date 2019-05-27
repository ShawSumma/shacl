#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "math takes an operator");
        exit(1);
    }
    if (strcmp(argv[1], "add") == 0) {
        double ret = 0;
        for (int i = 2; i < argc; i++) {
            ret += atof(argv[i]);
        }
        printf("%lf", ret);
        return 0;
    }
    if (strcmp(argv[1], "sub") == 0) {
        if (argc == 3) {
            printf("%lf\n", -atof(argv[2]));
            return 0;
        }
        double ret = atof(argv[2]);
        for (int i = 3; i < argc; i++) {
            ret -= atof(argv[i]);
        }
        printf("%lf", ret);
    }
    if (strcmp(argv[1], "mul") == 0) {
        double ret = 1;
        for (int i = 2; i < argc; i++) {
            ret *= atof(argv[i]);
        }
        printf("%lf", ret);
        return 0;
    }
    if (strcmp(argv[1], "div") == 0) {
        if (argc == 3) {
            printf("%lf\n", 1/atof(argv[2]));
            return 0;
        }
        double ret = atof(argv[2]);
        for (int i = 3; i < argc; i++) {
            ret /= atof(argv[i]);
        }
        printf("%lf", ret);
    }
    if (strcmp(argv[1], "mod") == 0) {
        double ret = atof(argv[2]);
        for (int i = 3; i < argc; i++) {
            ret = fmod(ret, atof(argv[i]));
        }
        printf("%lf", ret);
    }
}
