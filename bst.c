#define _GNU_SOURCE
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_OFFSET_CAPACITY 10

// Cria um novo nó da BST com a palavra e offset fornecidos
static BSTNode* createBSTNode(const char *word, long offset) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para BSTNode.\n");
        exit(EXIT_FAILURE);
    }
    node->word = strdup(word);
    if (node->word == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a palavra.\n");
        exit(EXIT_FAILURE);
    }
    node->frequency = 1;
    node->capacity_offsets = INITIAL_OFFSET_CAPACITY;
    node->count_offsets = 0;
    node->offsets = (long *)malloc(INITIAL_OFFSET_CAPACITY * sizeof(long));
    if (node->offsets == NULL) {
        fprintf(stderr, "Erro ao alocar memória para offsets.\n");
        exit(EXIT_FAILURE);
    }
    node->offsets[node->count_offsets++] = offset;
    node->left = node->right = NULL;
    return node;
}

// Insere ou atualiza um nó na BST
BSTNode* bst_insert(BSTNode *root, const char *word, long offset) {
    if (root == NULL) {
        return createBSTNode(word, offset);
    }
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        // Palavra já existe: atualiza a frequência e adiciona o offset
        root->frequency++;
        if (root->count_offsets >= root->capacity_offsets) {
            root->capacity_offsets *= 2;
            root->offsets = realloc(root->offsets, root->capacity_offsets * sizeof(long));
            if (root->offsets == NULL) {
                fprintf(stderr, "Erro ao realocar memória para offsets na BST.\n");
                exit(EXIT_FAILURE);
            }
        }
        root->offsets[root->count_offsets++] = offset;
    } else if (cmp < 0) {
        // Se a palavra for menor, insere à esquerda
        root->left = bst_insert(root->left, word, offset);
    } else {
        // Se a palavra for maior, insere à direita
        root->right = bst_insert(root->right, word, offset);
    }
    return root;
}

// Pesquisa a palavra na BST
BSTNode* bst_search(BSTNode *root, const char *word) {
    if (root == NULL)
        return NULL;
    int cmp = strcmp(word, root->word);
    if (cmp == 0)
        return root;
    else if (cmp < 0)
        return bst_search(root->left, word);
    else
        return bst_search(root->right, word);
}

// Libera a memória alocada para a BST (pós-ordem)
void bst_free(BSTNode *root) {
    if (root == NULL)
        return;
    bst_free(root->left);
    bst_free(root->right);
    free(root->word);
    free(root->offsets);
    free(root);
}
