#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <stdbool.h>
#include <limits.h>

#include "vector.h"
#include "bst.h"
#include "avl.h"
#include "frequency_avl.h"

#define MAX_LINE 1024

// Converte uma string para minúsculo (para caracteres ASCII)
// Note: caracteres multibyte (acima de 127) não são alterados.
void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        if ((unsigned char)str[i] < 128) {
            str[i] = tolower((unsigned char)str[i]);
        }
        // se o caractere for >=128, presumimos que já está em caixa adequada
    }
}

// Remove pontuação para acabar com erros de leitura
void remove_punctuation(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if ((unsigned char)str[i] < 128) {
            if (isalnum((unsigned char)str[i]) || isspace((unsigned char)str[i])) {
                str[j++] = str[i];
            }
        } else {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

//inserção nas estruturas vector, BST, AVL


int carregarArquivo(const char *nomeArquivo, Vector *vetor, BSTNode **bst, AVLNode **avl) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return 0;
    }

    // Pula o BOM UTF-8 se existir
    unsigned char bom[3];
    if (fread(bom, 1, 3, fp) == 3) {
        if (!(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)) {
            fseek(fp, 0, SEEK_SET);
        }
    } else {
        fseek(fp, 0, SEEK_SET);
    }

    char linha[MAX_LINE];
    long offset;
    clock_t inicio, fim;
    double tempo_vetor = 0.0, tempo_bst = 0.0, tempo_avl = 0.0;

    initVector(vetor);
    *bst = NULL;
    *avl = NULL;

    while (1) {
        offset = ftell(fp); // Posição da linha no arquivo
        if (fgets(linha, MAX_LINE, fp) == NULL)
            break;
        linha[strcspn(linha, "\r\n")] = '\0';

        char *citacao;
        if (linha[0] == '\"') {
            char *fimCitacao = strchr(linha + 1, '\"');
            if (fimCitacao != NULL) {
                *fimCitacao = '\0';
                citacao = linha + 1;
            } else {
                citacao = linha + 1;
            }
        } else {
            citacao = strtok(linha, ",");
        }
        if (citacao == NULL)
            continue;

        char citacaoProcessada[MAX_LINE];
        strcpy(citacaoProcessada, citacao);
        remove_punctuation(citacaoProcessada);
        str_to_lower(citacaoProcessada);

        char *token = strtok(citacaoProcessada, " ");
        while (token != NULL) {
            if (strlen(token) > 3) {
                inicio = clock();
                vector_insert(vetor, token, offset);
                fim = clock();
                tempo_vetor += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                inicio = clock();
                *bst = bst_insert(*bst, token, offset);
                fim = clock();
                tempo_bst += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                inicio = clock();
                *avl = avl_insert(*avl, token, offset);
                fim = clock();
                tempo_avl += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(fp);
    printf("\nTempo total de insercao:\n");
    printf("Vetor (pesquisa binaria): %.6f segundos\n", tempo_vetor);
    printf("Arvore Binaria de Busca: %.6f segundos\n", tempo_bst);
    printf("Arvore AVL: %.6f segundos\n", tempo_avl);
    return 1;
}

void pesquisarPalavra(const char *nomeArquivo, Vector *vetor, BSTNode *bst, AVLNode *avl) {
    char palavra[100];
    printf("Digite a palavra a ser pesquisada: ");
    scanf("%99s", palavra);
    while(getchar() != '\n'); // Limpa buffer
    str_to_lower(palavra);

    clock_t inicio, fim;
    double tempo;

    inicio = clock();
    WordEntry *entrada_vetor = vector_search_entry(vetor, palavra);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    if (entrada_vetor != NULL)
        printf("[Vetor] Palavra '%s' encontrada com frequencia %d (tempo: %.6f s).\n",
               palavra, entrada_vetor->frequency, tempo);
    else
        printf("[Vetor] Palavra '%s' nao encontrada (tempo: %.6f s).\n",
               palavra, tempo);

    inicio = clock();
    BSTNode *no_bst = bst_search(bst, palavra);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    if (no_bst != NULL)
        printf("[BST] Palavra '%s' encontrada com frequencia %d (tempo: %.6f s).\n",
               palavra, no_bst->frequency, tempo);
    else
        printf("[BST] Palavra '%s' nao encontrada (tempo: %.6f s).\n",
               palavra, tempo);

    inicio = clock();
    AVLNode *no_avl = avl_search(avl, palavra);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    if (no_avl != NULL)
        printf("[AVL] Palavra '%s' encontrada com frequencia %d (tempo: %.6f s).\n",
               palavra, no_avl->frequency, tempo);
    else
        printf("[AVL] Palavra '%s' nao encontrada (tempo: %.6f s).\n",
               palavra, tempo);

    if (entrada_vetor != NULL) {
        FILE *fp = fopen(nomeArquivo, "r");
        if (fp == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo %s para leitura das citacoes.\n", nomeArquivo);
            return;
        }
        printf("\nCitacoes contendo a palavra '%s':\n", palavra);
        char buffer[MAX_LINE];
        for (int i = 0; i < entrada_vetor->count_offsets; i++) {
            fseek(fp, entrada_vetor->offsets[i], SEEK_SET);
            if (fgets(buffer, MAX_LINE, fp) != NULL) {
                buffer[strcspn(buffer, "\r\n")] = '\0';
                // Exibe a linha completa, preservando as aspas originais.
                printf("-> %s\n", buffer);
            }
        }
        fclose(fp);
    }
}

void buscaPorFrequencia(Vector *vetor) {
    FreqNode *freqTree = NULL;
    clock_t inicio, fim;
    double tempo_insercao = 0.0;

    inicio = clock();
    for (int i = 0; i < vetor->size; i++) {
        freqTree = freq_avl_insert(freqTree, vetor->entries[i].frequency, &vetor->entries[i]);
    }
    fim = clock();
    tempo_insercao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nArvore AVL de frequencia construída em %.6f segundos.\n", tempo_insercao);

    printf("Digite a frequencia a ser pesquisada: ");
    char freqStr[64];
    int freq;
    if (!fgets(freqStr, sizeof(freqStr), stdin)) {
        fprintf(stderr, "Erro de leitura da frequencia.\n");
        freq_avl_free(freqTree);
        return;
    }
    freqStr[strcspn(freqStr, "\n")] = '\0';
    if (strlen(freqStr) == 0) {
        printf("Entrada invalida!\n");
        freq_avl_free(freqTree);
        return;
    }
    char *endptr;
    long freqVal = strtol(freqStr, &endptr, 10);
    if (*endptr != '\0') {
        printf("Entrada invalida! Por favor insira um numero inteiro.\n");
        freq_avl_free(freqTree);
        return;
    }
    if (freqVal < 0 || freqVal > INT_MAX) {
        printf("Numero fora do intervalo!\n");
        freq_avl_free(freqTree);
        return;
    }
    freq = (int)freqVal;

    inicio = clock();
    FreqNode *noFreq = freq_avl_search(freqTree, freq);
    fim = clock();
    double tempo_busca = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (noFreq != NULL) {
        printf("Foram encontradas %d palavra(s) com frequencia %d (tempo: %.6f s):\n",
               noFreq->count_words, freq, tempo_busca);
        for (int i = 0; i < noFreq->count_words; i++) {
            printf(" - %s\n", noFreq->words[i]->word);
        }
    } else {
        printf("Nenhuma palavra com frequencia %d foi encontrada (tempo: %.6f s).\n",
               freq, tempo_busca);
    }

    freq_avl_free(freqTree);
}

int main() {
    // Configura o locale para o padrão do sistema (geralmente UTF-8 no Linux)
    setlocale(LC_ALL, "");

    Vector vetor = {0};
    BSTNode *bst = NULL;
    AVLNode *avl = NULL;
    bool arquivoCarregado = false;

    char nomeArquivo[256];
    printf("Informe o nome do arquivo, ex: 'movie_quotes.csv': ");
    scanf("%255s", nomeArquivo);
    while(getchar() != '\n');

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Carregar arquivo e construir estruturas\n");
        printf("2. Pesquisar palavra\n");
        printf("3. Buscar por frequencia\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        char opcaoStr[16];
        if (!fgets(opcaoStr, sizeof(opcaoStr), stdin)) {
            fprintf(stderr, "Erro de leitura de opcao.\n");
            opcao = 4;
        } else {
            if (sscanf(opcaoStr, "%d", &opcao) != 1) {
                opcao = -1;
            }
        }

        switch(opcao) {
            case 1:
                if (vetor.size > 0) { freeVector(&vetor); }
                if (bst != NULL) { bst_free(bst); bst = NULL; }
                if (avl != NULL) { avl_free(avl); avl = NULL; }
                if (carregarArquivo(nomeArquivo, &vetor, &bst, &avl)) {
                    arquivoCarregado = true;
                } else {
                    arquivoCarregado = false;
                }
                break;
            case 2:
                if (!arquivoCarregado) {
                    printf("Nenhum arquivo carregado. Use a opcao 1 primeiro.\n");
                    break;
                }
                pesquisarPalavra(nomeArquivo, &vetor, bst, avl);
                break;
            case 3:
                if (!arquivoCarregado) {
                    printf("Nenhum arquivo carregado. Use a opcao 1 primeiro.\n");
                    break;
                }
                buscaPorFrequencia(&vetor);
                break;
            case 4:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 4);

    freeVector(&vetor);
    bst_free(bst);
    avl_free(avl);
    return 0;
}
