from MoveRead import *
mov=Movimientos("holoa.txt")
l=len(mov.listaMovi)
for i in range(l):
    s=mov.leerMov(i)
    print("movimiento: {} comando: {} descripcion: {}".format(s[2],s[1],s[3]))