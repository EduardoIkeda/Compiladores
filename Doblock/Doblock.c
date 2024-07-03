#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"
#include "Tabidentific.h"

void testaAnalex();
void imprimirTabela(TAB_IDENTIF lista, int tamanho);

FILE *codeFile;
int contLinha = 1;
TOKEN token, tokenLookAhead;
TAB_IDENTIF tabIdentif;
bool printAnalex, tokenFimlinha;
char TABS[MAX_TABS] = "";
char *LISTA_PALAVRA_RESERVADA[] =
{
    "bool",
    "block",
    "char",
    "const",
    "do",
    "downto",
    "else",
    "elseif",
    "endblock",
    "endif",
    "endwhile",
    "for",
    "from",
    "getchar",
    "getint",
    "getreal",
    "goback",
    "if",
    "int",
    "main",
    "putchar",
    "putint",
    "putreal",
    "real",
    "string",
    "times",
    "to",
    "varying",
    "while",
    "with"
};

int main()
{
    bool analiseSintatica = true;
    
    printAnalex = true;
    contLinha = 1;

    if ((codeFile = fopen("codigo.do", "r")) == NULL)
    {
        printf("Arquivo não encontrado.");
        exit(1);
    }
    
    if (analiseSintatica)
    {
        IniciaTabelaID();
        tokenFimlinha = false;
        prog();
    }
    else
    {
        printAnalex = false;
        tokenFimlinha = true;
        testaAnalex();
    }

    imprimirTabela(tabIdentif, tabIdentif.tamTab);

    return 0;
}

const char* obterTipoTexto(enum PALAVRAS_RESERVADAS tipo) {
    switch (tipo) {
        case INT: return "INT";
        case REAL: return "REAL";
        case CHAR: return "CHAR";
        case STRING: return "STRING";
        case BLOCK: return "BLOCK";
        // Adicione mais casos conforme necessário
        default: return "DESCONHECIDO";
    }
}

void imprimirTabela(TAB_IDENTIF lista, int tamanho) {
    printf("| %-20s | %-10s | %-6s | %-6s | %-10s | %-12s | %-10s | %-14s |\n",
           "Nome ID", "Endereço", "Escopo", "Tipo", "Categoria", "Constante", "Escalar", "Status Ativo");
    printf("|----------------------|------------|---------|---------|------------|--------------|------------|------------------|\n");

    for (int i = 0; i < tamanho; i++) {
        const char* tipoTexto = obterTipoTexto((enum PALAVRAS_RESERVADAS)lista.identificador[i].tipoVariavel);
        printf("| %-20s | %-10d | %-6s | %-6s | %-10s | %-12s | %-10s | %-14s |\n",
               lista.identificador[i].nomeId, lista.identificador[i].endereco, lista.identificador[i].escopo ? "Local" : "Global", tipoTexto,
               lista.identificador[i].categoria, lista.identificador[i].ehConstante ? "True" : "False",
               lista.identificador[i].ehEscalar ? "True" : "False", lista.identificador[i].statusAtivo ? "True" : "False");
    }
}


void testaAnalex()
{
    
    printf("LINHA %d: ", contLinha);
    while (1)
    {
        token = Analex(codeFile);
        switch (token.categoria)
        {
        case ID:
            printf("<ID, %s> ", token.lexema);
            break;
        case PALAVRA_RESERVADA:
            printf("<PALAVRA_RESERVADA, %s> ", LISTA_PALAVRA_RESERVADA[token.codigo]);
            break;
        case SN:
            switch (token.codigo)
            {
                case ADD:
                    printf("<SN, ADD> ");
                    break;
                case SUBTRACT:
                    printf("<SN, SUBTRACT> ");
                    break;
                case MULTIPLY:
                    printf("<SN, MULTIPLY> ");
                    break;
                case DIVIDE:
                    printf("<SN, DIVIDE> ");
                    break;
                case ATTRIB:
                    printf("<SN, ATTRIB> ");
                    break;
                case COMMA:
                    printf("<SN, COMMA> ");
                    break;
                case ADDRESS:
                    printf("<SN, ADDRESS> ");
                    break;
                case NOT:
                    printf("<SN, NOT> ");
                    break;
                case EQUAL:
                    printf("<SN, EQUAL> ");
                    break;
                case NOT_EQUAL: 
                    printf("<SN, NOT_EQUAL> ");
                    break;
                case GREATER:
                    printf("<SN, GREATER> ");
                    break;
                case GREATER_EQUAL:
                    printf("<SN, GREATER_EQUAL> ");
                    break;
                case LESS:
                    printf("<SN, LESS> ");
                    break;
                case LESS_EQUAL:
                    printf("<SN, LESS_EQUAL> ");
                    break;
                case AND:
                    printf("<SN, AND> ");
                    break;
                case OR:
                    printf("<SN, OR> ");
                    break;
                case OPEN_PARENTHESIS:
                    printf("<SN, OPEN_PARENTHESIS> ");
                    break;
                case CLOSE_PARENTHESIS:
                    printf("<SN, CLOSE_PARENTHESIS> ");
                    break;
                case OPEN_KEY:
                    printf("<SN, OPEN_KEY> ");
                    break;
                case CLOSE_KEY:
                    printf("<SN, CLOSE_KEY> ");
                    break;
                case OPEN_BRACKET:
                    printf("<SN, OPEN_BRACKET> ");
                    break;
                case CLOSE_BRACKET:
                    printf("<SN, CLOSE_BRACKET> ");
                    break;
            }
            break;
        case CT_INT:
            printf("<CT_INT, %d> ", token.valor_int);
            break;
        case CT_REAL:
            printf("<CT_REAL, %f> ", token.valor_real);
            break;
        case CT_CHAR:
            if(token.valor_char == NEW_LINE)
                printf("<CT_CHAR, '\\n'> ");
            else if(token.valor_char == END_LINE)
                printf("<CT_CHAR, '\\0'> ");
            else
            printf("<CT_CHAR, '%c'> ", token.valor_char);
            break;
        case CT_STRING:
            printf("<CT_STRING, \"%s\"> ", token.valor_string);
            break;
        case FIM_LINHA:
            printf("<FIM_LINHA, %d>\n", 0);
            printf("LINHA %d: ", contLinha);
            break;
        case FIM_ARQ:
            printf(" <Fim do arquivo encontrado>\n");
        }
        if (token.categoria == FIM_ARQ)
            break;
    }
}