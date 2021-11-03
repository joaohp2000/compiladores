#include "../analisador_lexico/analisador_lexico.h"
#include "../tabela_simbolos/tabela.h"
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
tokens **token =NULL;
char *erros[] = {"Deve iniciar com 'programa'", //0
                 "'Programa' deve ser sucedido por um nome", //1
                 "Falta ponto e virgula", //2
                 "Falta o ponto", //3
                 "Variavel Sem nome",  //4 
                 "Variavel duplicada", // 5
                 "O simbolo apos a variavel é invalido", //6
                 " ':' invalido", //7
                 "Tipo invalido", //8
                 "O procedimento ou função ja existe",//9
                 "Procedimento/função nao possui nome",//10
                 ": esperado", //11
                 "Espera-se 'inicio'",//12
                 "Espera-se 'entao'",//13
                 "Espera-se 'faca'",//14
                 "Espera-se fechar parenteses", //15
                 "Expressão inválida", //16
                 "Variavel invalida ou inexistente", //17
                 "Variavel não declarada", //18
                 "Espera-se abertura de parenteses",//19
                 "Variavel ou função não declarada"//20
                 };
void error(tokens **token, int num_erro)
{
   tokens *aux = *token;
   printf("%s\n", erros[num_erro]);
   printf("linha %d: %s error\n", aux->_linha, aux->lexema);
   exit(-1);
}


void analisador_sintatico(tokens *lista)
{
   tokens *token = lista;
   if (cp(&token, "sprograma"))
   {
      lx(&token);
      if (cp(&token, "sidentificador"))
      {
         insere_tabela(&tabela, token, 1, NULL);
         lx(&token);
         if (cp(&token, "sponto_virgula"))
         {
            analisa_bloco(&token);
            if (token!=NULL)
            {
               // FIM
               if (cp(&token, "sponto") && token->prox ==NULL){
                  printf("sucesso\n");
               }
               else{
                  printf("O programa deve acabar com o ponto");
                  exit(-1);
               }
               
            }
            else
            {
               printf("Esperado um ponto");
               exit(-1);
            }
         }
         else
         {
            error(&token, 2);
         }
      }
      else
      {
         error(&token, 1);
      }
   }
   else
   {
      error(&token, 0);
   }
}

void analisa_bloco(tokens **lista)
{
   lx(lista);
   analisa_et_variaveis(lista);
   analisa_subrotinas(lista);
   analisa_comandos(lista);
}

void analisa_et_variaveis(tokens **token)
{
   if (cp(token, "svar"))
   {
      lx(token);
      if (cp(token, "sidentificador"))
      {
         while (cp(token, "sidentificador"))
         {
            analisa_variaveis(token);
            if (cp(token, "sponto_virgula"))
            {
               lx(token);
            }
            else
            {
               error(token, 2);
            }
         }
      }
      else
      {
         error(token, 4);
      }
   }
}
void analisa_variaveis(tokens **token)
{
   while (!cp(token, "sdoispontos"))
   {
      if (cp(token, "sidentificador"))
      {
          if (busca_duplicatas(tabela, *token))
          {
            insere_tabela(&tabela, *token,0,NULL);
            lx(token);
            
            if (cp(token, "svirgula") || cp(token, "sdoispontos"))
            {
               if (cp(token, "svirgula"))
               {
                  lx(token);
                  if (cp(token, "sdoispontos"))
                     error(token,7);
               }
            }
            else
               error(token,6);
            }
            else
               error(token,5);
      }
      else
         error(token, 4);
   }
   lx(token);
   analisa_tipo(token);
}

void analisa_tipo(tokens **token)
{
   if (!cp(token, "sinteiro") && !cp(token, "sbooleano"))
   {
      error(token,8);
   }
   else
   {
      insere_tipo(tabela, *token, 0);
      // COLOCA_TIPO_TABELA(TOKEN.LEXEMA)
   }
   lx(token);
}

/*void analisa_subrotinas(tokens **token){
   //
}*/

void analisa_comandos(tokens **token)
{
   if (cp(token, "sinicio"))
   {
      lx(token);
      analisa_comando_simples(token);
      while (!cp(token, "sfim"))
      {
         if (cp(token, "sponto_virgula"))
         {
            lx(token);
            if (!cp(token, "sfim"))
            {
               analisa_comando_simples(token);
            }
         }
         else
            error(token, 2);
      }
      lx(token);
   }
   else
      error(token,12);
}

void analisa_comando_simples(tokens **token)
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

void analisa_atrib_chprocedimento(tokens **token)
{
   lx(token);
   if (cp(token, "satribuicao"))
   {
      // analisa atribuição //// analisa_expressao(token);
      lx(token);
      analisa_expressao(token);
   }
   else
   {
      // Chamada_procedimento

   }
}

void analisa_escreva(tokens **token)
{
   lx(token);
   if (cp(token, "sabre_parenteses"))
   {
      lx(token);
      if (cp(token, "sidentificador"))
      {
         if( !busca_incidente(*token, tabela))
         {
         lx(token);
         if (cp(token, "sfecha_parenteses"))
         {
            lx(token);
         }
         else
            error(token,15);
         }
         else
          error(token,18);
      }
      else
         error(token,17);
   }
   else
      error(token,19);
}

void analisa_leia(tokens **token)
{
   lx(token);
   if (cp(token, "sabre_parenteses"))
   {
      lx(token);
      if (cp(token, "sidentificador"))
      {
          if( !busca_incidente(*token, tabela)){
         lx(token);
         if (cp(token, "sfecha_parenteses"))
            lx(token);
         else
            error(token, 15);
         }
         else error(token, 18);
      }
      else
        error(token,17);
   }
   else
      error(token,19);
}

