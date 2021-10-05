#include "../analisador_lexico/analisador_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_sintatico.h"
/*Def rotulo inteiro
início
 rotulo:= 1
 Léxico(token)
 se token.simbolo = sprograma
 então início
 Léxico(token)
 se token.simbolo = sidentificador
 então início
 insere_tabela(token.lexema,”nomedeprograma”,””,””)
 Léxico(token)
 se token.simbolo = spontovirgula
 então início
 analisa_bloco
se token.simbolo = sponto
 então se acabou arquivo ou é comentário
 então sucesso
senão ERRO
 senão ERRO
 fim
 senão ERRO
 fim
 senão ERRO
 fim
 senão ERRO
fim.*/

void error(tokens *token)
{
   printf("%s error", token->simbolo);
}
void analisador_sintatico(tokens *lista)
{

   tokens *token = lista;
   if (cp(token, "sprograma"))
   {
      lx(&token);
      if (cp(token, "sidentificador"))
      {
         // INSERE TABELA
         lx(&token);
         if (cp(token, "spontovirgula"))
         {
            // ANALISA_BLOCO()
            if (cp(token, "sponto"))
            {
               // FIM
               printf("sucesso");
            }
            else
            {
               error(token);
            }
         }
         else
         {
            error(token);
         }
      }
      else
      {
         error(token);
      }
   }
   else
   {
      error(token);
   }
}

void analisa_bloco(tokens *lista)
{
   lx(&lista);
   analisa_et_variaveis(lista);
   analisa_subrotinas(lista);
   analisa_comandos(lista);
}

void analisa_et_variaveis(tokens *token)
{
   if (cp(token, "svar"))
   {
      lx(&token);
      if (cp(token, "sidentificador"))
      {
         while (cp(token, "sidentificador"))
         {
            analisa_variaveis(token);
            if (cp(token, "spontovirg"))
            {
               lx(&token);
            }
            else
            {
               error(token);
            }
         }
      }
   }
}
void analisa_variaveis(tokens *token)
{
   while (!cp(token, "sdoispontos"))
   {
      if (cp(token, "sidentificador"))
      {
         lx(&token);
         if (cp(token, "svirgula") || cp(token, "sdoispontos"))
         {
            if (cp(token, "svirgula"))
            {
               lx(&token);
               if (cp(token, "sdoispontos"))
                  error(token);
            }
         }
         else
            error(token);
      }
      else
         error(token);
   }
   analisa_tipo(token);
}

void analisa_tipo(tokens *token)
{
   if (!cp(token, "sinteiro") && !cp(token, "sbooleano"))
   {
      error(token);
   }
   else
   {
      // COLOCA_TIPO_TABELA(TOKEN.LEXEMA)
   }
   lx(&token);
}

/*void analisa_subrotinas(tokens *token){
   //
}*/

void analisa_comandos(tokens *token)
{
   if (cp(token, "sinicio"))
   {
      lx(&token);
      analisa_comando_simples(token);
      while (!cp(token, "sfim"))
      {
         if (cp(token, "spontovirgula"))
         {
            lx(&token);
            if (cp(token, "sfim"))
            {
               analisa_comando_simples(token);
            }
         }
         else
            error(token);
      }
      lx(&token);
   }
   else
      error(token);
}

void analisa_comando_simples(tokens *token)
{
   if (cp(token, "sidentificador"))
   {
      analisa_atrib_chprocedimento(token);
   }
   else if (cp(token, "sse"))
   {
      analisa_se(token);
   }
   else if (cp(token, "senquanto"))
   {
      analisa_enquanto(token);
   }
   else if (cp(token, "sleia"))
   {
      analisa_leia(token);
   }
   else if (cp(token, "sescreva"))
   {
      analisa_escreva(token);
   }
   else
   {
      analisa_comandos(token);
   }
}

void analisa_atrib_chprocedimento(tokens *token)
{
   lx(&token);
   if (cp(token, "satribuicao"))
   {
      //analisa_atribuicao(token);  chama na verdade expressao
   }
   else
   {
      //chamada_procedimento(token); --------------------------->fazer
   }
}

void analisa_escreva(tokens *token)
{
   lx(&token);
   if (cp(token, "sabre_parenteses"))
   {
      lx(&token);
      if (cp(token, "sidentificador"))
      {
         //if (pesquisa_declvarfunc_tabela(token->lexema))
         //{
            lx(&token);
            if (cp(token, "sfecha_parenteses"))
            {
               lx(&token);
            }
            else
               error(token);
        // }
         //else
           // error(token);
      }
      else
         error(token);
   }
   else
      error(token);
}

void analisa_leia(tokens *token)
{
   lx(&token);
   if (cp(token, "sabre_parenteses"))
   {
      lx(&token);
      if (cp(token, "sidentificador"))
      {
         //if( pesquisa_declvar_tabela(token.lexema)){
         lx(&token);
         if(cp(token, "sfecha_parenteses")) lx(&token);
         else error(token);
         //}
      }
      else error(token);
   }
   else error(token);
}

