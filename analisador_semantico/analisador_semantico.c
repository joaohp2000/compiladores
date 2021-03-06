#include "analisador_semantico.h"
#include "../tabela_simbolos/tabela.h"
#include "../analisador_sintatico/analisador_sintatico.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
// CONVERTER APONTAMENTO PARA ALOCAÇÃO
// C program to convert infix expression to postfix
//Stack *pilha= NULL;
static tokens *var;
static tokens *expressao;
static tokens *fim_expressao;
static char *element[] = {"sbooleano", "sinteiro", "W", "+u", "-u", "sunario"};
void salva_var(tokens *token)
{
  var = token;
}
void salva_expressao(tokens *token)
{
  expressao = token;
}
void _fim_expressao(tokens *token)
{
  fim_expressao = token;
}
void imprimi_lista(Stack *lista);
void fecha_parentes(Stack **pilha, Stack **lista);
void operadores(Stack **pilha, Stack **lista, tokens *token);
int prioridade(tokens *token, char *operador);
Stack *valida_tipo(Stack *fat1, Stack *fat2, Stack *op);
int valida_expressao(Stack *lista);
int tabela_tipos(Stack *fat1, Stack *fat2, Stack *op);
void unario(tokens *token, Stack **pilha, Stack **lista);
Stack *copia_lista(Stack *lista, int count);

void unario(tokens *token, Stack **pilha, Stack **lista)
{
  tokens *novo;
  novo = (tokens *)malloc(sizeof(tokens));
  if (cp(&token, "smais"))
  {
    novo->lexema = element[3];
  }
  else
  {
    novo->lexema = element[4];
  }
  novo->simbolo = element[5];
  operadores(pilha, lista, novo);
}
// Transformar de infixa para posfixa
Stack *In2Pos()
{
  tokens *token = expressao;
  Stack *pilha = NULL;
  Stack *lista = NULL;
  //printf("%d", &op_tipo);
  do
  {

    if (cp(&token, "sabre_parenteses"))
    {
      push_stack(&pilha, token);
      if (cp(&(token->prox), "smais") || cp(&(token->prox), "smenos"))
      {
        unario(token->prox, &pilha, &lista);
        lx(&token);
      }
    }
    else
    {
      if (cp(&token, "sidentificador") || cp(&token, "snumero"))
      {
        insere_lista(&lista, token_to_stack(token));
      }
      else
      {
        if (cp(&token, "sfecha_parenteses"))
        {
          fecha_parentes(&pilha, &lista);
        }
        else
        {

          operadores(&pilha, &lista, token);
        }
      }
    }
    lx(&token);
  } while (token != fim_expressao);
  if (pilha != NULL)
  {
    do
    {
      insere_lista(&lista, stack_pop(&pilha));
    } while (pilha != NULL);
  }
  imprimi_lista(lista);
  return lista;
}
void operadores(Stack **pilha, Stack **lista, tokens *token)
{
  if ((*pilha) == NULL)
  {
    push_stack(pilha, token);
  }
  else
  {
    if (!strcmp(((*pilha)->lexema), "("))
    {
      push_stack(pilha, token);
    }
    else
    {
      if (prioridade(token, (*pilha)->simbolo))
      {
        insere_lista(lista, stack_pop(pilha));
        operadores(pilha, lista, token);
      }
      else
      {
        push_stack(pilha, token);
      }
    }
  }
}
void fecha_parentes(Stack **pilha, Stack **lista)
{
  if (!strcmp((*pilha)->lexema, "("))
  {
    (*pilha) = (*pilha)->prox;
  }
  else
  {
    insere_lista(lista, stack_pop(pilha));
    fecha_parentes(pilha, lista);
  }
}

//Função auxiliar prioridade
int prioridade(tokens *token, char *operador)
{
  int pc, pt;

  if (cp(&token, "sou"))
    pc = 6;
  else if (cp(&token, "se"))
    pc = 5;
  else if (cp(&token, "snao"))
    pc = 4;
  else if (cp(&token, "smaior") || cp(&token, "smenor") || cp(&token, "smaiorig") || cp(&token, "smenorig") || cp(&token, "sig") || cp(&token, "sdif"))
    pc = 3;
  else if (cp(&token, "smenos") || cp(&token, "smais"))
    pc = 2;
  else if (cp(&token, "smult") || cp(&token, "sdiv"))
    pc = 1;
  else if (cp(&token, "sunario"))
    pc = 0;

  if (!strcmp(operador, "sou"))
    pt = 6;
  else if (!strcmp(operador, "se"))
    pt = 5;
  else if (!strcmp(operador, "snao"))
    pt = 4;
  else if (!strcmp(operador, "smaior") || !strcmp(operador, "smenor") || !strcmp(operador, "smaiorig") || !strcmp(operador, "smenorig") || !strcmp(operador, "sig") || !strcmp(operador, "sdif"))
    pt = 3;
  else if (!strcmp(operador, "smenos") || !strcmp(operador, "smais"))
    pt = 2;
  else if (!strcmp(operador, "smult") || !strcmp(operador, "sdiv"))
    pt = 1;
  else if (!strcmp(operador, "sunario"))
    pt = 0;
  if (pc >= pt)
    return 1;
  return 0;
}

