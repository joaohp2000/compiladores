#ifndef _ANALISADOR_SEMANTICO
#define _ANALISADOR_SEMANTICO

#include "../tabela_simbolos/tabela.h"

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
#endif