#define _GNU_SOURCE
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_OFFSET_CAPACITY 10

// Função para obter a altura de um nó
static int height(AVLNode *node) {
    return (node == NULL) ? 0 : node->height;
}

// Função para obter o máximo entre dois inteiros
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó AVL para a palavra
static AVLNode* createAVLNode(const char *word, long offset) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para AVLNode.\n");
        exit(EXIT_FAILURE);
    }
    node->word = strdup(word);
    if (node->word == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a palavra na AVL.\n");
        exit(EXIT_FAILURE);
    }
    node->frequency = 1;
    node->capacity_offsets = INITIAL_OFFSET_CAPACITY;
    node->count_offsets = 0;
    node->offsets = (long *)malloc(INITIAL_OFFSET_CAPACITY * sizeof(long));
    if (node->offsets == NULL) {
        fprintf(stderr, "Erro ao alocar memória para offsets na AVL.\n");
        exit(EXIT_FAILURE);
    }
    node->offsets[node->count_offsets++] = offset;
    node->left = node->right = NULL;
    node->height = 1; // Nó folha tem altura 1
    return node;
}

// Rotação à direita
static AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// Rotação à esquerda
static AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

// Obtém o fator de balanceamento do nó
static int getBalance(AVLNode *node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

// Insere ou atualiza a palavra na AVL
AVLNode* avl_insert(AVLNode *root, const char *word, long offset) {
    if (root == NULL)
        return createAVLNode(word, offset);

    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        // Palavra já existe: atualiza a frequência e adiciona offset
        root->frequency++;
        if (root->count_offsets >= root->capacity_offsets) {
            root->capacity_offsets *= 2;
            root->offsets = realloc(root->offsets, root->capacity_offsets * sizeof(long));
            if (root->offsets == NULL) {
                fprintf(stderr, "Erro ao realocar memória para offsets na AVL.\n");
                exit(EXIT_FAILURE);
            }
        }
        root->offsets[root->count_offsets++] = offset;
        return root;
    } else if (cmp < 0) {
        // Se a palavra for menor, insere à esquerda
        root->left = avl_insert(root->left, word, offset);
    } else {
        // Se a palavra for maior, insere à direita
        root->right = avl_insert(root->right, word, offset);
    }

    // Atualiza a altura do nó
    root->height = 1 + max(height(root->left), height(root->right));

    // Verifica o fator de balanceamento
    int balance = getBalance(root);

    // Caso Esquerda-Esquerda
    if (balance > 1 && strcmp(word, root->left->word) < 0)
        return rightRotate(root);

    // Caso Direita-Direita
    if (balance < -1 && strcmp(word, root->right->word) > 0)
        return leftRotate(root);

    // Caso Esquerda-Direita
    if (balance > 1 && strcmp(word, root->left->word) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && strcmp(word, root->right->word) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Pesquisa a palavra na árvore AVL
AVLNode* avl_search(AVLNode *root, const char *word) {
    if (root == NULL)
        return NULL;
    int cmp = strcmp(word, root->word);
    if (cmp == 0)
        return root;
    else if (cmp < 0)
        return avl_search(root->left, word);
    else
        return avl_search(root->right, word);
}

// Libera a memória da árvore AVL (pós-ordem)
void avl_free(AVLNode *root) {
    if (root == NULL)
        return;
    avl_free(root->left);
    avl_free(root->right);
    free(root->word);
    free(root->offsets);
    free(root);
}
