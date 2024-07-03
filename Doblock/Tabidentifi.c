#include <stdlib.h>
#include <string.h>
#include "Funcaux.h"
#include "Tabidentific.h"

void IniciaTabelaID() {
    tabIdentif.tamTab = 0;
}

/*
Retorna posição de um identificador,
Se não encontrar, retorna -1
*/
int BuscaTabelaID(char nomeId[])
{
    int i;

    for (i=0; i<tabIdentif.tamTab; i++) {
        if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)))
            return i;
    }
    return -1;
}

int BuscaTabelaIDMesmoEscopo(char nomeId[], int escopo)
{
    int i;

    for (i=0; i<tabIdentif.tamTab; i++) {
        if (!(strcmp(nomeId, tabIdentif.identificador[i].nomeId)))
            if( tabIdentif.identificador[i].escopo == escopo)
                return i;
    }
    return -1;
}


int InsereTabelaID(char nomeId[], int escopo, int tipoVariavel, char categoria[3], bool ehConstante)
{
    int i;
    if (tabIdentif.tamTab == TAM_MAX_TAB_IDENTIF)
        erro("Estouro na tabela de identificadores!");
    
    i = tabIdentif.tamTab;
    
    strcpy(tabIdentif.identificador[i].nomeId, nomeId);
    tabIdentif.identificador[i].endereco = i;
    tabIdentif.identificador[i].escopo = escopo;
    tabIdentif.identificador[i].tipoVariavel = tipoVariavel;
    strcpy(tabIdentif.identificador[i].categoria, categoria);
    tabIdentif.identificador[i].ehConstante = ehConstante;
    tabIdentif.identificador[i].ehEscalar = true;
    tabIdentif.identificador[i].qtdDimensoes = 0;
    tabIdentif.identificador[i].statusAtivo = true;
    
    tabIdentif.tamTab++;
    
    return i;
}

int removeUltimoIDInserido(int posicaoUltimoInserido)
{
    tabIdentif.identificador[posicaoUltimoInserido].nomeId[0] = '\0';
    tabIdentif.identificador[posicaoUltimoInserido].endereco = 0;
    tabIdentif.identificador[posicaoUltimoInserido].escopo = 0;
    tabIdentif.identificador[posicaoUltimoInserido].tipoVariavel = 0;
    tabIdentif.identificador[posicaoUltimoInserido].categoria[0] = '\0';
    tabIdentif.identificador[posicaoUltimoInserido].ehConstante = false;
    tabIdentif.identificador[posicaoUltimoInserido].ehEscalar = false;
    tabIdentif.identificador[posicaoUltimoInserido].qtdDimensoes = 0;
    tabIdentif.identificador[posicaoUltimoInserido].statusAtivo = false;
    return posicaoUltimoInserido - 1;
}