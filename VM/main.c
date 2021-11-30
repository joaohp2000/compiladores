#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "memoria.h"
#include "vm.h"

#define MAX 100

int main(int argc, char **argv)
{
    comando *_programa = NULL;
    table_memory *_table_aux = NULL;
    char caminho[MAX]; // = "teste/sint";
    sprintf(caminho, "%s", argv[1]);
    printf("Abrindo arquivo %s\n", caminho);
    FILE *arquivo;
    if ((arquivo = fopen(caminho, "r")) == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }
    int pc = ler_programa(arquivo, &_programa, &_table_aux);
    executar_programa(_programa, _table_aux, pc);
    //printf("\n%d %d\n", _table_aux[0].rotulo, pc);
    fclose(arquivo);
    return 0;
}