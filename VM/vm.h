#ifndef __VM
#define __VM

#include "memoria.h"

struct comando
{
    int tipo;
    int jump;
    char instrucao[8];
    int arg1;
    int arg2;
};
struct tabela_memoria_auxiliar
{
    int rotulo;
    int endereco;
};
typedef struct tabela_memoria_auxiliar table_memory;
typedef struct comando comando;

extern comando **programa;
extern table_memory **table_aux;
//extern int *m,s=0;
void executar_programa(comando *programa, table_memory *table_aux, int num_intrucoes);
int ler_programa(FILE *_arquivo, comando **programa, table_memory **table_aux);

void instrucoes_completas(comando *programa, table_memory *table_aux, int *pc);
void instrucoes_longas(comando *programa, table_memory *table_aux, int *pc);
void instrucoes_curtas(comando *programa, table_memory *table_aux, int *pc);

#endif