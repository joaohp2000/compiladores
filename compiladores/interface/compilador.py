
from PyQt5 import QtCore, QtGui, QtWidgets 
from tkinter import Tk
from tkinter.filedialog import askopenfilename
import os
import platform
import subprocess
import sys
from tkinter import END

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 652)
        self.file_name =""
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
        self.lineEdit_2 = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit_2.setGeometry(QtCore.QRect(20, 480, 121, 20))
        self.lineEdit_2.setReadOnly(True)
        self.lineEdit_2.setObjectName("lineEdit_2")
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
        self.lineEdit_3 = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit_3.setGeometry(QtCore.QRect(620, 570, 141, 20))
        self.lineEdit_3.setReadOnly(True)
        self.lineEdit_3.setObjectName("lineEdit_3")
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

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Compilador"))
        self.label.setText(_translate("MainWindow", "Arquivo:"))
        self.label_2.setText(_translate("MainWindow", "Error:"))
        self.pushButton.setText(_translate("MainWindow", "Compilar"))
        self.pushButton.clicked.connect(self.compilar)
        self.menuArquivo.setTitle(_translate("MainWindow", "Arquivo"))
        self.actionSelecionar_arquivo.setText(_translate("MainWindow", "Selecionar arquivo"))
        self.menuArquivo.addAction('Selecionar Arquivo', self.selecionar_arquivo)
        self.menuArquivo.addAction('Salvar Arquivo', self.salvar_texto)
    
    def salvar_texto(self):
        texto = self.textEdit.toPlainText()
        local_filename = self.file_name
        if local_filename != None  and local_filename != '':
            out = open(local_filename, "w")
            out.write(texto)
            out.close()

    def compilar(self):
        self.salvar_texto()
        self.__compilar(self.file_name)
        out = open("output.txt", "r")
        self.textEdit_2.clear()
        for linha in out.readlines():
            self.textEdit_2.insertPlainText(str(linha))
    
    def selecionar_arquivo(self):
        root=Tk()
        root.title("Compilador")
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
    

    def __compilar(self, file):
        x = subprocess.call(["./comp",file[:-4], "file"])

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
