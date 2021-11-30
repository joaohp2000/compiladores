#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "memoria.h"
#include "vm.h"

#define MAX 100
int main()
{
    iniciar_file("sint20.obj");
    executar_vm();
    return 0;
}