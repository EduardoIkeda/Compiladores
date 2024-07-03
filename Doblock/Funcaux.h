#ifndef _FUNC_AUX_
#define _FUNC_AUX_

#include <stdbool.h>
#include <string.h>

#define MAX_TABS 200

enum MOVIM {AVANCA, MANTEM, RETROCEDE};

extern bool mostraArvore;
extern char TABS[200];
extern int contLinha;

void erro(char msg[]);
void PrintNodo(char info[], int movim);
void PrintNodoInt(int val, int movim);


#endif