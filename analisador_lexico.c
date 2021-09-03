#include "analisador_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char * verifica_palavras_reservadas(char *palavra);
static void gerencia_numero(FILE *arquivo, char caracter, tokens **token);
static void gerencia_palavra(FILE *arquivo, char caracter,tokens **token);
static void gerencia_atribuicao(FILE *arquivo, char caracter, tokens **token);
static void gerencia_opAritmetico(FILE *arquivo, char caracter,tokens **token);
static void gerencia_opRelacional(FILE *arquivo, char caracter,tokens **token);
static void gerencia_pontuacao(FILE *arquivo, char caracter,tokens **token);


static void gerencia_comentarios (FILE *arquivo, char caracter){
      // Fazendo isso pra jogar fora os comentarios
      // {testes}{}
      while ((caracter != '}') && caracter != EOF){
         caracter = fgetc(arquivo);
      } 
}

static void gerencia_vazios(FILE *arquivo, char caracter){
   while ((caracter == ' ') && caracter != EOF){
      caracter = fgetc(arquivo);
   } 
  fseek(arquivo, -1L, SEEK_CUR);
}

void pega_token(FILE *arquivo, char caracter, tokens **token){
  
   if(caracter == ' ') {
     // gerencia_vazios(arquivo, caracter);
   }
   else {
      if(caracter == '{'){
         gerencia_comentarios(arquivo,caracter);
      }
      else {  
         if (isdigit(caracter)){
            gerencia_numero(arquivo, caracter, token);
         }
         else {
            if (isalpha(caracter)){
               gerencia_palavra(arquivo, caracter, token);
               }
            else {
               if (caracter == ':'){
                  gerencia_atribuicao(arquivo, caracter, token);}
               else{
                  if (caracter == '+' || caracter == '-' || caracter == '*'){
                     gerencia_opAritmetico(arquivo, caracter, token);
                  }
                  else{
                     if (caracter == '<' || caracter == '>' || caracter == '=' || caracter == '!'){  
                        gerencia_opRelacional(arquivo, caracter, token);
                     }
                     else{ 
                        if (caracter == ';' || caracter == ',' || caracter == '(' || caracter == ')' || caracter == '.'){
                           gerencia_pontuacao(arquivo, caracter, token);
                        }
                        else{
                           if( caracter != '	'&& caracter != '\n' && caracter != 13 && caracter != ' ') inserir_lista(token, &caracter, "serro");
                        }
                     }
                  }
               }
            }
         }
      }
   }
   return;
}

static void gerencia_numero(FILE *arquivo, char caracter,tokens **token){
   char *num = malloc(sizeof(char));
   *num = caracter;
   caracter = fgetc(arquivo);

   while(isdigit(caracter)){
      size_t tam = strlen(num);
      num = realloc(num,(tam+2));
      num[tam] = caracter;
      num[tam + 1] = '\0';
      caracter = fgetc(arquivo);
   }

   inserir_lista(token, num, "snumero");
   fseek(arquivo, -1L, SEEK_CUR); 
   free(num);
}

static void gerencia_palavra(FILE *arquivo, char caracter,tokens **token){
   char *palavra = malloc(sizeof(char));
   FILE *aux = arquivo;
   *palavra = caracter;
   caracter = fgetc(aux);
   
   while(isalpha(caracter) ||  isdigit(caracter)){
      size_t tam = strlen(palavra);
      palavra = realloc(palavra,(tam+2));
      palavra[tam] = caracter;
      palavra[tam + 1] = '\0';
      caracter = fgetc(aux);

   }
   inserir_lista(token, palavra, verifica_palavras_reservadas(palavra));
   fseek(arquivo, -1L, SEEK_CUR); 
   free(palavra);
}

static char * verifica_palavras_reservadas(char *palavra){
   
   if (!strcmp(palavra, "programa"))
      return "sprograma";
   else if (!strcmp(palavra, "se"))
      return "sse";
   else if (!strcmp(palavra, "entao"))
      return "sentao";
   else if (!strcmp(palavra, "senao"))
      return "ssenao";
   else if (!strcmp(palavra, "enquanto"))
      return "senquanto";
   else if (!strcmp(palavra, "faca"))
      return "sfaca";
   else if (!strcmp(palavra, "início"))
      return "sinício";
   else if (!strcmp(palavra, "fim"))
      return "sfim";
   else if (!strcmp(palavra, "escreva"))
      return "sescreva";
   else if (!strcmp(palavra, "leia"))
      return "sleia";
   else if (!strcmp(palavra, "var"))
      return "svar";
   else if (!strcmp(palavra, "inteiro"))
      return "sinteiro";
   else if (!strcmp(palavra, "booleano"))
      return "sbooleano";
   else if (!strcmp(palavra, "verdadeiro"))
      return "sverdadeiro";
   else if (!strcmp(palavra, "falso"))
      return "sfalso";
   else if (!strcmp(palavra, "procedimento"))
      return "sprocedimento";
   else if (!strcmp(palavra, "funcao"))
      return "sfuncao";
   else if (!strcmp(palavra, "div"))
      return "sdiv";
   else if (!strcmp(palavra, "e"))
      return "se";
   else if (!strcmp(palavra, "ou"))
      return "sou";
   else if (!strcmp(palavra, "nao"))
      return "snao";
   else return "sidentificador";
}

