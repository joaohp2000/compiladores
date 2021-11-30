#!/bin/bash
if [ $1 = "as" ]
   then
    echo "Compilando Analisador Sintatico:"
    gcc analisador_sintatico.c ../tabela_simbolos/tabela.c

elif [ $1 = "comp" ]
    then
    rm comp
    echo "Compilando compilador: "
    gcc compilador/compilador.c compilador/libs/*.c -o comp
elif [ $1 = "vm" ]
    then
    rm vm
    echo "Compilando Maquina Virtual: "
    gcc -Wall VM/vm.c VM/memoria.c -o vm -g
elif [ $1 = "com2" ]
    then
    echo "Compilando Maquina Virtual: "
    x86_64-w64-mingw32-gcc compilador/compilador.c compilador/libs/*.c -o comp
else
    echo "Erro:"
fi