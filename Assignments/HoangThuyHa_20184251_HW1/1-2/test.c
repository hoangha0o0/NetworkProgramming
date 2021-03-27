#include <string.h>
#include <stdio.h>

int main() {
    char a[20], b[20], f[20];
    double c, d;
    int e;
    char g;
    sscanf("S|20101160|NGUYEN VAN   |AN    | 8.5  | 7.0  | B |", "%[^|]|%d|%[^|]|%[^|]| %lf | %lf | %c |", a, &e, b, f, &c, &d, &g);
    printf("%s => %d => %s => %s => %lf => %lf => %c\n", a, e, b, f, c, d, g);
    return 0;
}