static void gerencia_atribuicao(FILE *arquivo, char caracter, tokens **token){
   char *atribuicao = malloc(sizeof(char) *2);
   FILE *aux;
   *atribuicao = caracter;
   aux=arquivo;
   caracter = fgetc(aux);
   if(caracter =='='){
      atribuicao = realloc(atribuicao,(3));
      atribuicao[1] = caracter;
      atribuicao[2] = '\0';
      inserir_lista(token, atribuicao, "Satribuicao");
   }
   else{
      atribuicao[1] = '\0';
      inserir_lista(token, atribuicao, "Sdoispontos");
      fseek(arquivo, -1L, SEEK_CUR); 
   }
   free(atribuicao);
}
static void gerencia_opAritmetico(FILE *arquivo, char caracter,tokens **token){
    char *opAritmetico = malloc(sizeof(char)*2);
   *opAritmetico = caracter;
   
    if (caracter == '+'){
      opAritmetico[1]='\0';
      inserir_lista(token, opAritmetico, "Smais");
    }
   else if (caracter == '-'){
      opAritmetico[1]='\0';
      inserir_lista(token, opAritmetico, "Smenos");
   }
   else if (caracter == '*'){
      opAritmetico[1]='\0';
      inserir_lista(token, opAritmetico, "Smult");
   }
   
   free(opAritmetico);
}
static void gerencia_opRelacional(FILE *arquivo, char caracter,tokens **token)
{
   char *opRelacional = malloc(sizeof(char)*3);
   *opRelacional = caracter;
   char opAux;
   opAux = fgetc(arquivo);
   if(caracter == '>' && opAux == '='){
      opRelacional[1] = opAux;
      opRelacional[2] = '\0';
      inserir_lista(token, opRelacional, "Smaiorig");
   }else if(caracter == '<' && opAux == '='){
      opRelacional[1] = opAux;
      opRelacional[2] = '\0';
      inserir_lista(token, opRelacional, "Smenorig");
   }
   else if(caracter == '!' && opAux == '='){
      opRelacional[1] = opAux;
      opRelacional[2] = '\0';
      inserir_lista(token, opRelacional, "Sdif");
   }
      
   else if(caracter == '>'){
      opRelacional[1] = '\0';
      inserir_lista(token, opRelacional, "Smaior");
      fseek(arquivo, -1L, SEEK_CUR); 
   }
   else if(caracter == '<'){
      opRelacional[1] = '\0';
      inserir_lista(token, opRelacional, "Smenor");
      fseek(arquivo, -1L, SEEK_CUR); 
   }
   else if(caracter == '='){
      opRelacional[1] = '\0';
      inserir_lista(token, opRelacional, "Sig");
      fseek(arquivo, -1L, SEEK_CUR); 
   }
   else if(caracter == '!'){
      opRelacional[1] = '\0';
      inserir_lista(token, opRelacional, "Serro");
      fseek(arquivo, -1L, SEEK_CUR); 
   }

   free(opRelacional);
}
static void gerencia_pontuacao(FILE *arquivo, char caracter,tokens **token){
   char *pontucao = malloc(sizeof(char)*2);
   *pontucao = caracter;
   
    if (caracter == ';'){
      pontucao[1]='\0';
      inserir_lista(token, pontucao, "Sponto_virgula");
    }
   else if (caracter == '.'){
      pontucao[1]='\0';
      inserir_lista(token,pontucao, "Sponto");
   }
   else if (caracter == '('){
      pontucao[1]='\0';
      inserir_lista(token, pontucao, "Sabre_parenteses");
   }
   else if (caracter == ')'){
      pontucao[1]='\0';
      inserir_lista(token, pontucao, "Sfecha_parenteses");
   }
   else if (caracter == ','){
      pontucao[1]='\0';
      inserir_lista(token, pontucao, "Svirgula");
   }
   
   free(pontucao);
}


void inserir_lista(tokens **lista, char lexema[], char simbolo[]) {
   
   tokens *arquivo;
   arquivo = *lista;
   int lex_tam = strlen(lexema);
   int sim_tam = strlen(simbolo);
   
   tokens *novo = (tokens *)malloc((sizeof(tokens *)) + lex_tam + sim_tam); // cria um novo nó
   novo->lexema = (char *)malloc(sizeof(lex_tam));
   sprintf(novo->lexema,"%s",lexema);
   novo->simbolo = simbolo;
   novo->prox = NULL;
   
   if(arquivo == NULL){ // Lista vazia
      *lista = novo;  
   } 
   else // Lista não vazia
   { 
      while(arquivo->prox != NULL){
         arquivo = arquivo->prox;
      }
      arquivo->prox = novo;
   }
    
}
void imprimir_lista(tokens *lista) {
   
   tokens *arquivo;
   arquivo = lista;
   while(arquivo != NULL){
      printf("%s  | %s\n", arquivo->lexema, arquivo->simbolo);
      arquivo = arquivo->prox;
   }
   printf("\n\n");
}

