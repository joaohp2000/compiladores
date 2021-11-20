#!/bin/bash
if [ $1 = "as" ]
   then
    echo "Compilando Analisador Sintatico:"
    gcc analisador_sintatico.c ../tabela_simbolos/tabela.c

elif [ $1 = "comp" ]
    then
    rm comp
    echo "Compilando compilador: "
    gcc -Wall compilador.c analisador_sintatico/analisador_sintatico.c tabela_simbolos/tabela.c analisador_lexico/analisador_lexico.c analisador_semantico/analisador_semantico.c -o comp -g
elif [ $1 = "vm" ]
    then
    rm vm
    echo "Compilando Maquina Virtual: "
    gcc -Wall VM/vm.c VM/memoria.c -o vm -g
else
    echo "Erro:"
fi