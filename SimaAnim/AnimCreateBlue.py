import tkinter
import bluetooth
from MoveRead import *

#Variables de configuracon e inicializacion
maxes= [180,180,180,180,180,180,180,180,255]
mines= [ 0, 0, 0,  0,  0,  0,  0,  0,  0]
#maxes= [115,130,145,180,130,140,130,180,255]
#mines= [ 50, 40, 50,  0, 65, 50, 35,  0,  0]
centro=[ 90, 90, 90, 90, 90, 90, 90, 90, 15]
L=centro.copy()
motores=["todo","sup","inf","pies","sPies","nada"]

comandosList=[]
startChar=bytes([253])
endChar=bytes([255])
poseChar=bytes([254])

#formato de animacion
direccion="NONE"
inicio="""  if(cmd=='{}')//M{} - {}
    {{
        byte movimiento[][9] = {{\n"""

fin="""		}};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,{},{});
    }}\n"""

#updatetexto: Actualiza la pose a los valores de los cuadros de texto
def updateTexto(x):
    for i in range(9):
        try:
            num = int(texto[i].get())
            barras[i].set(num)
        except:
            return
#updateBarra: Actualiza la pose a los valores fijados en las barras
def updateBarra(x):
    for i in range(9):
        num = int(barras[i].get())
        texto[i].delete(0, tkinter.END)
        texto[i].insert(0, str(num))
        L[i] = num
    comando.set("comando a insertar"+str(L))

#formatoAngulos: Retorna un string con el formato de los comandos de angulos
def formatoAngulos(angulos):
    angText="{"
    for ang in angulos[:-1]:
        angText+= str(ang).rjust(3)+","
    return angText+str(angulos[-1]).rjust(3)+"}"

#printAnimacion:actualiza la animacion de la pantalla
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

#agregar: agrega una pose a la animacion
def agregar():
    comandosList.append(L.copy())
    poses.insert(tkinter.END, formatoAngulos(L))
    printAnimacion()

#atras: Borra la ultima pose agregada
def atras():
    if len(comandosList) >0:
        comandosList.pop()
        poses.delete(tkinter.END)
    printAnimacion()

#enviar: Envia la animacion al robot conectado
def enviar():
    datos=startChar
    for angulos in comandosList:
        datos+=bytes(angulos)+poseChar
    datos +=endChar
    sima.send(datos)

#centrar: regresa los valores de la pose a la posicion central
def centrar():
    for i in range(8):
        barras[i].set(centro[i])

#eliminar: Elimina la pose seleccionada de la animacion
def eliminar():
    try:
        ind=poses.curselection()[0]
    except:
        return
    poses.delete(tkinter.ANCHOR)
    comandosList.pop(ind)
    printAnimacion()

#cargarPose: carga una pose a las barras y cuadros de texto
def cargarPose():
    global L
    global accionStr
    try:
        ind = poses.curselection()[0]
    except:
        return
    L = comandosList[ind].copy()
    for i in range(9):
        num = L[i]
        barras[i].set(num)
        texto[i].delete(0, tkinter.END)
        texto[i].insert(0, str(num))
    accionStr = "modificar"
    comando.set("comando a " + accionStr + str(L))
    accion.config(text=accionStr, command=modificar)
    bAtras.config(command=modificar)
    poses.config(state=tkinter.DISABLED)
    bModificar.config(state=tkinter.DISABLED)
    bEliminar.config(state=tkinter.DISABLED)

#modificar: cambia los valores de una pose por los valores actuales
def modificar():
    global accionStr
    global L
    ind = poses.curselection()[0]
    comandosList[ind] = L.copy()
    accionStr = "agregar"
    accion.config(text=accionStr, command=agregar)
    bAtras.config(command=atras)
    poses.config(state=tkinter.NORMAL)
    bModificar.config(state=tkinter.NORMAL)
    bEliminar.config(state=tkinter.NORMAL)
    poses.delete(ind)
    poses.insert(ind, formatoAngulos(L))
    printAnimacion()

#cargarAnim: carga una animacion desde un archivo
def cargarAnim():
    global comandosList
    global movimientos
    try:
        ind=anim.curselection()[0]
    except:
        return
    #cargar animacion
    lista_poses, com, num, descpt, mot_inicio, mot_final=movimientos.leerMov(ind)
    comandosList=lista_poses.copy()
    inLetra.delete(0, tkinter.END)
    inLetra.insert(0, com)
    inCod.delete(0, tkinter.END)
    inCod.insert(0, num)
    inDescp.delete(0, tkinter.END)
    inDescp.insert(0, descpt)
    #cambiar motores
    inMotorInicio.select_clear(inMotorInicio.curselection())
    inMotorInicio.select_set(motores.index(mot_inicio))
    inMotorFinal.select_clear(inMotorFinal.curselection())
    inMotorFinal.select_set(motores.index(mot_final))
    #cargar poses
    poses.delete(0,tkinter.END)
    for item in lista_poses:
        poses.insert(tkinter.END,formatoAngulos(item))
    #actualizar pantalla
    printAnimacion()

#rcargarArchivo: carga la lista de animaciones de un archivo
def recargarArchivo():
    global archivo
    global movimientos
    movimientos.scan()
    anim.delete(0, tkinter.END)
    for mov in movimientos.listaMovi:
        anim.insert(tkinter.END, mov[0])

#reconectar: conecta el robot nuevamente
def reconectar():
    global direccion
    global  sima
    if direccion!="NONE":
        sima.close()
        sima = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sima.connect((direccion, 1))

#Definicion de frames y ventana
ventana = tkinter.Tk()
fVD=tkinter.Frame(ventana)
fValores = tkinter.Frame(fVD)
fAcciones = tkinter.Frame(ventana)
fParametros = tkinter.Frame(ventana)
fDisplay = tkinter.Frame(fVD)
fAnimaciones= tkinter.Frame(fParametros)

