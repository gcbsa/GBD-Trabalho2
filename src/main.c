#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 200

// A ordem dos elementos no Bucket não é garantida.
#define BUCKET_CAPACITY 200
struct Bucket {
    unsigned nextFreeIndex;
    char* values[BUCKET_CAPACITY];
    struct Bucket* estouro;
};

void Bucket_insert(struct Bucket* b, char* str) {
    if (b->nextFreeIndex >= BUCKET_CAPACITY) {  // Estouro
        if (b->estouro == NULL) {
            puts("Bucket de estouro utilizado");
            b->estouro = malloc(sizeof(struct Bucket));
        }
        Bucket_insert(b->estouro, str);
        return;
    }

    char* tmp = malloc(sizeof(char) * STRING_SIZE);
    strcpy(tmp, str);
    b->values[b->nextFreeIndex] = tmp;
    b->nextFreeIndex++;
}

bool Bucket_remove(struct Bucket* b, char* str) {
    for (int i = 0; i < 200; ++i) {
        if (strcmp(b->values[i], str) == 0) {
            free(b->values[i]);
            b->values[i] = b->values[--b->nextFreeIndex];
            return true;
        }
    }

    if (b->estouro == NULL)
        return false;

    return Bucket_remove(b->estouro, str);
}

#define BUCKET_COUNT 10
// Deve ser zero-initialized
typedef struct Bucket HashMap[BUCKET_COUNT];
#define HashMap(nome)                             \
    HashMap nome;                                 \
    for (unsigned i = 0; i < BUCKET_COUNT; ++i) { \
        nome[i].nextFreeIndex = 0;                \
        nome[i].estouro = NULL;                   \
    }

long stringHash(const char* restrict str) {
    size_t n = strlen(str);
    long acc = 0;

    for (size_t i = 0; i < n; ++i)
        acc += str[i] * pow(31, n - i - 1);

    return acc % BUCKET_COUNT;
}

void HashMap_insert(HashMap hashMap, char* str) {
    long index = stringHash(str);
    Bucket_insert(&hashMap[index], str);
}

bool HashMap_remove(HashMap hashMap, char* str) {
    long index = stringHash(str);
    return Bucket_remove(&hashMap[index], str);
}

int main(void) {
    HashMap(registros);
    char buffer[256];

    printf("Digite uma string: ");
    fgets(buffer, 256, stdin);
    HashMap_insert(registros, buffer);
    printf("Deu certo? %d\n", HashMap_remove(registros, buffer));

    return 0;
}
