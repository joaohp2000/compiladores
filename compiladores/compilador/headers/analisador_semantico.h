#ifndef _ANALISADOR_SEMANTICO
#define _ANALISADOR_SEMANTICO

#include "tabela.h"
#include "analisador_sintatico.h"
struct Stack
{
    char *simbolo;
    char *lexema;
    char *tipo;
    struct Stack *prox;
};

typedef struct Stack Stack;
extern Stack *pilha;

Stack * nova_celula();

void salva_var(tokens *token);
void salva_expressao (tokens *token);
void _fim_expressao(tokens *token);
Stack * token_to_stack(tokens *token);
Stack *In2Pos();
Stack *stack_pop(Stack **_pilha);
int stack_vazia(Stack * _pilha);
void push_stack(Stack **pri, tokens *token);
void insere_lista(Stack **lista, Stack *cel);
void libera_lista(Stack *lista);
int valida_atribuicao(Stack *lista);
void inicializa_arquivo_objeto(char *nome_arquivo);
void gerador_codigo(char *_rotulo,char *comando, char *m, char *n);
int indice(Stack *lista, int var_total, registro *_tabela);
void gerar_expressao(Stack *pos_fix, int var_total);
void gerar_procedimento();
void gerar_atribuicao(int var_total);
void close_arquivo();
void gerar_leia_escreve(int var_total, tokens *token, char *comando);
int valida_expressao(Stack *lista);
#endif