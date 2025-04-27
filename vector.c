#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

// Tamanho inicial padrão para o vetor dinâmico de WordEntry e para os offsets
#define INITIAL_VECTOR_CAPACITY 100
#define INITIAL_OFFSET_CAPACITY 10

// Inicializa o vetor
void initVector(Vector *vec) {
    vec->size = 0;
    vec->capacity = INITIAL_VECTOR_CAPACITY;
    vec->entries = (WordEntry *)malloc(vec->capacity * sizeof(WordEntry));
    if (vec->entries == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor.\n");
        exit(EXIT_FAILURE);
    }
}

// Libera a memória de cada entrada e do vetor
void freeVector(Vector *vec) {
    for (int i = 0; i < vec->size; i++) {
        free(vec->entries[i].word);
        free(vec->entries[i].offsets);
    }
    free(vec->entries);
    // Reseta campos do vetor após liberar memória
    vec->entries = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

// Busca binária auxiliar: retorna índice e define o flag *found
int vector_binary_search(Vector *vec, const char *word, int *found) {
    int low = 0, high = vec->size - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        int cmp = strcmp(vec->entries[mid].word, word);
        if (cmp == 0) {
            *found = 1;
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    *found = 0;
    return low; // posição de inserção
}

// Insere ou atualiza uma palavra no vetor
void vector_insert(Vector *vec, const char *word, long offset) {
    int found;
    int pos = vector_binary_search(vec, word, &found);
    if (found) {
        // A palavra já existe: atualiza a frequência e insere o novo offset
        WordEntry *entry = &vec->entries[pos];
        entry->frequency++;
        if (entry->count_offsets >= entry->capacity_offsets) {
            entry->capacity_offsets *= 2;
            entry->offsets = realloc(entry->offsets, entry->capacity_offsets * sizeof(long));
            if (entry->offsets == NULL) {
                fprintf(stderr, "Erro ao realocar memória para offsets.\n");
                exit(EXIT_FAILURE);
            }
        }
        entry->offsets[entry->count_offsets++] = offset;
    } else {
        // Se o vetor estiver cheio, aumenta sua capacidade
        if (vec->size >= vec->capacity) {
            vec->capacity *= 2;
            vec->entries = realloc(vec->entries, vec->capacity * sizeof(WordEntry));
            if (vec->entries == NULL) {
                fprintf(stderr, "Erro ao realocar memória para o vetor.\n");
                exit(EXIT_FAILURE);
            }
        }
        // Desloca os elementos para abrir espaço para a nova entrada
        for (int i = vec->size; i > pos; i--) {
            vec->entries[i] = vec->entries[i - 1];
        }
        // Cria nova entrada
        vec->entries[pos].word = strdup(word);
        if (vec->entries[pos].word == NULL) {
            fprintf(stderr, "Erro ao alocar memória para a palavra.\n");
            exit(EXIT_FAILURE);
        }
        vec->entries[pos].frequency = 1;
        vec->entries[pos].capacity_offsets = INITIAL_OFFSET_CAPACITY;
        vec->entries[pos].count_offsets = 0;
        vec->entries[pos].offsets = (long *)malloc(INITIAL_OFFSET_CAPACITY * sizeof(long));
        if (vec->entries[pos].offsets == NULL) {
            fprintf(stderr, "Erro ao alocar memória para offsets.\n");
            exit(EXIT_FAILURE);
        }
        vec->entries[pos].offsets[vec->entries[pos].count_offsets++] = offset;
        vec->size++;
    }
}

// Procura e retorna a entrada correspondente à palavra
WordEntry* vector_search_entry(Vector *vec, const char *word) {
    int found;
    int pos = vector_binary_search(vec, word, &found);
    if (found)
        return &vec->entries[pos];
    else
        return NULL;
}
