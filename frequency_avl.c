#include "frequency_avl.h"

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define INITIAL_WORDS_CAPACITY 5

// Função para obter a altura de um nó de frequência
static int heightFreq(FreqNode *node) {
    return (node == NULL) ? 0 : node->height;
}

// Função para obter o máximo entre dois inteiros
static int maxFreq(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó de frequência com a entrada fornecida
static FreqNode* createFreqNode(int frequency, WordEntry *entry) {
    FreqNode *node = (FreqNode *)malloc(sizeof(FreqNode));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para FreqNode.\n");
        exit(EXIT_FAILURE);
    }
    node->frequency = frequency;
    node->capacity_words = INITIAL_WORDS_CAPACITY;
    node->count_words = 0;
    node->words = (WordEntry **)malloc(node->capacity_words * sizeof(WordEntry *));
    if (node->words == NULL) {
        fprintf(stderr, "Erro ao alocar memória para vetor de palavras na FreqNode.\n");
        exit(EXIT_FAILURE);
    }
    // Adiciona a entrada
    node->words[node->count_words++] = entry;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Rotação à direita para a árvore de frequência
static FreqNode* rightRotateFreq(FreqNode *y) {
    FreqNode *x = y->left;
    FreqNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxFreq(heightFreq(y->left), heightFreq(y->right)) + 1;
    x->height = maxFreq(heightFreq(x->left), heightFreq(x->right)) + 1;
    return x;
}

// Rotação à esquerda para a árvore de frequência
static FreqNode* leftRotateFreq(FreqNode *x) {
    FreqNode *y = x->right;
    FreqNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxFreq(heightFreq(x->left), heightFreq(x->right)) + 1;
    y->height = maxFreq(heightFreq(y->left), heightFreq(y->right)) + 1;
    return y;
}

// Obtém o fator de balanceamento para a árvore de frequência
static int getBalanceFreq(FreqNode *node) {
    return (node == NULL) ? 0 : heightFreq(node->left) - heightFreq(node->right);
}

// Insere uma entrada na árvore AVL de frequência
FreqNode* freq_avl_insert(FreqNode *root, int frequency, WordEntry *entry) {
    if (root == NULL)
        return createFreqNode(frequency, entry);

    if (frequency < root->frequency)
        root->left = freq_avl_insert(root->left, frequency, entry);
    else if (frequency > root->frequency)
        root->right = freq_avl_insert(root->right, frequency, entry);
    else {
        // Frequência já existe: adiciona a entrada no vetor de palavras
        if (root->count_words >= root->capacity_words) {
            root->capacity_words *= 2;
            root->words = realloc(root->words, root->capacity_words * sizeof(WordEntry *));
            if (root->words == NULL) {
                fprintf(stderr, "Erro ao realocar memória para vetor de palavras na FreqNode.\n");
                exit(EXIT_FAILURE);
            }
        }
        root->words[root->count_words++] = entry;
        return root;
    }

    // Atualiza a altura do nó
    root->height = 1 + maxFreq(heightFreq(root->left), heightFreq(root->right));
    int balance = getBalanceFreq(root);

    // Rotaciona se necessário
    // Caso Esquerda-Esquerda
    if (balance > 1 && frequency < root->left->frequency)
        return rightRotateFreq(root);
    // Caso Direita-Direita
    if (balance < -1 && frequency > root->right->frequency)
        return leftRotateFreq(root);
    // Caso Esquerda-Direita
    if (balance > 1 && frequency > root->left->frequency) {
        root->left = leftRotateFreq(root->left);
        return rightRotateFreq(root);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && frequency < root->right->frequency) {
        root->right = rightRotateFreq(root->right);
        return leftRotateFreq(root);
    }
    return root;
}

// Pesquisa um nó de frequência na árvore AVL de frequência
FreqNode* freq_avl_search(FreqNode *root, int frequency) {
    if (root == NULL)
        return NULL;
    if (frequency == root->frequency)
        return root;
    else if (frequency < root->frequency)
        return freq_avl_search(root->left, frequency);
    else
        return freq_avl_search(root->right, frequency);
}

// Libera a memória da árvore de frequência (pós-ordem)
void freq_avl_free(FreqNode *root) {
    if (root == NULL)
        return;
    freq_avl_free(root->left);
    freq_avl_free(root->right);
    free(root->words);
    free(root);
}