#Barras para in elegir angulo
labels=["Talon I","Rodilla I", "Caderas I", "Hombro I", "Talon D","Rodilla D", "Caderas D", "Hombro D", "Delay"]
barras = []
texto = []
for i in range(9):
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
comando.set("comando a insertar"+str(L))
tkinter.Label(fDisplay, textvariable=comando).grid(row=0, column=0)

#Lista de movimientos
tkinter.Label(fAnimaciones, text="Animaciones").grid(row=0, column=0)
scrollPoseAnim = tkinter.Scrollbar(fAnimaciones, orient=tkinter.VERTICAL, width=40)
anim=tkinter.Listbox(fAnimaciones, selectmode=tkinter.SINGLE, exportselection=0,width=35,
                      yscrollcommand=scrollPoseAnim.set)
scrollPoseAnim.config(command=anim.yview)
anim.grid(row=1, column=0)
scrollPoseAnim.grid(row=1, column=7)
fAnimButton=tkinter.Frame(fAnimaciones)
tkinter.Button(fAnimButton, text="Cargar", command=cargarAnim).grid(row=0, column=0)
#tkinter.Button(fAnimButton, text="Modificar", command=cargarPose).grid(row=0, column=1)
tkinter.Button(fAnimButton, text="Recargar animaciones", command=recargarArchivo).grid(row=0, column=2)
fAnimButton.grid(row=2, column=0)
fAnimaciones.grid(row=1, column=0)

#Ingreso de parameros (comando y motores activados)
tkinter.Label(fParametros, text="comando").grid(row=0, column=1)
inLetra=tkinter.Entry(fParametros)
inLetra.bind(sequence='<KeyRelease>', func=printAnimacion)
inLetra.grid(row=1, column=1)

#Motor al inicio
tkinter.Label(fParametros, text="Motor Inicio").grid(row=0, column=2)
inMotorInicio=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0)
inMotorInicio.bind('<<ListboxSelect>>', printAnimacion)
inMotorInicio.grid(row=1, column=2)

#Motor al final
tkinter.Label(fParametros, text="Motor Final").grid(row=0, column=3)
inMotorFinal=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0)
inMotorFinal.bind('<<ListboxSelect>>', printAnimacion)
inMotorFinal.grid(row=1, column=3)
#Insertar opciones
for item in motores:
    inMotorInicio.insert(tkinter.END, item)
    inMotorFinal.insert(tkinter.END, item)
inMotorInicio.select_set(0)
inMotorInicio.event_generate("<<ListboxSelect>>")
inMotorFinal.select_set(0)
inMotorFinal.event_generate("<<ListboxSelect>>")

#Comentarios
tkinter.Label(fParametros, text="Codigo").grid(row=0, column=4)
inCod=tkinter.Entry(fParametros)
inCod.bind(sequence='<KeyRelease>', func=printAnimacion)
inCod.grid(row=1, column=4)

tkinter.Label(fParametros, text="Descripcion").grid(row=0, column=5)
inDescp=tkinter.Entry(fParametros, width=40)
inDescp.bind(sequence='<KeyRelease>', func=printAnimacion)
inDescp.grid(row=1, column=5)

#Ventana de poses
tkinter.Label(fParametros, text="Poses").grid(row=0, column=6)
scrollPose = tkinter.Scrollbar(fParametros, orient=tkinter.VERTICAL)
poses=tkinter.Listbox(fParametros, selectmode=tkinter.SINGLE, exportselection=0,width=35,
                      yscrollcommand=scrollPose.set)
scrollPose.config(command=poses.yview)
poses.grid(row=1, column=6)
scrollPose.grid(row=1, column=7)
bEliminar=tkinter.Button(fParametros, text="Eliminar", command=eliminar)
bEliminar.grid(row=3, column=6)
bModificar=tkinter.Button(fParametros, text="Modificar", command=cargarPose)
bModificar.grid(row=3, column=7)

#Botones de accion
accion=tkinter.Button(fAcciones, text="Agregar", command=agregar)
accion.grid(row=0, column=0)
bAtras=tkinter.Button(fAcciones, text="Atrás", command=atras)
bAtras.grid(row=0, column=1)
tkinter.Button(fAcciones, text="Enviar", command=enviar).grid(row=0, column=2)
tkinter.Button(fAcciones, text="centrar", command=centrar).grid(row=0, column=3)
tkinter.Button(fAcciones, text="Reconectar", command=reconectar).grid(row=0, column=4)

#Definicion de geometria
fValores.pack(side=tkinter.LEFT)
fDisplay.pack(side=tkinter.LEFT)
fVD.pack()
fParametros.pack()
fAcciones.pack()

#Cargar Archivo
cargar=input("Cargar Archivo [y/n]:")
if cargar=="y" or cargar=="Y":
    archivo=input("Nombre del archivo:")
    movimientos=Movimientos(archivo)
    for mov in movimientos.listaMovi:
        anim.insert(tkinter.END, mov[0])
print("Cargado")

##Busqueda Bluetooth
print("buscando dispositivos cercanos...")
nearby_devices = bluetooth.discover_devices(lookup_names = True, flush_cache = True, duration = 5)
print("found %d devices" % len(nearby_devices))
i=0
for addr, name in nearby_devices:
    i +=1
    print("%d) %s - %s" % (i,addr, name))

ind= int(input("Seleccionar dispositivo:"))
direccion,dispositivo=nearby_devices[ind-1]
print(direccion)
sima=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sima.connect((direccion,1))
sima.settimeout(1.0)
ventana.mainloop()