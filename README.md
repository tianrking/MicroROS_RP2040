pyside6-uic untitled.ui > ui_mainwindow.py

python3 main.py


# SLOT

from PySide6.QtCore import Slot

button.clicked.connect(say_hello)
@Slot()
def say_hello():
    print("Button clicked, Hello!")

