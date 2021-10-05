#ifndef _ANALISA_SINTATICO
#define _ANALISA_SINTATICO

#include <stdio.h>
#include <stdlib.h>
#include "../analisador_lexico/analisador_lexico.h"

void error(tokens *token);
void analisador_sintatico(tokens *lista);
void analisa_bloco(tokens *lista);

int cp(tokens *token, char * simbolo);
void analisa_bloco(tokens *lista);
void analisa_et_variaveis(tokens *token);
void analisa_variaveis(tokens *token);
void analisa_tipo(tokens *token);
void analisa_comandos(tokens *token);
void analisa_fator(tokens *token);
void analisa_termo(tokens *token);
void analisa_expressao_simples(tokens *token);
void analisa_expressao(tokens *token);
void analisa_declaracao_funcao(tokens *token);
void analisa_declaracao_procedimento(tokens *token);
void analisa_subrotinas(tokens *token);
void analisa_se(tokens *token);
void analisa_enquanto(tokens *token);
void analisa_leia(tokens *token);
void analisa_escreva(tokens *token);
void analisa_atrib_chprocedimento(tokens *token);
void analisa_comando_simples(tokens *token);
void lx(tokens **token);
#endif