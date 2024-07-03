#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"
#include "Funcaux.h"

/*Função retorna token com base no arquivo informado*/
TOKEN AnalexProxy(FILE *codeFile)
{
    int i;

    int estado;
    char lexema[TAMANHO_LEXEMA] = "";
    int tamanho_Lexema = 0;

    char digitos[TAMANHO_NUM] = "";
    int tamanho_Digitos = 0;

    char valor_string[TAMANHO_MAX_STRING] = "";
    int tamanho_String = 0;

    char valor_char;

    bool eh_palavra_reservada = false;

    TOKEN token;

    estado = 0;

    while (true)
    {
        char c = fgetc(codeFile);
        switch (estado)
        {
        case 0:
            if (c == BLANK || c == TAB)
                estado = 0;
            else if (LETRAS)    // Identificador
            {
                estado = 11;
                lexema[tamanho_Lexema] = c;
                lexema[++tamanho_Lexema] = END_LINE;
            }
            else if (c == '_')  // Identificador
            {
                estado = 12;
                lexema[tamanho_Lexema] = c;
                lexema[++tamanho_Lexema] = END_LINE;
            }
            else if (DIGITOS)   // Digito real ou inteiro
            {
                estado = 14;
                digitos[tamanho_Digitos] = c;
                digitos[++tamanho_Digitos] = END_LINE;
            }
            else if (c == '\'') // Char
            {
                estado = 7;
                token.categoria = CT_CHAR;
            }
            else if (c == '"')  // String
            {
                estado = 4;
                token.categoria = CT_STRING;
            }
            else if (c == '+')  // Adição
            {
                estado = 28;
                token.categoria = SN;
                token.codigo = ADD;
                return token;
            }
            else if (c == '-')  // Subtração
            { // sinal de subtracao - monta e devolve token
                estado = 29;
                token.categoria = SN;
                token.codigo = SUBTRACT;
                return token;
            }
            else if (c == '*')  // Multiplicação
            {
                estado = 30;
                token.categoria = SN;
                token.codigo = MULTIPLY;
                return token;
            }
            else if (c == '/')  // Divisão
            {
                estado = 1;
            }
            else if (c == '=')  // Atribuição ou Igualdade
            {
                estado = 25;
                token.categoria = SN;
            }
            else if (c == '!')  // Negação ou Diferente
            {
                estado = 46;
                token.categoria = SN;
            }
            else if (c == '|')  // Ou
            {
                estado = 40;
                token.categoria = SN;
            }
            else if (c == '>')  // Maior ou Maior igual
            {
                estado = 34;
                token.categoria = SN;
            }
            else if (c == '<')  // Menor ou Menor igual
            {
                estado = 37;
                token.categoria = SN;
            }
            else if (c == ',')  // Virgula
            {
                estado = 6;
                token.categoria = SN;
                token.codigo = COMMA;
                return token;
            }
            else if (c == '(')  // Abre parenteses
            {
                estado = 20;
                token.categoria = SN;
                token.codigo = OPEN_PARENTHESIS;
                return token;
            }
            else if (c == ')')  // Fecha parenteses
            {
                estado = 19;
                token.categoria = SN;
                token.codigo = CLOSE_PARENTHESIS;
                return token;
            }
            else if (c == '{')  // Abre chaves
            {
                estado = 22;
                token.categoria = SN;
                token.codigo = OPEN_KEY;
                return token;
            }
            else if (c == '}')  // Fecha chaves
            {
                estado = 23;
                token.categoria = SN;
                token.codigo = CLOSE_KEY;
                return token;
            }
            else if (c == '[')  // Abre colchetes
            {
                estado = 24;
                token.categoria = SN;
                token.codigo = OPEN_BRACKET;
                return token;
            }
            else if (c == ']')  // Fecha colchetes
            {
                estado = 23;
                token.categoria = SN;
                token.codigo = CLOSE_BRACKET;
                return token;
            }
            else if (c == NEW_LINE)
            {
                estado = 0;
                if(tokenFimlinha)
                    token.categoria = FIM_LINHA; // fim de linha (ou expressao) encontrado
                contLinha++;
                if(tokenFimlinha)
                    return token;
            }
            else if (c == EOF)
            { // fim do arquivo fonte encontrado
                token.categoria = FIM_ARQ;
                return token;
            }
            else // Sem transicao valida no AFD
            {
                erro("Caracter invalido na expressao!");
            }
        break;
        case 1:
            if(c == '/')    // Inicio de Comentario
                estado = 2;
            else    // Sinal de divisão
            {
                ungetc(c, codeFile);
                token.categoria = SN;
                token.codigo = DIVIDE;
                return token;
            }
            break;
        case 2:
            if (c == NEW_LINE || c == EOF)  // Fim do comentário
            {
                ungetc(c, codeFile);
                estado = 0;
            }
            else    // Consome comentario
                estado = 2;
            break;
        case 4:
            if (LETRAS || DIGITOS || CH_STRING) //Acumula String
            {
                estado = 4;
                valor_string[tamanho_String] = c;
                valor_string[++tamanho_String] = END_LINE;
            }
            else if(c == '"')   //Fim da string
            {
                estado = 5;               
                strcpy(token.valor_string, valor_string);
                return token;
            }
            else    //Ausencia de final da string ou caractere invalido
                erro("Falta uma aspas para fechar a string ou foi encontrado um caractere inválido.");
            break;
        case 7:
            if (LETRAS || DIGITOS || CH_CHAR)   // Char
            {
                estado = 8;
                token.valor_char = c;
            }
            else if(c == '\\')  // Char que pode ser \0 ou \n 
            {
                estado = 10;
            }
            else    // Ausencia de char valido
                erro("Espera-se um caractere válido após o apóstrofe.");
            break;
        case 8:     // Fecharmento de um char
            if (c == '\'')
            {
                estado = 9;
                return token;
            }
            else    // Caractere invalido ao invés do fechamento do apostrofe
                erro("Espera-se um apóstrofe para declaração de um char.");
            break;
        case 10:
            if (c == 'n')   // Char \n
            {
                estado = 42;
                token.valor_char = NEW_LINE;
            }
            else if('0')    // Char \0
            {
                estado = 44;
                token.valor_char = END_LINE;
            }
            else    //  Encontrado um char invalido após o barra invertida
                erro("Espera-se um apóstrofe para declaração de um char.");
            break;
        case 11:
            if (LETRAS || DIGITOS || (c == '_'))    //Monta a cadeia do lexema
            {
                estado = 11;
                lexema[tamanho_Lexema] = c;
                lexema[++tamanho_Lexema] = END_LINE;
            }
            else    //Encerra lexema vendo se é uma palavra reservada
            {               
                estado = 13;
                ungetc(c, codeFile);
                for (i = 0; i < TAMANHO_LISTA; i++)
                    if (strcmp(lexema, LISTA_PALAVRA_RESERVADA[i]) == 0)
                    {
                        eh_palavra_reservada = true;
                        break;
                    }

                if (eh_palavra_reservada)
                {
                    token.categoria = PALAVRA_RESERVADA;
                    token.codigo = i;
                }
                else
                {
                    token.categoria = ID;
                    strcpy(token.lexema, lexema);
                }
                return token;
            }
            break;
        case 12:
            if (LETRAS)     // Depois de começar com _, espera-se uma letra
            {
                estado = 11;
                lexema[tamanho_Lexema] = c;
                lexema[++tamanho_Lexema] = END_LINE;
            }
            else if(c == '_')
            {
                estado = 12;
                lexema[tamanho_Lexema] = c;
                lexema[++tamanho_Lexema] = END_LINE;
            }
            else   //Sem letra, ou _ gera erro
                erro("Espera-se uma letra depois do '_'.");
            break;
        case 14:
            if (DIGITOS)    // Monta a cadeia de digitos
            {
                estado = 14;
                digitos[tamanho_Digitos] = c;
                digitos[++tamanho_Digitos] = END_LINE;
            }
            else if(c == '.') // IDENTIFICA SE É UM NUMERO REAL
            {
                estado = 16;
                digitos[tamanho_Digitos] = '.';
                digitos[++tamanho_Digitos] = END_LINE;
            }
            else    //Se encontrar outro, monta o token inteiro
            {
                estado = 15;
                ungetc(c, codeFile);
                token.categoria = CT_INT;
                token.valor_int = atoi(digitos);
                return token;
            }
            break;

        case 16: // EXIGE UM DIGITO DEPOIS DO PONTO PARA SER REAL
            if(DIGITOS)
            {
                estado = 17;
                digitos[tamanho_Digitos] = c; // acumula digitos lidos na variaavel digitos
                digitos[++tamanho_Digitos] = END_LINE;
                break;
            }
            else    // Erro se nao encontrar digito depois do ponto
                erro("Espera-se um numero depois do ponto!");
            break;
        case 17:
            if(DIGITOS)     // Monta a cadeia de numeros depois da virgula
            {
                estado = 17;
                digitos[tamanho_Digitos] = c;
                digitos[++tamanho_Digitos] = END_LINE;
            }
            else    // Fim e retorna token real
            {
                estado = 18;
                ungetc(c, codeFile);
                token.categoria = CT_REAL;
                token.valor_real = atof(digitos);
                return token;
            }
            break;
        case 25:
            if(c == '=')    // Comparação igualdade
            {
                estado = 26;
                token.codigo = EQUAL;
            }
            else    // Atribuição
            {
                estado = 27;
                ungetc(c, codeFile);
                token.codigo = ATTRIB;
            }
            return token;
            break;
        case 31:
            if (c == '&')   // Comparação E
            {
                estado = 33;
                token.codigo = AND;
            }
            else    // Endereço
            {
                estado = 32;
                ungetc(c, codeFile);
                token.codigo = ADDRESS;
            }
            return token;
            break;
        case 34:
            if (c == '=')   // Comparação Maior igual
            {
                estado = 36;
                token.codigo = GREATER_EQUAL;
            }
            else    // Comparação Maior
            {
                estado = 35;
                ungetc(c, codeFile);
                token.codigo = GREATER;
            }
            return token;
            break;
        case 37:
            if (c == '=')   // Comparação Menor igual
            {
                estado = 38;
                token.codigo = LESS_EQUAL;
            }
            else    // Comparação Menor
            {
                estado = 39;
                ungetc(c, codeFile);
                token.codigo = LESS;
            }
            return token;
            break;
        case 40:
            if (c == '|')   // Comparação OU
            {
                estado = 41;
                token.codigo = OR;
                return token;
            }
            else    //  Erro, faltando outro |
                erro("Espera-se || para declaração de um 'OU'.");
            break;
        case 42:
            if (c == '\'')  // Fim token \n
            {
                estado = 43;
                return token;
            }
            else
                erro("Espera-se um apóstrofe para declaração de um char.");
            break;
        case 44:
            if (c == '\'')    // Fim token \0
            {
                estado = 45;
                return token;
            }
            else
                erro("Espera-se um apóstrofe para declaração de um char.");
            break;
        case 46:
            if (c == '=')   //Comparação Diferente
            {
                estado = 47;
                token.codigo = NOT_EQUAL;
            }
            else    //Comparação Negação
            {
                estado = 48;
                ungetc(c, codeFile);
                token.codigo = NOT;
            }
            return token;
            break;
        }
    }
}

void printToken(TOKEN token)
{
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
}

TOKEN Analex(FILE *codeFile)
{
    TOKEN token;
    static bool primVez = true;
   
    if (primVez)
    {
        primVez = false;
        token = AnalexProxy(codeFile);
        if (token.categoria == FIM_ARQ) tokenLookAhead = token;
        else tokenLookAhead = AnalexProxy(codeFile);
        if(printAnalex)
            printToken(token);
        return token;
    }
    else if (tokenLookAhead.categoria == FIM_ARQ)
    {
        return(tokenLookAhead);
    }
    else {
        token = tokenLookAhead;
        tokenLookAhead = AnalexProxy(codeFile);
        if(printAnalex)
        {
            printf("Linha %d: ", contLinha);
            printToken(token);
            printf("\n");
        }
        return token;
    }
}