from PyQt5 import QtCore, QtGui, QtWidgets
from tkinter import Tk
from tkinter.filedialog import askopenfilename
from ctypes import *
from tkinter import Tk
from tkinter.filedialog import askopenfilename
import os
import sys

from PyQt5 import QtCore, QtGui, QtWidgets

class comando(Structure):
    """ creates a struct to match emxArray_real_T """
    _fields_ = [('tipo', (c_int)),
                ('jum',(c_int)),
                ('instrucao',(c_char) * 8),
                ('arg1',c_int),
                ('arg2', c_int)] 

class table_memory(Structure):
    """ creates a struct to match emxArray_real_T """
    _fields_ = [('rotulo', (c_int)),
                ('endereco',(c_int)),
                ]

""" class Widget(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self._textedit = QtWidgets.QTextEdit(readOnly=True)
        self._lineedit = QtWidgets.QLineEdit()
        self._pushbutton = QtWidgets.QPushButton("Send")
        self._pushbutton.clicked.connect(self.on_clicked)

        lay = QtWidgets.QGridLayout(self)
        lay.addWidget(self._textedit, 0, 0, 1, 2)
        lay.addWidget(self._lineedit, 1, 0)
        lay.addWidget(self._pushbutton, 1, 1)

        self._process = QtCore.QProcess(self)
        self._process.setProcessChannelMode(QtCore.QProcess.MergedChannels)
        self._process.readyRead.connect(self.on_readReady)

        current_dir = os.path.dirname(os.path.realpath(__file__))
        self._process.start("python3", ['teste.py'])
 """
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(916, 700)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.file_name =""
        
        """  self._process = QtCore.QProcess()
        self._process.setProcessChannelMode(QtCore.QProcess.MergedChannels)
        self._process.readyRead.connect(self.on_readReady)
        current_dir = os.path.dirname(os.path.realpath(__file__))
        """
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(90, 20, 620, 20))
        self.lineEdit.setReadOnly(True)
        self.lineEdit.setObjectName("lineEdit")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(30, 20, 60, 13))
        self.label.setObjectName("label")
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(20, 70, 541, 391))
        self.textEdit.viewport().setProperty("cursor", QtGui.QCursor(QtCore.Qt.IBeamCursor))
        self.textEdit.setReadOnly(True)
        self.textEdit.setObjectName("textEdit")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(20, 490, 81, 16))
        self.label_2.setObjectName("label_2")
        self.textEdit_2 = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit_2.setGeometry(QtCore.QRect(20, 510, 501, 71))
        self.textEdit_2.viewport().setProperty("cursor", QtGui.QCursor(QtCore.Qt.IBeamCursor))
        self.textEdit_2.setReadOnly(False)
        self.textEdit_2.setObjectName("textEdit_2")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(750, 540, 111, 23))
        self.pushButton.setObjectName("pushButton")
        self.textEdit_3 = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit_3.setGeometry(QtCore.QRect(600, 70, 241, 391))
        self.textEdit_3.setObjectName("textEdit_3")
        self.textEdit_3.setReadOnly(True)
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(680, 50, 81, 16))
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(20, 50, 101, 16))
        self.label_4.setObjectName("label_4")
        self.radioButton = QtWidgets.QRadioButton(self.centralwidget)
        self.radioButton.setGeometry(QtCore.QRect(580, 530, 82, 17))
        self.radioButton.setChecked(True)
        self.radioButton.setObjectName("radioButton")
        self.radioButton_2 = QtWidgets.QRadioButton(self.centralwidget)
        self.radioButton_2.setGeometry(QtCore.QRect(580, 550, 91, 17))
        self.radioButton_2.setObjectName("radioButton_2")
        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(580, 500, 101, 16))
        self.label_5.setObjectName("label_5")
        self.lineEdit_2 = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit_2.setGeometry(QtCore.QRect(600, 70, 121, 20))
        self.lineEdit_2.setReadOnly(True)
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.lineEdit_4 = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit_4.setGeometry(QtCore.QRect(720, 70, 121, 20))
        self.lineEdit_4.setReadOnly(True)
        self.lineEdit_4.setObjectName("lineEdit_4")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setEnabled(True)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 916, 21))
        self.menubar.setDefaultUp(False)
        self.menubar.setObjectName("menubar")
        self.menuArquivo = QtWidgets.QMenu(self.menubar)
        self.menuArquivo.setObjectName("menuArquivo")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setEnabled(True)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionSelecionar_arquivo = QtWidgets.QAction(MainWindow)
        self.menubar.addAction(self.menuArquivo.menuAction())
        self.lineEdit_5 = QtWidgets.QLineEdit(self.centralwidget)
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(20, 600, 101, 16))
        self.label_6.setObjectName("label_6")
        self.lineEdit_5.setGeometry(QtCore.QRect(20, 620, 170, 25))
        self.lineEdit_5.setReadOnly(False)
        self.lineEdit_5.setObjectName("lineEdit_5")
        self.pushButton_input = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_input.setGeometry(QtCore.QRect(220, 620, 111, 23))
        
       

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Maquina virtual"))
        self.label.setText(_translate("MainWindow", "Arquivo:"))
        self.label_2.setText(_translate("MainWindow", "Saída de dados:"))
        self.pushButton.setText(_translate("MainWindow", "Executar"))
        self.label_3.setText(_translate("MainWindow", "Memória (Pilha)"))
        self.label_4.setText(_translate("MainWindow", "Código de máquina:"))
        self.radioButton.setText(_translate("MainWindow", "Normal"))
        self.radioButton_2.setText(_translate("MainWindow", "Passo a passo"))
        self.label_5.setText(_translate("MainWindow", "Modo de execução:"))
        self.lineEdit_2.setText(_translate("MainWindow", "Endereço"))
        self.lineEdit_4.setText(_translate("MainWindow", "Valor"))
        self.menuArquivo.setTitle(_translate("MainWindow", "Arquivo"))
        self.menuArquivo.addAction('Selecionar Arquivo', self.selecionar_arquivo)
        self.label_6.setText(_translate("MainWindow", "Entrada:"))
        self.pushButton_input.setText(_translate("MainWindow", "Entrada"))
        self.pushButton_input.clicked.connect(self.on_clicked)
        self.pushButton.clicked.connect(self.inicia_execucao)

    def inicia_execucao(self):
        nomeArquivo = open("nomedoarquivo.txt", "w")
        nomeArquivo.write(self.file_name)
        nomeArquivo.close
        
        self._process = QtCore.QProcess()
        self._process.setProcessChannelMode(QtCore.QProcess.MergedChannels)
        self._process.readyRead.connect(self.on_readReady)

        current_dir = os.path.dirname(os.path.realpath(__file__))
        print(current_dir)
        self._process.start("python3", [current_dir+'/vm.py'])
        

    def on_readReady(self):
        cursor = self.textEdit_2.textCursor()
        cursor.movePosition(QtGui.QTextCursor.End)
        cursor.insertText(str(self._process.readAll(), "utf-8"))
        self.textEdit_2.ensureCursorVisible()

    
    def on_clicked(self):
        text = self.lineEdit_5.text() + "\n"
        self._process.write(text.encode())
        
    def selecionar_arquivo(self):
        root=Tk()
        root.withdraw()
        root.filename = askopenfilename() #
        root.destroy()
        local_filename=root.filename
        self.file_name = local_filename
        self.lineEdit.setText(local_filename)
        if local_filename != None  and local_filename != '':
            out = open(local_filename, "r")
            self.textEdit.clear()
            for linha in out.readlines():
                self.textEdit.insertPlainText(str(linha))           
            out.close()
        
if __name__ == "__main__":
    import sys
    os.environ["PYTHONUNBUFFERED"] = "1"
    app = QtWidgets.QApplication(sys.argv)
    
    #w = Widget()
    #w.show()
    
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())


    