#include<stdio.h>
#include<stdlib.h>
#include"../analisador_lexico/analisador_lexico.h"
#include"tabela.h"
#define MAX 100

int main(int argc, char **argv){
   char caminho[MAX] = "../teste/";
   sprintf(caminho, "%s%s",caminho,argv[1]);
   printf("Abrindo arquivo %s\n",caminho);
   FILE *arquivo;
   char caracter;
   tokens *token;
   token = NULL;
   registro * tabela;

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
      // pega o valor da li
      caracter = fgetc(arquivo);
   }

   imprimir_lista(token);

   
   fclose(arquivo);
}