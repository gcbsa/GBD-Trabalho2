#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 200
#define BUCKET_CAPACITY 200
#define BUCKET_COUNT 100

unsigned nro_buckets_estouro = 0;
unsigned long long histograma[BUCKET_COUNT] = {0};

// A ordem dos elementos no Bucket não é garantida.
struct Bucket {
    unsigned nextFreeIndex;
    char* values[BUCKET_CAPACITY];
    struct Bucket* estouro;
};

void Bucket_insert(struct Bucket* b, char* str) {
    if (b->nextFreeIndex >= BUCKET_CAPACITY) {  // Estouro
        if (b->estouro == NULL) {
            nro_buckets_estouro++;
            b->estouro = malloc(sizeof(struct Bucket));
            b->estouro->nextFreeIndex = 0;
            b->estouro->estouro = NULL;
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
    for (int i = 0; i < BUCKET_CAPACITY; ++i) {
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

// Deve ser zero-initialized
typedef struct Bucket HashMap[BUCKET_COUNT];

unsigned long stringHash(const char* restrict str) {
    size_t n = strlen(str);
    unsigned long acc = 0;

    for (size_t i = 0; i < n; ++i)
        acc += str[i] * pow(31, n - i - 1);

    return acc % BUCKET_COUNT;
}

void HashMap_insert(HashMap hashMap, char* str) {
    long index = stringHash(str);
    histograma[index]++;
    Bucket_insert(&hashMap[index], str);
}

bool HashMap_remove(HashMap hashMap, char* str) {
    long index = stringHash(str);
    return Bucket_remove(&hashMap[index], str);
}

int main(void) {
    HashMap registros = {0};
    char buffer[STRING_SIZE];
    FILE* file = fopen("dblp.txt", "r");
    if (file == NULL) {
        perror("Erro ao ler o arquivo");
        return EXIT_FAILURE;
    }

    while (fgets(buffer, STRING_SIZE, file) != NULL) {
        HashMap_insert(registros, buffer);
    }
    printf("Numero de buckets com estouro: %u\n", nro_buckets_estouro);

    FILE* grafico = fopen("grafico.plt", "w");
    fprintf(grafico,
            "set title \"Histograma\"\n"
            "set encoding iso_8859_1\n"
            "set xlabel \"Hash\"\n"
            "set ylabel \"Quantidade\"\n"
            "plot '-' title 'Quantidade' with linespoints linewidth 2 linetype "
            "1 pointtype 1\n");
    for (int i = 0; i < BUCKET_COUNT; i++)
        fprintf(grafico, "%d %llu\n", i, histograma[i]);
    fprintf(grafico,
            "end\n"
            "pause -1\n");

    fclose(file);
    fclose(grafico);
    return EXIT_SUCCESS;
}
