#!/bin/bash
if [ $1 = "as" ]
   then
    echo "Compilando Analisador Sintatico:"
    gcc analisador_sintatico.c ../tabela_simbolos/tabela.c

elif [ $1 = "comp" ]
    then
    echo "Compilando compilador: "
    gcc compilador.c analisador_sintatico/analisador_sintatico.c tabela_simbolos/tabela.c analisador_lexico/analisador_lexico.c -o comp -g
else
    echo "Erro:"
fi