 /*A Tabela de Símbolos no CSD deverá seguir um dos padrões vistos anteriormente. A
única restrição ficará por conta do registro a ser definido, que deverá representar as seguintes
informações:
• Nome do identificador (lexema)
• Escopo (nível de declaração)
• Tipo (padrão do identificador)
• Memória (endereço de memória alocado)

Além do registro deverão ser implementados os seguintes procedimentos básicos:
Procedimentos:
• Insere na Tabela: inserir os identificadores na Tabela.
• Consulta a Tabela: percorre a Tabela procurando por um identificador. Devolve todos os
campos do registro.
• Coloca Tipo nas Variáveis: percorre a tabela do final para o começo substituindo todos
os campos tipo que possuem o valor variável pelo tipo agora localizado.*/
#ifndef _TABELA_SIMBOLOS
#define _TABELA_SIMBOLOS

#include "../analisador_lexico/analisador_lexico.h"
#include <sys/types.h>

struct elemento
{
    char *simbolo;
    char *lexema;
    u_int8_t escopo;
    char tipo[15];
    void *memoria;
};

typedef struct elemento elemento;

struct registro
{
    elemento conteudo;
    struct registro *prox;
};

typedef struct registro registro;
extern registro *tabela;

void insere_tipo(registro *pri, tokens *token, int func_var);

void insere_tabela(registro **pri, tokens *token, int nivel,void *rotulo );

registro * cria_tabela(tokens * lista);

void novo_elemento(elemento *x,  char *lexema, char *tipo, u_int8_t escopo);

registro *novo_registro();

void push (elemento x, registro* *pri);

void imprimir_tabela(registro *lista);

int pilha_vazia(registro * tabela);

int busca_incidente(tokens *token, registro *tabela);

int busca_duplicatas(registro * _registro, tokens *token); // RETORNA NULL SE NAO HOUVER DUPLICATA

elemento pop(registro **tabela);

void libera_mem(registro * element);

registro *novo_registro();

registro *pesquisa_tabela(registro * tabela, tokens * token);

void desempilha(registro **lista);
#endif