#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma entrada (word entry) no repositório.
// Cada entrada contém a palavra, a contagem de ocorrências e uma lista
// de offsets (posição no arquivo onde a linha com a citação inicia).
typedef struct {
    char *word;          // Palavra armazenada (todas em minúsculo)
    int frequency;       // Frequência da palavra
    long *offsets;       // Lista dinâmica de offsets
    int count_offsets;   // Quantos offsets foram armazenados
    int capacity_offsets;// Capacidade atual do vetor de offsets
} WordEntry;

// Estrutura que representa o vetor (array dinâmico) de entradas.
typedef struct {
    WordEntry *entries;  // Vetor dinâmico de WordEntry
    int size;            // Número de entradas atualmente armazenadas
    int capacity;        // Capacidade atual do vetor
} Vector;

// Inicializa o vetor
void initVector(Vector *vec);

// Libera a memória alocada pelo vetor
void freeVector(Vector *vec);

// Função auxiliar de busca binária no vetor ordenado alfabeticamente.
// Se a palavra for encontrada, *found é definido como 1 e retorna o índice da entrada.
// Se não for encontrada, *found é definido como 0 e retorna o índice onde a palavra deve ser inserida.
int vector_binary_search(Vector *vec, const char *word, int *found);

// Insere uma ocorrência da palavra com o offset informado no vetor.
// Se a palavra já existir, atualiza a frequência e adiciona o novo offset à lista.
// Se não existir, insere uma nova entrada mantendo a ordem alfabética.
void vector_insert(Vector *vec, const char *word, long offset);

// Procura uma entrada no vetor a partir da palavra e retorna o ponteiro para a entrada ou NULL se não existir.
WordEntry* vector_search_entry(Vector *vec, const char *word);

#endif // VECTOR_H
