from MoveRead import *
from tkinter import filedialog
from tkinter import *

root=Tk()
root.file=filedialog.askopenfilename(initialdir = "/" ,title = "Seleccionar archivo de movimientos",
                                            filetypes=(("Libreria", "*.cpp"), ("Todos los archivos", "*.*")))
mov=Movimientos(root.file)
l=len(mov.listaMovi)
for i in range(l):
    s=mov.leerMov(i)
    print("movimiento: {} comando: {} descripcion: {}".format(s[2],s[1],s[3]))
