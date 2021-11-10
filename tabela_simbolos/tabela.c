#include "tabela.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include "../analisador_lexico/analisador_lexico.h"

char *tipo_null = "null";

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

int busca_incidente(tokens *token, registro *tabela)
{
    registro *p;
    p = tabela;
    while (!pilha_vazia(p) && strcmp(token->lexema, p->conteudo.lexema))
        p = p->prox;
    if(pilha_vazia(p)) {
        return 1;
    }
    else {
        return 0;
    }
}

int busca_duplicatas(registro * _registro, tokens *token)
{
    registro *p;
    p = tabela;
    while (!pilha_vazia(p) && strcmp(token->lexema, p->conteudo.lexema) && p->conteudo.escopo!=1)
        p = p->prox;
    
    if(p->conteudo.escopo==1) {
        while(!pilha_vazia(p)){
            if(p->conteudo.escopo==1 && !strcmp(token->lexema, p->conteudo.lexema)){
                return 0;
            }
            p=p->prox;
        }  
        return 1;
    }
    else {
      //printf("%s : %s ja existe\n\n", p->conteudo.lexema, token->lexema);
        return 0;
    }

}

elemento pop(registro **tabela){
    elemento _registro;
    bzero(&_registro, sizeof(elemento));
    registro *aux = *tabela;
    if(!pilha_vazia(aux)) {
        memcpy(&_registro, &(aux->conteudo), sizeof(elemento));
        *tabela = aux->prox;
        free(aux);
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
    registro  *tabela;
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

void insere_tabela(registro **pri, tokens *token, int nivel,void *rotulo)
{
    registro *p, *novo;
    p = *pri;
    novo = novo_registro();
    novo->conteudo.lexema = token->lexema;
    novo->conteudo.simbolo = token->simbolo;
    novo->conteudo.escopo =nivel;
    sprintf(novo->conteudo.tipo, "null"); 
    novo->prox =p;
    *pri = novo;
}

void insere_tipo(registro *pri, tokens *token,int func_var){
    registro *aux =pri;
    if(func_var == 0){
    while(aux->conteudo.escopo != 1 && !strcmp(aux->conteudo.tipo,tipo_null))
    {
        if(!strcmp(aux->conteudo.simbolo, "sidentificador")){
            sprintf(aux->conteudo.tipo, "%s",token->simbolo);
        }
        aux=aux->prox;
    }  
    }
    else{
        sprintf(aux->conteudo.tipo, "func_%s",token->simbolo);
 
    } 
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
registro * pesquisa_tabela(registro * tabela, tokens * token){
    registro *p;
    p = tabela;
    while(!pilha_vazia(p) && strcmp(token->lexema, p->conteudo.lexema)){
      p = p->prox;  
    }
    if(pilha_vazia(p)) {
        return NULL;
    }
    else {
        return p;
    }
}
registro *tabela =NULL;

void desempilha(registro **lista){
    
    while((*lista)->conteudo.escopo != 1 && !pilha_vazia(*lista)){
        (*lista) = (*lista)->prox;
    }
    
    if(lista !=NULL){
        (*lista)->conteudo.escopo = 0;
    }
    else{
        printf("Error");
        exit(-1);
    }
}