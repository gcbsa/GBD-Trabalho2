#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

long stringHash(const char* restrict string) {
    size_t n = strlen(string);
    long acc = 0;

    for (size_t i = 0; i < n; ++i)
        acc += string[i] * pow(31, n - i - 1);

    return acc;
}

int main(void) {
    printf("Hash: %ld\n", stringHash("Bom dia"));

    return 0;
}
