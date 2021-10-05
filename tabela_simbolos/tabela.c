#include "tabela.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include "../analisador_lexico/analisador_lexico.h"


const char *tipo_null = "null";

registro *novo_registro()
{
    registro *novo;
    novo = (registro *)malloc(sizeof(registro));
    return novo;
}
void libera_mem(registro * element){
    free(element->conteudo.lexema);
    free(element);
}
void novo_elemento(elemento *x, char *lexema, char *tipo, u_int8_t escopo)
{
    int size;
    size  = strlen(lexema);
    x->lexema = malloc(size);
    sprintf(x->lexema, "%s", lexema);
    sprintf(x->tipo, "%s", tipo);
    x->escopo = escopo;
}

registro *busca_incidente(registro * _registro, registro *tabela)
{
    registro *p;
    p = tabela;
    while (!pilha_vazia(p) && !strcmp(_registro->conteudo.lexema, p->conteudo.lexema))
        p = p->prox;
    return p;
}

registro *busca_duplicatas(registro * _registro)
{
    registro *p;
    p = _registro;
    while (!pilha_vazia(p) && !strcmp(_registro->conteudo.lexema, p->conteudo.lexema) && p->conteudo.escopo!=1)
        p = p->prox;
    return p;
}

elemento pop(registro **tabela){
    elemento _registro;
    bzero(&_registro, sizeof(elemento));
    registro *aux = *tabela;
    if(!pilha_vazia(aux)) {
        _registro = aux->conteudo;
        *tabela = aux->prox;
        libera_mem(aux);
    }
    return _registro;
}

int pilha_vazia(registro * tabela){
    if(tabela!=NULL) return 0;
    return 1;
}

registro *cria_tabela(tokens *lista)
{
    tokens *p = lista;
    elemento campos;
    registro *aux, *tabela;
    char *tipo;

    tabela = NULL;

    if (!strcmp(p->lexema, "var"))
    {
        while (strcmp(p->lexema, "inteiro") && strcmp(p->lexema, "booleano") && p->prox!=NULL)
        {
            p = p->prox;
        }
         if(p->prox==NULL) return (registro *)tipo_null;
         return  (registro *)p->lexema;
    }
    else
    {
        while (p != NULL)
        {
            if (!strcmp(p->lexema, "inicio"))
            {
                while (strcmp(p->lexema, "fim"))
                    p = p->prox;
            }
            else
            {
                if (!strcmp(p->lexema, "var"))
                {
                    tipo = (char *)cria_tabela(p);
                }
                else
                {
                    if (!strcmp(p->simbolo, "sidentificador"))
                    {
                        if (!strcmp(tipo, "procedimento") || !strcmp(tipo, "programa"))
                        {
                            novo_elemento(&campos, p->lexema, tipo, 1);
                            push(campos, &tabela);
                        }
                        else
                        {
                            novo_elemento(&campos, p->lexema, tipo, 0);
                            push(campos, &tabela);
                        }
                    }
                    else
                    {
                        if (!strcmp(p->lexema, "procedimento"))
                            tipo = p->lexema;
                        else if (!strcmp(p->lexema, "programa"))
                            tipo = p->lexema;
                        else if (strcmp(p->lexema, ","))
                            tipo = tipo_null;
                    }
                }
            }

            p = p->prox;
        }
        return tabela;
    }
}

void push(elemento x, registro **pri)
{
    registro *p, *novo;
    p = *pri;
    novo = novo_registro();
    novo->conteudo = x;
    novo->prox = p;
    *pri = novo;
}

void imprimir_tabela(registro *lista)
{

    registro *p;
    p = lista;
    while (!pilha_vazia(p))
    {
        printf(" | %s | %s | %d \n", p->conteudo.lexema, p->conteudo.tipo, p->conteudo.escopo);
        p = p->prox;
    }
    printf("\n\n");
}