Stack *nova_celula()
{
  Stack *novo;
  novo = (Stack *)malloc(sizeof(Stack));
  return novo;
}

Stack *stack_pop(Stack **_pilha)
{
  Stack *aux = *_pilha;
  if (!stack_vazia(aux))
  {
    *_pilha = aux->prox;
  }
  return aux;
}

int stack_vazia(Stack *_pilha)
{
  if (_pilha != NULL)
    return 0;
  return 1;
}

void push_stack(Stack **pri, tokens *token)
{
  Stack *p, *novo = NULL;
  if (!strcmp(token->simbolo, "sidentificador"))
  {
    registro *aux;
    aux = pesquisa_tabela(tabela, token);
    novo->tipo = aux->conteudo.tipo;
  }

  p = *pri;
  novo = nova_celula();
  novo->lexema = token->lexema;
  novo->simbolo = token->simbolo;

  novo->prox = p;
  *pri = novo;
}

void insere_lista(Stack **lista, Stack *cel)
{
  Stack *aux = *lista;
  cel->prox = NULL;
  if (aux == NULL)
  { // Lista vazia
    aux = cel;
    *lista = aux;
  }
  else // Lista não vazia
  {
    while (aux->prox != NULL)
    {
      aux = aux->prox;
    }
    aux->prox = cel;
  }
}

/*
void insere_lista(Stack **lista, Stack *cel){
  static int k = 0;
  Stack *_lista=*lista;
  if(k==0) _lista =(Stack *) malloc(sizeof(Stack));
  else _lista = (Stack *)realloc(_lista, (k+1)*sizeof(Stack));

  _lista[k].lexema = cel->lexema;
  _lista[k].simbolo = cel->simbolo;
  _lista[k].tipo = cel->tipo;
  _lista[k].prox = NULL;
 *lista = _lista;
  k++;
}*/
void libera_lista(Stack *lista)
{
  Stack *aux;
  while (!stack_vazia(lista))
  {
    aux = lista;
    lista = lista->prox;
    free(aux);
  }
}

Stack *token_to_stack(tokens *token)
{
  Stack *novo;
  novo = nova_celula();
  if (!strcmp(token->simbolo, "sidentificador") || !strcmp(token->simbolo, "snumero"))
  {
    if (!strcmp(token->simbolo, "snumero"))
    {
      novo->tipo = element[1];
    }
    else
    {
      registro *aux;
      aux = pesquisa_tabela(tabela, token);
      novo->tipo = aux->conteudo.tipo;
    }
  }
  novo->lexema = token->lexema;
  novo->simbolo = token->simbolo;
  novo->prox = NULL;
  return novo;
}
void imprimi_lista(Stack *lista)

{

  Stack *arquivo;
  arquivo = lista;
  while (arquivo != NULL)
  {
    printf("%s ", arquivo->lexema);
    arquivo = arquivo->prox;
  }
  /*for(int i=0;i<count;i++){
    printf("%s ", arquivo[i].lexema);
  }*/
  printf("\nfim\n");
}

