from PyQt5 import QtCore, QtGui, QtWidgets
import os
import sys
from ctypes import *

arquivo = open("nomedoarquivo.txt", "r")
file_name = arquivo.readline()
arquivo.close

 
libc = cdll.LoadLibrary("../VM/vm.so")
libc.iniciar_file.argtypes=[c_char_p]

name = create_unicode_buffer(file_name)


string1 =file_name


# create byte objects from the strings
b_string1 = string1.encode('utf-8')


# send strings to c function



iniciar_file = libc.iniciar_file
executar_vm = libc.executar_vm
#numm = libc.numm
#numm(35)

iniciar_file(b_string1)
executar_vm()
class comando(Structure):
    
    _fields_ = [('tipo', (c_int)),
                ('jum',(c_int)),
                ('instrucao',(c_char) * 8),
                ('arg1',c_int),
                ('arg2', c_int)] 

class table_memory(Structure):
    
    _fields_ = [('rotulo', (c_int)),
                ('endereco',(c_int)),
                ]

