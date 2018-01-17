import tkinter

L=[90,90,90,90,90,90,90,90]
maxes=[180,180,180,180,180,180,180,180]
mines=[0,0,0,0,0,0,0,0]
comandosList=[]
accionStr="insertar"
inicio="""  if(cmd=='{}')//M{} - {}
    {{
        byte movimiento[][8] = {{\n"""

fin="""		}};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,'{}','{}');
    }}\n"""

def updateTexto(x):
    for i in range(8):
        try:
            num = int(texto[i].get())
            barras[i].set(num)
        except:
            return
    #comando.set("comando a insertar" + str(L))

def updateBarra(x):
    for i in range(8):
        num = int(barras[i].get())
        texto[i].delete(0, tkinter.END)
        texto[i].insert(0, str(num))
        L[i] = num
    comando.set("comando a " + accionStr + str(L))

def formatoAngulos(angulos):
    angText="{"
    for ang in angulos[:-1]:
        angText+= str(ang).rjust(3)+","
    return angText+str(angulos[-1]).rjust(3)+"}"

def printAnimacion(x=None):
    animacion.config(state=tkinter.NORMAL)
    animacion.delete(1.0, tkinter.END)
    animacion.insert(tkinter.END,inicio.format(inLetra.get(),inCod.get(),inDescp.get()))
    if len(comandosList) > 0:
        for angulos in comandosList[:-1]:
            animacion.insert(tkinter.END, "\t\t\t" + formatoAngulos(angulos) + ", \n")
        animacion.insert(tkinter.END, "\t\t\t" + formatoAngulos(comandosList[-1]) + "\n")
    animacion.insert(tkinter.END, fin.format(inMotorInicio.get(inMotorInicio.curselection()),
                                             inMotorFinal.get(inMotorFinal.curselection())))

    animacion.config(state=tkinter.DISABLED)

def agregar():
    comandosList.append(L.copy())
    poses.insert(tkinter.END, str(L))
    printAnimacion()

def atras():
    if len(comandosList) >0:
        comandosList.pop()
        poses.delete(tkinter.END)
    for i in range(8):
            if len(comandosList)>0:
                barras[i].set(comandosList[-1][i])
    printAnimacion()

def eliminar():
    ind=poses.curselection()[0]
    poses.delete(tkinter.ANCHOR)
    comandosList.pop(ind)
    printAnimacion()

def cargarPose():
    global L
    global accionStr
    ind=poses.curselection()[0]
    L=comandosList[ind].copy()
    for i in range(8):
        num=L[i]
        barras[i].set(num)
        texto[i].delete(0, tkinter.END)
        texto[i].insert(0, str(num))
    accionStr="modificar"
    comando.set("comando a "+ accionStr + str(L))
    accion.config(text=accionStr,command=modificar)
    bAtras.config( command=modificar)
    poses.config(state=tkinter.DISABLED)
    bModificar.config(state=tkinter.DISABLED)
    bEliminar.config(state=tkinter.DISABLED)

def modificar():
    global accionStr
    global L
    ind=poses.curselection()[0]
    comandosList[ind]=L.copy()
    accionStr = "agregar"
    accion.config(text=accionStr, command=agregar)
    bAtras.config(command=atras)
    poses.config(state=tkinter.NORMAL)
    bModificar.config(state=tkinter.NORMAL)
    bEliminar.config(state=tkinter.NORMAL)
    poses.delete(ind)
    poses.insert(ind,str(L))
    printAnimacion()

#Definicion de frames y ventana
ventana = tkinter.Tk()
fVD=tkinter.Frame(ventana)
fValores = tkinter.Frame(fVD)
fAcciones = tkinter.Frame(ventana)
fParametros = tkinter.Frame(ventana)
fDisplay = tkinter.Frame(fVD)


#Barras para in elegir angulo
labels=["Talon I","Rodilla I", "Caderas I", "Hombro I", "Talon D","Rodilla D", "Caderas D", "Hombro D"]
barras = []
texto = []
for i in range(8):
    tkinter.Label(fValores, text="Grados " + labels[i]).grid(row=0, column=i)
    barras.append(tkinter.Scale(fValores, from_=maxes[i], to=mines[i], length=360, tickinterval=20, command=updateBarra))
    barras[i].grid(row=1, column=i)
    barras[i].set(L[i])
    texto.append(tkinter.Entry(fValores, width=7))
    texto[i].grid(row=2,column=i)
    texto[i].insert(0,L[i])
    texto[i].bind(sequence='<KeyRelease>', func=updateTexto)

#Ventana de texto
animacion=tkinter.Text(fDisplay, width=65)
animacion.grid(row=1, column=0)
comando=tkinter.StringVar()
comando.set("comando a "+accionStr+str(L))
tkinter.Label(fDisplay, textvariable=comando).grid(row=0, column=0)

#Ingreso de parameros (comando y motores activados)
tkinter.Label(fParametros, text="comando").grid(row=0, column=0)
inLetra=tkinter.Entry(fParametros)
inLetra.bind(sequence='<KeyRelease>', func=printAnimacion)
inLetra.grid(row=1, column=0)

#Motor al inicio
tkinter.Label(fParametros, text="Motor Inicio").grid(row=0, column=1)
inMotorInicio=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0)
inMotorInicio.bind('<<ListboxSelect>>', printAnimacion)
inMotorInicio.grid(row=1, column=1)

#Motor al final
tkinter.Label(fParametros, text="Motor Final").grid(row=0, column=2)
inMotorFinal=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0)
inMotorFinal.bind('<<ListboxSelect>>', printAnimacion)
inMotorFinal.grid(row=1, column=2)
for item in ["todo","sup","inf","pies","sPies","nada"]:
    inMotorInicio.insert(tkinter.END, item)
    inMotorFinal.insert(tkinter.END, item)
inMotorInicio.select_set(0)
inMotorInicio.event_generate("<<ListboxSelect>>")
inMotorFinal.select_set(0)
inMotorFinal.event_generate("<<ListboxSelect>>")

#Comentarios
tkinter.Label(fParametros, text="Codigo").grid(row=0, column=3)
inCod=tkinter.Entry(fParametros)
inCod.bind(sequence='<KeyRelease>', func=printAnimacion)
inCod.grid(row=1, column=3)

tkinter.Label(fParametros, text="Descripcion").grid(row=0, column=4)
inDescp=tkinter.Entry(fParametros, width=40)
inDescp.bind(sequence='<KeyRelease>', func=printAnimacion)
inDescp.grid(row=1, column=4)

#Ventana de poses
tkinter.Label(fParametros, text="Poses").grid(row=0, column=5)
scrollPose = tkinter.Scrollbar(fParametros, orient=tkinter.VERTICAL)
poses=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0,width=35,
                      yscrollcommand=scrollPose.set)
scrollPose.config(command=poses.yview)
poses.grid(row=1, column=5)
scrollPose.grid(row=1, column=6)
bEliminar=tkinter.Button(fParametros, text="Eliminar", command=eliminar)
bEliminar.grid(row=3, column=5)
bModificar=tkinter.Button(fParametros, text="Modificar", command=cargarPose)
bModificar.grid(row=3, column=6)

#Botones de accion

accion=tkinter.Button(fAcciones, text="Agregar", command=agregar)
accion.grid(row=0, column=0)
bAtras=tkinter.Button(fAcciones, text="Atrás", command=atras)
bAtras.grid(row=0, column=1)

#Definicion de geometria
fValores.pack(side=tkinter.LEFT)
fDisplay.pack(side=tkinter.LEFT)
fVD.pack()
fParametros.pack()
fAcciones.pack()

ventana.mainloop()
