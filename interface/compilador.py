
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QWidget, QApplication, QVBoxLayout, QHBoxLayout
from PyQt5.QtWidgets import QTextEdit, QLabel, QShortcut, QFileDialog, QMessageBox
from tkinter import Tk
from tkinter.filedialog import askopenfilename
import os
import platform
import subprocess
import sys
from tkinter import END
class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.file_path = None
    def open_new_file(self):
        self.file_path = QFileDialog.getOpenFileName(self, "Open new file", "", "All files (*)")

    def salvar_arquivo(self):
        self.file_path = QFileDialog.getSaveFileName(self, "Save this file  as...", "", "All files (*)")
        return self.file_path[0]
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 652)
        self.file_name =None
        self.return_compilador = 0
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(90, 20, 620, 20))
        self.lineEdit.setReadOnly(True)
        self.lineEdit.setObjectName("lineEdit")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(30, 20, 60, 13))
        self.label.setObjectName("label")
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(20, 50, 761, 411))
        self.textEdit.viewport().setProperty("cursor", QtGui.QCursor(QtCore.Qt.IBeamCursor))
        self.textEdit.setReadOnly(False)
        self.textEdit.setObjectName("textEdit")
        
      
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(30, 510, 47, 13))
        self.label_2.setObjectName("label_2")
        self.textEdit_2 = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit_2.setGeometry(QtCore.QRect(20, 530, 501, 71))
        self.textEdit_2.viewport().setProperty("cursor", QtGui.QCursor(QtCore.Qt.IBeamCursor))
        self.textEdit_2.setReadOnly(True)
        self.textEdit_2.setObjectName("textEdit_2")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(630, 530, 111, 23))
        self.pushButton.setObjectName("pushButton")

        self.comp_exec = QtWidgets.QPushButton(self.centralwidget)
        self.comp_exec.setGeometry(QtCore.QRect(630, 570, 150, 23))
        self.comp_exec.setObjectName("comp_exec")

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setEnabled(True)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
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
        self.actionSelecionar_arquivo.setEnabled(True)
        self.actionSelecionar_arquivo.setAutoRepeat(False)
        self.actionSelecionar_arquivo.setIconVisibleInMenu(False)
        self.actionSelecionar_arquivo.setShortcutVisibleInContextMenu(False)
        self.actionSelecionar_arquivo.setObjectName("actionSelecionar_arquivo")
        
        self.menubar.addAction(self.menuArquivo.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def compilar_executar(self):
        self.compilar()
        print("ok")
        if self.return_compilador ==1:
            current_dir = os.path.dirname(os.path.realpath(__file__))
            subprocess.call(["python3", current_dir+"/maquina_virtual.py", self.file_name[:-4]+".obj"])
    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Compilador"))
        self.label.setText(_translate("MainWindow", "Arquivo:"))
        self.label_2.setText(_translate("MainWindow", "Error:"))
        self.pushButton.setText(_translate("MainWindow", "Compilar"))
        self.pushButton.clicked.connect(self.compilar)
        self.comp_exec.setText(_translate("MainWindow", "Compilar e executar"))
        self.comp_exec.clicked.connect(self.compilar_executar)
        self.menuArquivo.setTitle(_translate("MainWindow", "Arquivo"))
        self.actionSelecionar_arquivo.setText(_translate("MainWindow", "Selecionar arquivo"))
        self.menuArquivo.addAction('Selecionar Arquivo', self.selecionar_arquivo)
        self.menuArquivo.addAction('Salvar Arquivo', self.salvar_texto)
    
    def salvar_texto(self):
        salva = Window()
        if self.file_name ==None:
            self.file_name = salva.salvar_arquivo()
        texto = self.textEdit.toPlainText()
        local_filename = self.file_name
        if local_filename != None  and local_filename != '':
            out = open(local_filename, "w")
            out.write(texto)
            #out.write('\x04')
            out.close()

    def compilar(self):
        self.salvar_texto()
        self.__compilar(self.file_name)
        out = open("output.txt", "r")
        self.textEdit_2.clear()
        for linha in out.readlines():
            self.textEdit_2.insertPlainText(str(linha))
    
    def selecionar_arquivo(self):
        abre = Window()
        abre.open_new_file()
        local_filename = abre.file_path[0]
        self.file_name = local_filename
        self.lineEdit.setText(local_filename)
        if local_filename != None  and local_filename != '':
            out = open(local_filename, "r")
            self.textEdit.clear()
            for linha in out.readlines():
                self.textEdit.insertPlainText(str(linha))           
            out.close()
    

    def __compilar(self, file):
        current_dir = os.path.dirname(os.path.realpath(__file__))
        self.return_compilador = subprocess.call(["./comp",file[:-4], "file"])

    
        
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
