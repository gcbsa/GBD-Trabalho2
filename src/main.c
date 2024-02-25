/*
Arthur Henrique de Souza 12121BCC031
Thiago Pacheco Rocha 12121BCC033
Gabriel Cardoso Brandão de Sousa Amaral 12011BSI255

Verifique se a troca do valor 31 na função hash por outro valor (outro número
primo? Testamos a distribuição com os números 31 e 53, o número 53 teve desempenho melhor.

Mesmo alterando a quantidade de elementos por bucket nos obtivemos a mesma quantidade de estouros: 34758
Logo é importate haver a funcionalidade de bucket de estouro no nosso programa.
*/

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 200
#define BUCKET_CAPACITY 200
#define BUCKET_COUNT 100

unsigned nro_buckets_estouro = 0;
unsigned long histograma[BUCKET_COUNT] = {0};

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

uint64_t stringHash(const char* restrict str) {
    size_t n = strlen(str);
    uint64_t acc = 0;

    for (size_t i = 0; i < n; ++i)
        acc += str[i] * pow(53, n - i - 1);

    return acc % BUCKET_COUNT;
}

void HashMap_insert(HashMap hashMap, char* str) {
    uint64_t index = stringHash(str);
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
        fprintf(grafico, "%d %lu\n", i, histograma[i]);
    fprintf(grafico,
            "end\n"
            "pause -1\n");

    fclose(file);
    fclose(grafico);
    return EXIT_SUCCESS;
}