void analisa_enquanto(tokens *token)
{
   // Def auxrot1,auxrot2 inteiro
   // auxrot1:= rotulo
   // Gera(rotulo,NULL,´ ´,´ ´) {início do while}
   // rotulo:= rotulo+1
   lx(&token);
   analisa_expressao(token);
   if (cp(token, "sfaca")){
      // auxrot2:= rotulo
      // Gera(´ ´,JMPF,rotulo,´ ´) {salta se falso}
      // rotulo:= rotulo+1
      lx(&token);
      analisa_comando_simples(token);
      // Gera(´ ´,JMP,auxrot1,´ ´) {retorna início loop}
      // Gera(auxrot2,NULL,´ ´,´ ´) {fim do while}
   }else error(token);
}

void analisa_se(tokens *token){
   lx(&token);
   analisa_expressao(token);
   if (cp(token, "sentao")){
   lx(&token);
   analisa_comando_simples(token);
      if (cp(token, "ssenao")){
         lx(&token);
         analisa_comando_simples(token);
      }
   }else error(token);
}
void analisa_subrotinas(tokens *token){
   int flag = 0;
   if(cp(token,"sprocedimento") || cp(token, "sfuncao")){
      // faz nada por enquanto
   }
   while(cp(token,"sprocedimento") || cp(token, "sfuncao")){
      if(cp(token,"sprocedimento")){
         analisa_declaracao_procedimento(token);
      }
      else{
         //analisa_declaracao_funcao(token);
      }
      if(cp(token, "sponto_virgula")){
         lx(&token);
      }
      else error(token);
   }
   if(flag ==1) {
      //faz algo
   }
   
}

void analisa_declaracao_procedimento(tokens *token){
   lx(&token);
   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador")){
   // pesquisa_declproc_tabela(token.lexema)
   // se não encontrou
   // então início
   // Insere_tabela(token.lexema,”procedimento”,nível, rótulo)
   // {guarda na TabSimb}
   // Gera(rotulo,NULL,´ ´,´ ´)
   // {CALL irá buscar este rótulo na TabSimb}
   // rotulo:= rotulo+1
      lx(&token);
      if (cp(token, "sponto_vírgula")){
         analisa_bloco(token);
      }else error(token);
   // }else error(token);
   }else error(token);
}

void analisa_declaracao_funcao(tokens *token){
   lx(&token);
   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador")){
   // pesquisa_declproc_tabela(token.lexema)
   // se não encontrou
   // então início
   // Insere_tabela(token.lexema,”procedimento”,nível, rótulo)
   // {guarda na TabSimb}
   // Gera(rotulo,NULL,´ ´,´ ´)
   // {CALL irá buscar este rótulo na TabSimb}
   // rotulo:= rotulo+1
      lx(&token);
      if (cp(token, "sdoispontos")){
         lx(&token);
         if(cp(token, "sinteiro") || cp(token, "sbooleano")){
            lx(&token);
            if(cp(token, "sponto_virgula")){
               analisa_bloco(token);
            }
         }else(token);
      }else error(token);
   // }else error(token);
   }else error(token);
}

void analisa_expressao(tokens *token){
   analisa_expressao_simples(token);
   if (cp(token, "smaior") || cp(token, "smaiorig") || cp(token, "sig") || cp(token, "smenor") || cp(token, "smenorig") || cp(token, "sdif")){
      lx(&token);
      analisa_expressao_simples(token);
   }
}

void analisa_expressao_simples(tokens *token){
   if (cp(token, "smais") || cp(token, "smenos")){
      lx(&token);
      analisa_termo(token);
      while(cp(token, "smais") || cp(token, "smenos") || cp(token, "sou")){
         lx(&token);
         analisa_termo(token);
      }
   }
}

void analisa_termo(tokens *token){
   analisa_fator(token);
   while(cp(token, "smult") || cp(token, "sdiv") || cp(token, "se")){
      lx(&token);
      analisa_termo(token);
   }
}

void analisa_fator(tokens *token){
   if (cp(token, "sidentificador")){
      // Se pesquisa_tabela(token.lexema,nível,ind)
      // Então Se (TabSimb[ind].tipo = “função inteiro”) ou
      // (TabSimb[ind].tipo = “função booleano”)
      // Então
     // analisa_chamada_funcao(token); --------------------------> fazer
      // Senão Léxico(token)
      // Senão ERRO
   }else if (cp(token, "snumero")){
      lx(&token);
   }else if (cp(token, "snao")){
      lx(&token);
      analisa_fator(token);
   }else if (cp(token, "sabre_parenteses")){
      lx(&token);
      analisa_expressao(token);
      if (cp(token, "sfecha_parenteses")){
         lx(&token);
      }else error(token);
   }else if (cp(token, "verdadeiro") || cp(token, "falso")){
      lx(&token);
   }else error(token);
}

int cp(tokens *token, char *simbolo)
{
   if (!strcmp(token->simbolo, simbolo))
      return 1;
   else
      return 0;
}

void lx(tokens **token)
{
   *token = (*token)->prox;
}
