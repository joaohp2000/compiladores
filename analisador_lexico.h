#ifndef _ANALISA_LEXICO
#define _ANALISA_LEXICO

#include <stdio.h>
#include <stdlib.h>

struct tokens{
   char *lexema;
   char *simbolo;
   struct tokens *prox;
};
typedef struct tokens tokens; //tipo da struct 

void pega_token(FILE *arquivo, char caracter, tokens **token);
void inserir_lista(tokens **lista, char *lexema, char *simbolo);
void imprimir_lista(tokens *lista);



#endif