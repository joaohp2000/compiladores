#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analisador_lexico.h"

void main(){
   FILE *arquivo;
   char str[80], caracter;
   tokens *token;
   
   token = NULL;
   
   // Nome do arquivo a ser aberto: 
   printf("\n\n Digite o nome do arquivo: ");
   gets(str);
   
   // Erro na abertura do arquivo
   if ((arquivo = fopen(str, "r")) == NULL)
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
   
   imprimir_lista(token);
   fclose(arquivo);
}