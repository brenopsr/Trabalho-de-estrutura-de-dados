#ifndef FREQUENCY_AVL_H
#define FREQUENCY_AVL_H

#include "vector.h"

// Estrutura do nó da árvore AVL organizada por frequência.
typedef struct FreqNode {
    int frequency;             // Frequência (chave)
    WordEntry **words;         // Vetor de ponteiros para WordEntry com essa frequência
    int count_words;           // Número de palavras armazenadas neste nó
    int capacity_words;        // Capacidade atual do vetor de palavras
    struct FreqNode *left;     // Filho esquerdo
    struct FreqNode *right;    // Filho direito
    int height;                // Altura do nó
} FreqNode;

// Insere uma entrada (WordEntry) na árvore de frequências. Se já existir um nó com a mesma frequência,
// adiciona a entrada no vetor de palavras.
FreqNode* freq_avl_insert(FreqNode *root, int frequency, WordEntry *entry);

// Pesquisa um nó na árvore de frequências para uma dada frequência.
FreqNode* freq_avl_search(FreqNode *root, int frequency);

// Libera a memória alocada para a árvore AVL de frequência.
void freq_avl_free(FreqNode *root);

#endif // FREQUENCY_AVL_H
