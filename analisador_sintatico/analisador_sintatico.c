#include "../analisador_lexico/analisador_lexico.h"
#include "../tabela_simbolos/tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_sintatico.h"
#include "../analisador_semantico/analisador_semantico.h"
#include <unistd.h>
#include <sys/wait.h>
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
tokens **token = NULL;
int rotulo, var_mem = 0, num_var = 0;
char svar_mem[5], snum_var[5];
char *obj_file;
char *erros[] = {
    "Deve iniciar com 'programa'",                     //0
    "'Programa' deve ser sucedido por um nome",        //1
    "Falta ponto e virgula",                           //2
    "Falta o ponto",                                   //3
    "Variavel Sem nome",                               //4
    "Variavel duplicada",                              //5
    "O simbolo apos a variavel é invalido",            //6
    " ':' invalido",                                   //7
    "Tipo invalido",                                   //8
    "O procedimento ou função ja existe",              //9
    "Procedimento/função nao possui nome",             //10
    ": esperado",                                      //11
    "Espera-se 'inicio'",                              //12
    "Espera-se 'entao'",                               //13
    "Espera-se 'faca'",                                //14
    "Espera-se fechar parenteses",                     //15
    "Expressão inválida",                              //16
    "Variavel invalida ou inexistente",                //17
    "Variavel não declarada",                          //18
    "Espera-se abertura de parenteses",                //19
    "Variavel ou função ou procedimento não declarada" //20
};
void error(tokens **token, int num_erro)
{
   tokens *aux = *token;
   printf("%s\n", erros[num_erro]);
   printf("linha %d: %s error\n", aux->_linha, aux->lexema);
   close_arquivo();
   remove(obj_file);
   exit(-1);
}
const char *int_to_string(int inteiro);
void analisador_sintatico(tokens *lista, char *nome_arquivo)
{
   tokens *token = lista;
   rotulo = 1;
   obj_file = nome_arquivo;
   inicializa_arquivo_objeto(nome_arquivo);
   if (cp(&token, "sprograma"))
   {
      gerador_codigo("", "START", "", "");
      lx(&token);
      if (cp(&token, "sidentificador"))
      {
         insere_tabela(&tabela, token, 1, NULL);
         gerador_codigo("", "ALLOC", "0", "1");
         num_var++;
         //rotulo++;
         lx(&token);
         if (cp(&token, "sponto_virgula"))
         {
            analisa_bloco(&token);
            if (token != NULL)
            {
               // FIM
               if (cp(&token, "sponto") && token->prox == NULL)
               {

                  var_mem = num_var;
                  num_var = 0;
                  sprintf(svar_mem, "%d", var_mem);
                  sprintf(snum_var, "%d", num_var);
                  gerador_codigo("", "DALLOC", snum_var, svar_mem);
                  gerador_codigo("", "HLT", "", "");

                  printf("sucesso\n");
               }
               else
               {
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
         var_mem++;
         if (busca_duplicatas(tabela, *token))
         {
            insere_tabela(&tabela, *token, 0, NULL);
            lx(token);

            if (cp(token, "svirgula") || cp(token, "sdoispontos"))
            {
               if (cp(token, "svirgula"))
               {
                  lx(token);
                  if (cp(token, "sdoispontos"))
                     error(token, 7);
               }
            }
            else
               error(token, 6);
         }
         else
            error(token, 5);
      }
      else
         error(token, 4);
   }
   // gerador_codigo("","ALLOC", var_mem, num_var);
   sprintf(svar_mem, "%d", var_mem);
   sprintf(snum_var, "%d", num_var);
   gerador_codigo("", "ALLOC", snum_var, svar_mem);
   num_var = num_var + var_mem;
   var_mem = 0;
   lx(token);
   analisa_tipo(token);
}

void analisa_tipo(tokens **token)
{
   if (!cp(token, "sinteiro") && !cp(token, "sbooleano"))
   {
      error(token, 8);
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
      error(token, 12);
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

   if (!busca_incidente(*token, tabela))
   {
      salva_var(*token);
      lx(token);
      if (cp(token, "satribuicao"))
      {
         // analisa atribuição //// analisa_expressao(token);
         Stack *saida;
         int retorno;
         lx(token);
         salva_expressao(*token);
         analisa_expressao(token);
         _fim_expressao(*token);
         saida = In2Pos();
         gerar_expressao(saida, num_var);
         retorno = valida_atribuicao(saida);
         gerar_atribuicao(num_var);
      }
      else
      {
         // Chamada_procedimento
         gerar_procedimento();
      }
   }
   else
   {
      error(token, 20);
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
         if (!busca_incidente(*token, tabela))
         {
            gerar_leia_escreve(num_var, *token, "LDV");
            gerador_codigo("", "PRN", "", "");
            lx(token);
            if (cp(token, "sfecha_parenteses"))
            {
               lx(token);
            }
            else
               error(token, 15);
         }
         else
            error(token, 18);
      }
      else
         error(token, 17);
   }
   else
      error(token, 19);
}

void analisa_leia(tokens **token)
{
   lx(token);
   gerador_codigo("", "RD", "", "");
   if (cp(token, "sabre_parenteses"))
   {
      lx(token);
      if (cp(token, "sidentificador"))
      {
         if (!busca_incidente(*token, tabela))
         {
            if (!strcmp((pesquisa_tabela(tabela, *token))->conteudo.tipo, "sinteiro"))
            {
               gerar_leia_escreve(num_var, *token, "STR");
               lx(token);
               if (cp(token, "sfecha_parenteses"))
                  lx(token);
               else
                  error(token, 15);
            }
            else
               error(token, 8);
         }
         else
            error(token, 18);
      }
      else
         error(token, 17);
   }
   else
      error(token, 19);
}

void analisa_enquanto(tokens **token)
{
   int auxrot1, auxrot2;
   // Def auxrot1,auxrot2 inteiro
   auxrot1 = rotulo;
   sprintf(snum_var, "%d", rotulo);
   gerador_codigo(snum_var, "NULL", "", "");
   // Gera(rotulo,NULL,´ ´,´ ´) {início do while}
   rotulo = rotulo + 1;
   lx(token);
   Stack *saida;
   salva_expressao(*token);
   analisa_expressao(token);
   _fim_expressao(*token);
   saida = In2Pos();
   gerar_expressao(saida, num_var);
   if (cp(token, "sfaca"))
   {
      auxrot2 = rotulo;
      sprintf(snum_var, "%d", rotulo);
      gerador_codigo("", "JMPF", snum_var, "");
      // Gera(´ ´,JMPF,rotulo,´ ´) {salta se falso}
      // rotulo:= rotulo+1
      rotulo = rotulo + 1;

      lx(token);
      analisa_comando_simples(token);
      sprintf(snum_var, "%d", auxrot1);
      gerador_codigo("", "JMP", snum_var, "");
      sprintf(snum_var, "%d", auxrot2);
      gerador_codigo(snum_var, "NULL", "", "");
      // Gera(´ ´,JMP,auxrot1,´ ´) {retorna início loop}
      // Gera(auxrot2,NULL,´ ´,´ ´) {fim do while}
   }
   else
      error(token, 14);
}

void analisa_se(tokens **token)
{
   int auxrot1, auxrot2;
   // Def auxrot1,auxrot2 inteiro

   lx(token);
   Stack *saida;
   salva_expressao(*token);
   analisa_expressao(token);
   _fim_expressao(*token);
   saida = In2Pos();
   gerar_expressao(saida, num_var);

   auxrot1 = rotulo;
   auxrot2 = rotulo;
   sprintf(snum_var, "%d", rotulo);
   gerador_codigo("", "JMPF", snum_var, "");
   rotulo = rotulo + 1;

   if (cp(token, "sentao"))
   {
      lx(token);

      analisa_comando_simples(token);

      if (cp(token, "ssenao"))
      {
         auxrot2 = rotulo;
         sprintf(snum_var, "%d", rotulo);
         gerador_codigo("", "JMP", snum_var, "");
         rotulo = rotulo + 1;

         sprintf(snum_var, "%d", auxrot1);
         gerador_codigo(snum_var, "NULL", "", "");
         auxrot1 = rotulo;
         lx(token);
         analisa_comando_simples(token);
      }
      sprintf(snum_var, "%d", auxrot2);
      gerador_codigo(snum_var, "NULL", "", "");
   }
   else
      error(token, 13);
}
void analisa_subrotinas(tokens **token)
{
   int flag = 0, auxrot;
   if (cp(token, "sprocedimento") || cp(token, "sfuncao"))
   {
      // faz nada por enquanto
      auxrot = rotulo;
      sprintf(snum_var, "%d", rotulo);
      gerador_codigo("", "JMP", snum_var, ""); //{Salta sub - rotinas}
      rotulo = rotulo + 1;
      flag = 1;
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
         error(token, 2);
   }
   if (flag == 1)
   {
      //faz algo
      sprintf(snum_var, "%d", auxrot);
      gerador_codigo(snum_var, "NULL", "", "");
   }
}

void analisa_declaracao_procedimento(tokens **token)
{
   lx(token);

   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador")) //nome do procedimento
   {
      if (busca_incidente(*token, tabela))
      {
         insere_tabela(&tabela, *token, 1, &rotulo);
         sprintf(snum_var, "%d", rotulo);
         gerador_codigo(snum_var, "NULL", "", "");

         rotulo++;
         lx(token);
         if (cp(token, "sponto_virgula"))
         {
            analisa_bloco(token);
         }
         else
            error(token, 2);
      }
      else
      {
         error(token, 9);
      }
   }
   else
   {
      error(token, 10);
   }
   var_mem = desempilha(&tabela);
   num_var = num_var - var_mem;
   if (var_mem != 0)
   {
      sprintf(svar_mem, "%d", var_mem);
      sprintf(snum_var, "%d", num_var);
      gerador_codigo("", "DALLOC", snum_var, svar_mem);
   }
   gerador_codigo("", "RETURN", "", "");
}

void analisa_declaracao_funcao(tokens **token)
{
   lx(token);

   //nível := “L” (marca ou novo galho)
   if (cp(token, "sidentificador"))
   {
      if (busca_incidente(*token, tabela))
      {
         insere_tabela(&tabela, *token, 1, &rotulo);
         sprintf(snum_var, "%d", rotulo);
         gerador_codigo(snum_var, "NULL", "", "");
         rotulo++;
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
               error(token, 8);
         }
         else
            error(token, 11);
         // }else error(token);
      }
      else
      {
         error(token, 9);
      }
   }
   else
      error(token, 10);
   var_mem = desempilha(&tabela);
   num_var = num_var - var_mem;
   if (var_mem != 0)
   {
      sprintf(svar_mem, "%d", var_mem);
      sprintf(snum_var, "%d", num_var);
      gerador_codigo("", "DALLOC", snum_var, svar_mem);
   }
   gerador_codigo("", "RETURN", "", "");
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
      registro *aux_cel;
      if ((aux_cel = pesquisa_tabela(tabela, *token)) != NULL)
      {
         if (!strcmp(aux_cel->conteudo.tipo, "func_sinterio") || !strcmp(aux_cel->conteudo.tipo, "func_sbooleano"))
         {
            lx(token); // Analisa chamada de função
         }
         else
         {
            lx(token);
         }
      }
      else
      {
         error(token, 20);
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
      error(token, 16);
}

void analisa_chamada_procedimento(tokens **token)
{
}

int cp(tokens **token, char *simbolo)
{
   tokens *aux = *token;
   if (!strcmp(aux->simbolo, simbolo))
      return 1;
   else
      return 0;
}

void lx(tokens **token)
{

   *token = (*token)->prox;
}
