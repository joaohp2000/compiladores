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

void salva_var(tokens *token); //salva token correspondente a variavel
void salva_expressao (tokens *token);  // salva token do iniciio da expressão
void _fim_expressao(tokens *token); //salva dim da expressao
Stack * token_to_stack(tokens *token); //converte o tipo token para a tipo Stack
Stack *In2Pos(); //gera expressao pos-fixa passada pelo salva expressao
Stack *stack_pop(Stack **_pilha); // remove elemento na pilha
int stack_vazia(Stack * _pilha); //verifica se pilha esta vazia
void push_stack(Stack **pri, tokens *token); //salva elemento na pilha
void insere_lista(Stack **lista, Stack *cel); // insere na lista pos-fix um elemento
void libera_lista(Stack *lista); //libera da memoria lista
int valida_atribuicao(Stack *lista); //valida o tipo da atribuição 
void inicializa_arquivo_objeto(char *nome_arquivo); // abre arquivo objeto
void gerador_codigo(char *_rotulo,char *comando, char *m, char *n); // funçao que escreve no arquivo objeto
int indice(Stack *lista, int var_total, registro *_tabela); // busca qual o numero da variavel
void gerar_expressao(Stack *pos_fix, int var_total); // gera expressão assembly a partir da expressao pos-fixa
void gerar_procedimento(); // resposavel pela intrução CALL
void gerar_atribuicao(int var_total); // resposavel por gerar codigo das atribuições
void close_arquivo(); // fecha arquivo
void gerar_leia_escreve(int var_total, tokens *token, char *comando); //gera codigo dos comandos leia e escreva
int valida_expressao(Stack *lista); // ve se a expressão é valida
#endif