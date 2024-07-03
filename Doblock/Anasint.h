#ifndef _ANASINT_
#define _ANASINT_

#include <stdio.h>
#include <stdbool.h>
#include "Analex.h"

#define INTCON (token.categoria == CT_INT)
#define REALCON (token.categoria == CT_REAL)
#define CHARCON (token.categoria == CT_CHAR)
#define STRINGCON (token.categoria == CT_STRING)
#define EH_OPERADOR_RELACIONAL (token.codigo == EQUAL || token.codigo == NOT_EQUAL || token.codigo == GREATER || token.codigo == GREATER_EQUAL || token.codigo == LESS || token.codigo == LESS_EQUAL)
#define EH_TIPO (token.codigo == INT || token.codigo == REAL || token.codigo == CHAR || token.codigo == BOOL)
#define EH_COMANDO (token.codigo == DO || token.codigo == IF || token.codigo == WHILE || token.codigo == GOBACK || token.codigo == GETINT || token.codigo == GETREAL || token.codigo == GETCHAR || token.codigo == PUTINT || token.codigo == PUTREAL || token.codigo == PUTCHAR)

/* Variaveis globais */
extern TOKEN t;
extern FILE *fd;
extern int contLinha;

extern bool mostraArvore;
extern char TABS[200];

/* Assinaturas das funcoes */
void prog();
void decl_list_var(int escopo);
void decl_block_prot();
void DeclareVariableBlock();
void block_main();
void endBlock(bool ehBlockMain, int posicaoArgumento);
void decl_var(int escopo, int tipo, bool ehConstante);
int tipo();
void block_def();
void decl_tipo_id_brackeys(int posicaoIdentificador);
void cmd();
void cmdOnce();
int checaID();
int id(int escopo, int tipoVariavel, char categoria[3], bool ehConstante);
int idVazio(int escopo, int tipoVariavel, char categoria[3], bool ehConstante);
void attrib();
void expr();
void expr_simp();
void termo();
void fator();
void op_rel();

#endif // ANASINT