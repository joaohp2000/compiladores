#ifndef _MEMORIA
#define _MEMORIA

#include <stdio.h>
#include <stdlib.h>


typedef int memoria; //tipo da struct 
extern memoria *m,s;

void inicializar_memoria(memoria *M, memoria *S);
void alocar(int pos, int num_elementos);
void desalocar(int pos, int num_elementos);

int ler_mem(int posicao);
void gravar_mem(int valor, int posicao);
void push(int valor);
int pop();
void imprimir_memoria();

#endif