void analisa_enquanto(tokens **token)
{
   // Def auxrot1,auxrot2 inteiro
   // auxrot1:= rotulo
   // Gera(rotulo,NULL,´ ´,´ ´) {início do while}
   // rotulo:= rotulo+1
   lx(token);
   analisa_expressao(token);
   if (cp(token, "sfaca"))
   {
      // auxrot2:= rotulo
      // Gera(´ ´,JMPF,rotulo,´ ´) {salta se falso}
      // rotulo:= rotulo+1
      lx(token);
      analisa_comando_simples(token);
      // Gera(´ ´,JMP,auxrot1,´ ´) {retorna início loop}
      // Gera(auxrot2,NULL,´ ´,´ ´) {fim do while}
   }
   else
      error(token,14);
}

void analisa_se(tokens **token)
{
   lx(token);
   analisa_expressao(token);
   if (cp(token, "sentao"))
   {
      lx(token);
      analisa_comando_simples(token);
      if (cp(token, "ssenao"))
      {
         lx(token);
         analisa_comando_simples(token);
      }
   }
   else
      error(token,13);
}
void analisa_subrotinas(tokens **token)
{
   int flag = 0;
   if (cp(token, "sprocedimento") || cp(token, "sfuncao"))
   {
      // faz nada por enquanto
   }
   while (cp(token, "sprocedimento") || cp(token, "sfuncao"))
   {
      if (cp(token, "sprocedimento"))
      {
         analisa_declaracao_procedimento(token);
      }
      else
      {
         analisa_declaracao_funcao(token);
      }
      if (cp(token, "sponto_virgula"))
      {
         lx(token);
      }
      else
         error(token,2);
   }
   if (flag == 1)
   {
      //faz algo
   }
}

void analisa_declaracao_procedimento(tokens **token)
{
   lx(token);
   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador")) //nome do procedimento
   {
      if(busca_incidente(*token, tabela)){
         insere_tabela(&tabela,*token,1,NULL);
         lx(token);
         if (cp(token, "sponto_virgula"))
         {
            analisa_bloco(token);
         }
         else
            error(token,2);
      }
      else{ 
         error(token,9);
      }
   }
   else{
      error(token,10);
   }

}

void analisa_declaracao_funcao(tokens **token)
{
   lx(token);
   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador"))
   {
       if(busca_incidente(*token, tabela)){
         insere_tabela(&tabela,*token,1,NULL);
         lx(token);
         if (cp(token, "sdoispontos"))
         {
            lx(token);
            if (cp(token, "sinteiro") || cp(token, "sbooleano"))
            {
               insere_tipo(tabela, *token, 1);
               lx(token);
               if (cp(token, "sponto_virgula"))
               {
                  analisa_bloco(token);
               }
            }
            else
               error(token,8);
         }
         else
            error(token, 11);
         // }else error(token);
      }
      else{
         error(token,9);
      }
   }
   else
      error(token,10);
}

void analisa_expressao(tokens **token)
{
   analisa_expressao_simples(token);
   if (cp(token, "smaior") || cp(token, "smaiorig") || cp(token, "sig") || cp(token, "smenor") || cp(token, "smenorig") || cp(token, "sdif"))
   {
      lx(token);
      analisa_expressao_simples(token);
   }
}

void analisa_expressao_simples(tokens **token)
{
   if (cp(token, "smais") || cp(token, "smenos"))
   {
      lx(token);
   }
      analisa_termo(token);
   
      while (cp(token, "smais") || cp(token, "smenos") || cp(token, "sou"))
      {
         lx(token);
         analisa_termo(token);
      }
   

}

void analisa_termo(tokens **token)
{
   analisa_fator(token);
   while (cp(token, "smult") || cp(token, "sdiv") || cp(token, "se"))
   {
      lx(token);
      analisa_fator(token);
   }
}

void analisa_fator(tokens **token)
{
   if (cp(token, "sidentificador"))
   {
      // Se pesquisa_tabela(token.lexema,nível,ind)
      // Então Se (TabSimb[ind].tipo = “função inteiro”) ou
      // (TabSimb[ind].tipo = “função booleano”)
      // Então
      registro * aux_cel;
      if ((aux_cel = pesquisa_tabela(tabela, *token)) != NULL){
         if(!strcmp(aux_cel->conteudo.tipo, "func_sinterio") || !strcmp(aux_cel->conteudo.tipo, "func_sbooleano")){
            lx(token); // Analisa chamada de função
         }
         else{
            lx(token); 
         }
      }
      else{
         error(token,20);
      }

   }
   else if (cp(token, "snumero"))
   {
      lx(token);
   }
   else if (cp(token, "snao"))
   {
      lx(token);
      analisa_fator(token);
   }
   else if (cp(token, "sabre_parenteses"))
   {
      lx(token);
      analisa_expressao(token);
      if (cp(token, "sfecha_parenteses"))
      {
         lx(token);
      }
      else
         error(token, 15);
   }
   else if (cp(token, "verdadeiro") || cp(token, "falso"))
   {
      lx(token);
   }
   else
      error(token,16);
}

void analisa_chamada_procedimento(tokens **token)
{
}

int cp(tokens **token, char *simbolo)
{
   tokens *aux =*token;
   if (!strcmp(aux->simbolo, simbolo))
      return 1;
   else
      return 0;
}

void lx(tokens **token)
{
   
   *token = (*token)->prox;
}
