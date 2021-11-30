#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "memoria.h"
#include "vm.h"

enum tipo_intrucoes
{
    NULAS = 1,
    CURTAS,
    LONGAS,
    COMPLETAS
} tipos; //(1 NULL, ADD, JPM 1, ALLOC 0,1)

//extern int *m,s=0;

int ler_programa(FILE *_arquivo, comando **programa, table_memory **_table_aux)
{
    char command[8], arg[3], arg2[3];
    char caracter;
    int i = 1, rotulo = 0;
    comando *program;
    table_memory *table_aux;

    program = (comando *)malloc(sizeof(comando));
    table_aux = (table_memory *)malloc(0);

    fscanf(_arquivo, "%s", command);
    sprintf(program->instrucao, "%s", command);
    while (strcmp(command, "HLT"))
    {
        //  printf("Tipo:%d||%d |%s | %d | %d\n", program[i - 1].tipo, program[i - 1].jump, program[i - 1].instrucao, program[i - 1].arg1, program[i - 1].arg2);
        program = (comando *)realloc(program, (i + 1) * sizeof(comando));
        fscanf(_arquivo, "%s", command);
        caracter = fgetc(_arquivo);
        if (caracter == 9)
        {
            //printf("\nrotulo %s|", command);
            program[i].jump = atoi(command);
            fscanf(_arquivo, "%s", command);
            sprintf(program[i].instrucao, "%s", command);
            program[i].arg1 = 0;
            program[i].arg2 = 0;
            if (program[i].jump > rotulo)
            {
                table_aux = (table_memory *)realloc(table_aux, sizeof(table_memory) * (program[i].jump + 1));
                rotulo = program[i].jump;
            }
            table_aux[program[i].jump].rotulo = program[i].jump;
            table_aux[program[i].jump].endereco = i;
            program[i].tipo = NULAS;
            //printf("%s", command);
        }
        else
        {
            if (caracter == 32)
            {
                program[i].jump = 0;
                caracter = fgetc(_arquivo);
                if (caracter == 32)
                {
                    //printf("\n%s", command);
                    program[i].arg1 = 0;
                    sprintf(program[i].instrucao, "%s", command);
                    program[i].tipo = CURTAS;
                }
                else
                {
                    sprintf(program[i].instrucao, "%s", command);
                    fseek(_arquivo, -1L, SEEK_CUR);
                    fscanf(_arquivo, "%s", arg);
                    //printf("\n%s | %s", command, arg);
                    program[i].arg1 = atoi(arg);
                    caracter = fgetc(_arquivo);
                    caracter = fgetc(_arquivo);
                    if (caracter != 10)
                    {
                        fseek(_arquivo, -1L, SEEK_CUR);
                        fscanf(_arquivo, "%s", arg2);
                        //printf("|%s", arg2);
                        program[i].arg2 = atoi(arg2);
                        program[i].tipo = COMPLETAS;
                    }
                    else
                    {
                        program[i].tipo = LONGAS;
                        program[i].arg2 = 0;
                    }
                }
            }
        }
        i++;
    }
    //printf("\n%d %d", table_aux[2].endereco, table_aux[2].rotulo);
    table_aux[0].rotulo = rotulo;
    *programa = program;
    *_table_aux = table_aux;
    return i;
}
void executar_programa(comando *programa, table_memory *table_aux, int num_intrucoes)
{
    int pc = 0;
    m = (memoria *) malloc(sizeof(int));
    m[0] = 0;
    char c;
    if (!strcmp(programa[pc].instrucao, "START"))
    {
        pc++;
        while (strcmp(programa[pc].instrucao, "HLT"))
        {
            switch (programa[pc].tipo)
            {
            case NULAS: //TIPO NULL
                /* code */
                //printf("%d  %s\n", programa[pc].jump, programa[pc].instrucao);
                break;
            case CURTAS: //SEM NENHUM ARGUMENTO
                //printf("    %s\n", programa[pc].instrucao);
                instrucoes_curtas(&(programa[pc]), table_aux, &pc);
                break;
            case LONGAS: //COM APENAS 1 ARGUMENTO
                 //printf("    %s %d", programa[pc].instrucao, programa[pc].arg1);
                instrucoes_longas(&(programa[pc]), table_aux, &pc);
                break;
            case COMPLETAS: //COM OS 2 ARGUMENTOS
                //printf("    %s %d %d", programa[pc].instrucao, programa[pc].arg1, programa[pc].arg2);
                instrucoes_completas(&(programa[pc]), table_aux, &pc);
                break;
            default:
                break;
            }
            imprimir_memoria();
            scanf("%c", &c);
            pc++;
        }
        free(m);
    }
    //alocar(1,1);
    //alocar(1,1);588888888888888888888888888888888888888888888888888888884}
}

