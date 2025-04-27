#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da Árvore Binária de Busca (BST)
typedef struct BSTNode {
    char *word;              // Palavra armazenada
    int frequency;           // Frequência da palavra
    long *offsets;           // Lista dinâmica de offsets
    int count_offsets;       // Quantidade de offsets armazenados
    int capacity_offsets;    // Capacidade atual do vetor de offsets
    struct BSTNode *left;    // Ponteiro para filho à esquerda
    struct BSTNode *right;   // Ponteiro para filho à direita
} BSTNode;

// Insere (ou atualiza) uma palavra na BST e retorna a raiz atualizada.
BSTNode* bst_insert(BSTNode *root, const char *word, long offset);

// Pesquisa uma palavra na BST e retorna o nó correspondente ou NULL se não encontrar.
BSTNode* bst_search(BSTNode *root, const char *word);

// Libera a memória alocada para a BST.
void bst_free(BSTNode *root);

#endif // BST_H
