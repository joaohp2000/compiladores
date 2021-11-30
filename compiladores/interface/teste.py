import sys
import time
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QTextEdit
from PyQt5 import QtCore, QtGui

class Stream(QtCore.QObject):
   ""
"Redirects console output to text widget."
""
newText = QtCore.pyqtSignal(str)

def write(self, text):
   self.newText.emit(str(text))

class GenMast(QMainWindow):
   ""
"Main application window."
""
def __init__(self):
    super().__init__()

    self.initUI()

# Custom output stream.
sys.stdout = Stream(newText = self.onUpdateText)

def onUpdateText(self, text):
   ""
"Write console output to text widget."
""
cursor = self.process.textCursor()
cursor.movePosition(QtGui.QTextCursor.End)
cursor.insertText(text)
self.process.setTextCursor(cursor)
self.process.ensureCursorVisible()

def closeEvent(self, event):
   ""
"Shuts down application on close."
""
# Return stdout to defaults.
sys.stdout = sys.__stdout__
super().closeEvent(event)

def initUI(self):
   ""
"Creates UI window on launch."
""
# Button
for generating the master list.
btnGenMast = QPushButton('Run', self)
btnGenMast.move(450, 100)
btnGenMast.resize(100, 100)
btnGenMast.clicked.connect(self.genMastClicked)

# Create the text output widget.
self.process = QTextEdit(self, readOnly = True)
self.process.ensureCursorVisible()
self.process.setLineWrapColumnOrWidth(500)
self.process.setLineWrapMode(QTextEdit.FixedPixelWidth)
self.process.setFixedWidth(400)
self.process.setFixedHeight(150)
self.process.move(30, 100)

# Set window size and title, then show the window.
self.setGeometry(300, 300, 600, 300)
self.setWindowTitle('Generate Master')
self.show()

def genMastClicked(self):
   ""
"Runs the main function."
""
print('Running...')
time.sleep(5)
print('Done.')

if __name__ == '__main__':
   # Run the application.
app = QApplication(sys.argv)
app.aboutToQuit.connect(app.deleteLater)
gui = GenMast()
sys.exit(app.exec_())