void instrucoes_completas(comando *programa, table_memory *table_aux, int *pc)
{

    if (!strcmp(programa->instrucao, "ALLOC"))
    {
        m = (memoria *)realloc(m, sizeof(int) * (s + programa->arg2 + 1)); //alloc 0,1    1,3  4, 11 s=9
        for (int k = 0; k <= (programa->arg2-1); k++)
        {
            s++; //10
            m[s] = m[programa->arg1 + k]; //m[10]=m[9]
        }
        
        for (int k = 0; k < (programa->arg2); k++)
        {
            m[programa->arg1 + k]=0;
        }
    }
    else
    {
        for (int k = programa->arg2 - 1; k >= 0; k--)
        {
            m[programa->arg1 + k] = m[s];
            s--;
        }
        m =(memoria *) realloc(m, sizeof(int) * (s + programa->arg1 + 1));
    }
}
void instrucoes_longas(comando *programa, table_memory *table_aux, int *pc)
{
    int *pc_aux = pc;
    if (!strcmp(programa->instrucao, "LDC"))
        push(programa->arg1);
    else if (!strcmp(programa->instrucao, "LDV"))
        push(m[programa->arg1]);
    else if (!strcmp(programa->instrucao, "STR"))
        m[programa->arg1] = pop();
    else if (!strcmp(programa->instrucao, "JMP"))
        *pc_aux = table_aux[programa->arg1].endereco;
    else
    {
        if (!strcmp(programa->instrucao, "JMPF"))
        {
            int aux = pop();
            if (aux == 0){
                *pc_aux = table_aux[programa->arg1].endereco;
            }
        }
        else
        { //FUNCAO CALL
            push(*pc_aux);
            *pc_aux = table_aux[programa->arg1].endereco;
        }
    }
}
void instrucoes_curtas(comando *programa, table_memory *table_aux, int *pc)
{
    int *pc_aux = pc;
    int num1, num2;
    if (!strcmp(programa->instrucao, "ADD"))
    {
        num1 = pop();
        num2 = pop();
        push(num1 + num2);
    }
    else
    {
        if (!strcmp(programa->instrucao, "SUB"))
        {
            num1 = pop();
            num2 = pop();
            push(num2 - num1);
        }
        else
        {
            if (!strcmp(programa->instrucao, "MULT"))
            {
                num1 = pop();
                num2 = pop();
                push(num2 * num1);
            }
            else
            {
                if (!strcmp(programa->instrucao, "DIV"))
                {
                    num1 = pop();
                    num2 = pop();
                    push(num2 / num1);
                }

                else
                { //Operações logicas
                    if (!strcmp(programa->instrucao, "AND"))
                    {
                        num1 = pop();
                        num2 = pop();
                        push(num1 && num2);
                    }
                    else
                    {
                        if (!strcmp(programa->instrucao, "OR"))
                        {
                            num1 = pop();
                            num2 = pop();
                            push(num2 || num1);
                        }
                        else
                        {
                            if (!strcmp(programa->instrucao, "NEG"))
                            {
                                num1 = pop();
                                push(!num1);
                            }

                            else
                            { //INTRUÇÕES DE COMPARAÇÃO
                                if (!strcmp(programa->instrucao, "CME"))
                                {
                                    num1 = pop();
                                    num2 = pop();
                                    if (num2 < num1)
                                        push(1);
                                    else
                                        push(0);
                                }
                                else
                                {
                                    if (!strcmp(programa->instrucao, "CMA"))
                                    {
                                        num1 = pop();
                                        num2 = pop();
                                        if (num2 > num1)
                                            push(1);
                                        else
                                            push(0);
                                    }
                                    else
                                    {
                                        if (!strcmp(programa->instrucao, "CEQ"))
                                        {
                                            num1 = pop();
                                            num2 = pop();
                                            if (num2 == num1)
                                                push(1);
                                            else
                                                push(0);
                                        }
                                        else
                                        {
                                            if (!strcmp(programa->instrucao, "CDIF"))
                                            {
                                                num1 = pop();
                                                num2 = pop();
                                                if (num2 != num1)
                                                    push(1);
                                                else
                                                    push(0);
                                            }

                                            else
                                            {
                                                if (!strcmp(programa->instrucao, "CMEQ"))
                                                {
                                                    num1 = pop();
                                                    num2 = pop();
                                                    if (num2 <= num1)
                                                        push(1);
                                                    else
                                                        push(0);
                                                }
                                                else
                                                {
                                                    if (!strcmp(programa->instrucao, "CMAQ"))
                                                    {
                                                        num1 = pop();
                                                        num2 = pop();
                                                        if (num2 >= num1)
                                                            push(1);
                                                        else
                                                            push(0);
                                                    }
                                                    else
                                                    {
                                                        if (!strcmp(programa->instrucao, "RD"))
                                                        {
                                                            printf("\n:");
                                                            scanf("%d", &num1);
                                                            push(num1);
                                                        }
                                                        else
                                                        {
                                                            if (!strcmp(programa->instrucao, "RETURN"))
                                                            {
                                                                *pc = pop();
                                                            }
                                                            else
                                                            {
                                                                if (!strcmp(programa->instrucao, "PRN"))
                                                                {
                                                                    num1 = pop();
                                                                    printf("\n%d", num1);
                                                                }
                                                                else
                                                            {
                                                                if (!strcmp(programa->instrucao, "INV"))
                                                                {
                                                                    num1 = pop();
                                                                    push(num1*(-1));
                                                                }
                                                            }
                                                            }
                                                            
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}