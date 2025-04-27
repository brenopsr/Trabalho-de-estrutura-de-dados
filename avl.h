#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da Árvore AVL
typedef struct AVLNode {
    char *word;              // Palavra armazenada
    int frequency;           // Frequência da palavra
    long *offsets;           // Lista dinâmica de offsets
    int count_offsets;       // Quantidade de offsets armazenados
    int capacity_offsets;    // Capacidade atual do vetor de offsets
    struct AVLNode *left;    // Ponteiro para filho à esquerda
    struct AVLNode *right;   // Ponteiro para filho à direita
    int height;              // Altura do nó
} AVLNode;

// Insere (ou atualiza) uma palavra na árvore AVL e retorna a raiz atualizada.
AVLNode* avl_insert(AVLNode *root, const char *word, long offset);

// Pesquisa uma palavra na árvore AVL e retorna o nó correspondente ou NULL se não encontrar.
AVLNode* avl_search(AVLNode *root, const char *word);

// Libera a memória alocada para a árvore AVL.
void avl_free(AVLNode *root);

#endif // AVL_H
