#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 200
typedef char* string_t;

// A ordem dos elementos no Bucket não é garantida.
#define BUCKET_CAPACITY 200
struct Bucket {
    unsigned nextFree;
    string_t values[BUCKET_CAPACITY];
};

bool Bucket_insert(struct Bucket* b, string_t str) {
    if (b->nextFree >= BUCKET_CAPACITY)  // Estouro
        return false;

    string_t tmp = malloc(sizeof(char) * STRING_SIZE);
    strcpy(tmp, str);
    b->values[b->nextFree] = tmp;
    b->nextFree++;
    return true;
}

bool Bucket_remove(struct Bucket* b, string_t str) {
    for (int i = 0; i < 200; ++i) {
        if (strcmp(b->values[i], str) == 0) {
            free(b->values[i]);
            b->values[i] = b->values[--b->nextFree];
            return true;
        }
    }
    return false;
}

#define BUCKET_COUNT 10
// Deve ser zero-initialized
typedef struct Bucket HashMap[BUCKET_COUNT];

long stringHash(const char* restrict str) {
    size_t n = strlen(str);
    long acc = 0;

    for (size_t i = 0; i < n; ++i)
        acc += str[i] * pow(31, n - i - 1);

    return acc % BUCKET_COUNT;
}

bool HashMap_insert(HashMap hashMap, string_t str) {
    long index = stringHash(str);
    return Bucket_insert(&hashMap[index], str);
}

bool HashMap_remove(HashMap hashMap, string_t str) {
    long index = stringHash(str);
    return Bucket_remove(&hashMap[index], str);
}

int main(void) {
    HashMap registros = {0};
    char buffer[256];

    FILE* file;
    file = fopen("dblp.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erro ao ler o arquivo");
        return 1;
    }
    while (fgets(buffer, 256, file) != NULL)
    {
        printf("%s %d\n", buffer,HashMap_insert(registros, buffer));
    }
    

    return 0;
}
