from MoveRead import *
mov=Movimientos("movimientos.txt")
l=len(mov.listaMovi)
for i in range(l):
    s=mov.leerMov(i)
    print("movimiento: {} comando: {} descripcion: {}".format(s[2],s[1],s[3]))