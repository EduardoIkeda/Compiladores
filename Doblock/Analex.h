#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h>
#include <stdbool.h>

#define TAMANHO_MAX_LEXEMA 31
#define TAMANHO_MAX_STRING 100
#define TAMANHO_LEXEMA 50
#define TAMANHO_NUM 20
#define LETRAS (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') // c são letras maiusculas (a-z) e minusculas (A-Z)
#define DIGITOS (c >= '0' && c <= '9') // c é um numero de 0-9
#define CH_CHAR isprint(c) && (c != '\'') && (c != '\\')  // Caracteres imprimiveis em ASCII
#define CH_STRING isprint(c) && (c != '"') && (c != '\n')  // Caracteres imprimiveis em ASCII
#define TAMANHO_LISTA 30

#define END_LINE '\0' // \0
#define NEW_LINE '\n' // \n
#define BLANK ' ' // space
#define TAB '\t' // tab

enum TOKEN_CATEGORIA //Categorias validas para linguagem
{
    ID = 1,            // IDENTIFICADOR
    PALAVRA_RESERVADA, // PALAVRA RESERVADA
    SN,                // SINAL
    CT_INT,            // INTEGER
    CT_REAL,          // FLOAT
    CT_CHAR,
    CT_STRING,
    FIM_LINHA,
    FIM_ARQ
};

enum SINAIS //Sinais validos da linguagem
{
    // OPERADORES
    ATTRIB = 1, // ATRIBUIÇÃO
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    // SYMBOL
    COMMA, // VIRGULA
    ADDRESS, // ENDEREÇO DE UMA VARIAVEL
    // COMPARACAO
    NOT,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AND,
    OR,
    // (){}[]
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_KEY,
    CLOSE_KEY,
    OPEN_BRACKET,
    CLOSE_BRACKET
};

enum PALAVRAS_RESERVADAS //Sinais validos da linguagem
{
    BOOL = 0,   //0
    BLOCK,      //1
    CHAR,       //2
    CONST,      //3
    DO,         //4
    DOWNTO,     //5
    ELSE,       //6
    ELSEIF,     //7
    ENDBLOCK,   //8
    ENDIF,      //9
    ENDWHILE,   //10
    FOR,        //11
    FROM,       //12
    GETCHAR,    //13
    GETINT,     //14
    GETREAL,    //15
    GOBACK,     //16
    IF,         //17
    INT,        //18
    MAIN,       //19
    PUTCHAR,    //20
    PUTINT,     //21
    PUTREAL,    //22
    REAL,       //23
    STRING,     //24
    TIMES,      //25
    TO,         //26
    VARYING,    //27
    WHILE,      //28
    WITH        //29
};

typedef struct
{
    enum TOKEN_CATEGORIA categoria; // Recebe valores conforme definido em TOKEN_CAT
    union // parte variável do registro
    {                                
        int codigo;                         // Usado para SN
        char lexema[TAMANHO_MAX_LEXEMA];        // Usado para ID
        int valor_int;                      // Usado para CT_INT
        double valor_real;                  // Usado para CT_FLOAT
        char valor_char;                    // Usado para CT_CHAR
        char valor_string[TAMANHO_MAX_STRING];  // Usado para CT_STRING
    };
} TOKEN; // Estrutura do TOKEN

/* Contador de linhas do código fonte */
extern int contLinha;
extern FILE *codeFile;
extern TOKEN token;
extern TOKEN tokenLookAhead;
extern char *LISTA_PALAVRA_RESERVADA[];
extern bool printAnalex;
extern bool tokenFimlinha;

/* Assinaturas de funcoes */
TOKEN Analex(FILE *);

#endif