#include "memoria.h"

//Foi decidido usar lista ligada para fazer a memoria,
// devido ao receio de excessos de fragmentacao por uso de memoria dinamica

//declara endereços para a memoria;
memoria *m = NULL, s = -1;
//static int tamanho_memoria = 0;
//aloca nova posiçao de memoria
/* static memoria *nova_celula();
static memoria *nova_celula()
{
    memoria *novo = (memoria *)malloc(sizeof(memoria));
    return novo;
}

//retorna endereço de determinada posição
static memoria *__pos(int posicao);
static memoria *__pos(int posicao)
{
    int i = 0;
    memoria *aux = m;
    while (i < posicao && aux !=NULL)
    {
        aux = aux->prox;
        i++;
    }
    return aux;
}
//VALIDA SE MEMORIA OU PILHA ESTÁ VAZIA
int vazia(memoria *estrutura);
int vazia(memoria *estrutura)
{
    if (estrutura == NULL)
        return 1;
    return 0;
} */
//inicializa memoria e pilha

//aloca na memoria elementos
/*
void alocar(int pos, int num_elementos)
{
    
        int i = 0;
        memoria *novo, *aux;
        if (m == NULL)
        {
            m=malloc(sizeof(memoria)*num_elementos);
            s=&(m[num_elementos-1]);
        }
        else{
            m = realloc(m, sizeof(memoria)*(num_elementos+pos));
        }
        
        tamanho_memoria = tamanho_memoria + num_elementos;
    
    
}
void desalocar(int pos, int num_elementos)
{
    int aux;
    if (pos < tamanho_memoria)
    {
        int i = 0;
        while (i < num_elementos)
        {
            aux = pop();
            gravar_mem(aux, pos + i);
            i++;
        }
    }
    else
    {
        int i = 0;
        while (i < num_elementos)
        {
            pop();
            i++;
        }
        tamanho_memoria = tamanho_memoria - num_elementos;
    }
    
}
int ler_mem(int posicao)
{
    memoria *valor = __pos(posicao);
    return valor->mem;
}
void gravar_mem(int valor, int posicao)
{
    memoria *_pos = __pos(posicao);
    _pos->mem = valor;
} */
void push(int valor)
{
    int _s = s;
    m=(memoria *)realloc(m,sizeof(memoria)*(2+_s));
    s++;
    m[s]=valor;

}
int pop()
{
    int valor = m[s];
    m=(memoria *)realloc(m,sizeof(memoria)*(s));
    s--;
    return valor;
}

void imprimir_memoria(){
    
   for(int i=0;i<=s;i++){
       fprintf(stderr,"memoria[%d]          %4d\n",i,m[i]);
   }
   //printf("\n __^__");
}