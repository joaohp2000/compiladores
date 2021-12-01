#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"headers/analisador_lexico.h"
#include"headers/tabela.h"
#include"headers/analisador_sintatico.h"
#define MAX 100

int main(int argc, char **argv){
   FILE *saida;
   char caminho[MAX];// = "teste/sint";       
   sprintf(caminho, "%s.txt",argv[1]);   //Le o nome arquivo  passado por parametro
   printf("Abrindo arquivo %s\n",caminho);
   if(argc == 3){
      if(!strcmp(argv[2], "file")){
         freopen("output.txt", "w+", stdout); 
      }
   }

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
   char cc;
   caracter = fgetc(arquivo);
   while (caracter != EOF)
   {
      pega_token(arquivo, caracter, &token); //le caractere e passa para funçao pega_token
      caracter = fgetc(arquivo);
      }
   int caminho_tam = strlen(caminho);
   
   sprintf(caminho, "%s.obj",argv[1]);

   analisador_sintatico(token, caminho);
   //imprimir_tabela(tabela);
   fclose(arquivo);
   return 1;
}