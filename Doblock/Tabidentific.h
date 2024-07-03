#ifndef _TAB_IDENTIFIC_
#define _TAB_IDENTIFIC_

#include <stdlib.h>
#include <stdio.h>
#include "Analex.h"

#define TAM_MAX_TAB_IDENTIF 200
#define TAMANHO_MAX_DIMENSOES 10

typedef
    struct {
        char nomeId[TAMANHO_MAX_LEXEMA];
        int endereco;
        int escopo;
        int tipoVariavel;
        char categoria[3]; // VG, VL, BLK, ARG
        bool ehConstante;
        bool ehEscalar;
        int qtdDimensoes;
        /*
        Linha 0 - Valores de endereço de inteiro constante
        Linha 1 - Identifica se é constante '0' ou ID '1'
        */
        int dimensoes[2][TAMANHO_MAX_DIMENSOES];
        bool statusAtivo; // Ativo/Inativo - Variavel zumbi
    } REG_IDENTIF;                      // Uma entrada na Tabela de Identificadores


typedef
    struct {
        int tamTab;
        REG_IDENTIF identificador[TAM_MAX_TAB_IDENTIF];
    } TAB_IDENTIF;                      // Tabela de identificadores


/* Variaveis globais */
extern TOKEN t;
extern TAB_IDENTIF tabIdentif;

/* Assinaturas de funcoes */
void IniciaTabelaID();
int BuscaTabelaID(char []);
int BuscaTabelaIDMesmoEscopo(char nomeId[], int escopo);
int InsereTabelaID(char nomeId[], int escopo, int tipoVariavel, char categoria[3], bool ehConstante);
int removeUltimoIDInserido();

#endif // _TAB_IDENTIFIC_