int valida_expressao(Stack *lista)
{

  int count = 0;
  if (strcmp(lista->simbolo, "sidentificador") && strcmp(lista->simbolo, "snumero"))
  {
    if (!strcmp(lista->simbolo, "sunario") || !strcmp(lista->simbolo, "snao"))
      return 0;
    else
      return 1;
  }
  else
  {
    if (lista->prox != NULL)
    {
      count = valida_expressao(lista->prox);
      while (lista->prox != NULL)
      {
        if (count != 0)
        {
          count--;
          return count;
        }
        else
        {
          if (!strcmp((lista->prox)->simbolo, "sunario") || !strcmp((lista->prox)->simbolo, "snao"))
            lista = valida_tipo(lista, NULL, (lista->prox));
          else
            lista = valida_tipo(lista, lista->prox, (lista->prox)->prox);
          if (lista->prox == NULL)
            return 1;
          if (lista == NULL)
            return 0;
          count = valida_expressao(lista->prox);
        }
      }
    }
    else
      return 1;
  }
}
Stack *valida_tipo(Stack *fat1, Stack *fat2, Stack *op)
{
  int qt;
  qt = tabela_tipos(fat1, fat2, op);
  if (qt == 1)
    fat1->tipo = element[1];
  else if (qt == 2)
    fat1->tipo = element[0];
  else
  {
    printf("Expressão invalida! Linha: %d", expressao->_linha);
    exit(0);
  }
  fat1->prox = op->prox;
  fat1->lexema = element[2];
  return fat1;
}
int tabela_tipos(Stack *fat1, Stack *fat2, Stack *op)
{
  short int retorna;
  if (!strcmp(op->simbolo, "smenos") || !strcmp(op->simbolo, "smais") || !strcmp(op->simbolo, "smult") || !strcmp(op->simbolo, "sdiv"))
  {
    if (!strcmp(fat1->tipo, "sinteiro") && !strcmp(fat2->tipo, "sinteiro"))
      retorna = 1;
    else
      retorna = 0;
  }
  else
  {
    if (!strcmp(op->simbolo, "smaior") || !strcmp(op->simbolo, "smenor") || !strcmp(op->simbolo, "smaiorig") || !strcmp(op->simbolo, "smenorig") || !strcmp(op->simbolo, "sig") || !strcmp(op->simbolo, "sdif"))
    {
      if (!strcmp(fat1->tipo, "sinteiro") && !strcmp(fat2->tipo, "sinteiro"))
        retorna = 2;
      else
        retorna = 0;
    }
    else
    {
      if (!strcmp(op->simbolo, "se") || !strcmp(op->simbolo, "sou"))
      {
        if (!strcmp(fat1->tipo, "sbooleano") && !strcmp(fat2->tipo, "sbooleano"))
          retorna = 2;
        else
          retorna = 0;
      }
      else
      {
        if (!strcmp(op->simbolo, "snao"))
        {
          if (!strcmp(fat1->tipo, "sbooleano"))
            retorna = 2;
          else
            retorna = 0;
        }
        else
        {
          if (!strcmp(op->simbolo, "sunario"))
          {
            if (!strcmp(fat1->tipo, "sinteiro"))
              retorna = 1;
            else
              retorna = 0;
          }
        }
      }
    }
  }
  return retorna;
}

int valida_atribuicao(Stack *lista)
{
  int rt = 0;
  registro *aux = pesquisa_tabela(tabela, var);
  if (valida_expressao(lista))
  {
    if (!strcmp(lista->tipo, "func_sinteiro") || !strcmp(lista->tipo, "func_sbooleano"))
    {
      if (!strcmp(lista->tipo, "func_sinteiro") && !strcmp(aux->conteudo.tipo, "sinteiro"))
      {
        rt = 1;
      }
      else
      {
        if (!strcmp(lista->tipo, "func_sbooleano") && !strcmp(aux->conteudo.tipo, "sbooleano"))
        {
          rt = 1;
        }
        else
        {
          printf("(linha : %d)Atribuição de Tipos diferentes. Varivel %s ", var->_linha, var->lexema);
          rt = 0;
          exit(0);
        }
      }
    }
    else
    {

      if (!strcmp((aux->conteudo.tipo), "func_sinteiro") || !strcmp((aux->conteudo.tipo), "func_sbooleano"))
      {
        if (!strcmp((aux->conteudo.tipo), "func_sinteiro") && !strcmp(lista->tipo, "sinteiro"))
        {
          rt = 1;
        }
        else
        {
          if (!strcmp((aux->conteudo.tipo), "func_sbooleano") && !strcmp(lista->tipo, "sbooleano"))
          {
            rt = 1;
          }
          else
          {
            printf("(linha : %d)Atribuição de Tipos diferentes. Varivel %s ", var->_linha, var->lexema);
            rt = 0;
            exit(0);
          }
        }
      }
      else
      {
        if (strcmp(lista->tipo, (pesquisa_tabela(tabela, var)->conteudo.tipo)))
        {
          printf("(linha : %d)Atribuição de Tipos diferentes. Varivel %s ", var->_linha, var->lexema);
          rt = 0;
          exit(0);
        }
        else
          rt = 1;
      }
    }
  }
  return rt;
}