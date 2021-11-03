#include<stdio.h>
#include<stdlib.h>
#include"analisador_lexico/analisador_lexico.h"
#include"tabela_simbolos/tabela.h"
#include"analisador_sintatico/analisador_sintatico.h"
#define MAX 100

int main(int argc, char **argv){
   char caminho[MAX];// = "teste/sint";
   sprintf(caminho, "teste/sint%s.txt",argv[1]);
   printf("Abrindo arquivo %s\n",caminho);
   FILE *arquivo;
   char caracter;
   tokens *token;
   token = NULL;
   
   // Erro na abertura do arquivo
   if ((arquivo = fopen(caminho, "r")) == NULL)
   {
      printf("Erro ao abrir o arquivo!\n");
      exit(1);
   }

   caracter = fgetc(arquivo);
   while (caracter != EOF)
   {
      pega_token(arquivo, caracter, &token);
      caracter = fgetc(arquivo);
   }
   //imprimir_lista(token);
   analisador_sintatico(token);
   imprimir_tabela(tabela);
   fclose(arquivo);
}