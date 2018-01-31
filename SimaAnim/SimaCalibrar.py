#Autor: Javier Urrutia
#Email: javier.urrutia.r@ug.uchile.cl
import tkinter
import bluetooth
import serial
import serial.tools.list_ports
from tkinter import filedialog
import sqlite3

#Excepcion de falta de archivo
class SinArchivo(Exception):
    pass

#VARIABLES
maxes= [ 45, 45, 45, 45, 45, 45, 45, 45]
mines= [-45,-45,-45,-45,-45,-45,-45,-45]
centro=[  0,  0,  0,  0,  0,  0,  0, 0,]
textoCalib="int calibracion[8] = "
startChar=bytes([253])
endChar=bytes([255])
poseChar=bytes([254])
delay=bytes([15])
todo=bytes([6])

#usar mac+angulos
insertQuery='''INSERT INTO calibraciones(mac, talon_I, rodilla_I, cadera_I,hombro_I, talon_D, rodilla_D,
                cadera_D, hombro_D) VALUES(?,?,?,?,?,?,?,?,?)'''

#usar angulos+mac
updateQuery='''UPDATE calibraciones SET talon_I = ?, rodilla_I= ?, cadera_I = ?, hombro_I = ?, talon_D = ?,
                                        rodilla_D = ?, cadera_D = ?, hombro_D = ?
                                        WHERE mac = ?'''
#usar mac
selectQuery='''SELECT talon_I, rodilla_I, cadera_I,hombro_I, talon_D, rodilla_D,
                cadera_D, hombro_D FROM calibraciones WHERE mac=?'''

#FUNCIONES

#updateBarra: Actualiza la pose a los valores fijados en las barras
def updateBarra(x):
    for i in range(8):
        ventana.L[i] = int(barras[i].get())
    ventanaCalib.config(state=tkinter.NORMAL)
    ventanaCalib.delete(1.0, tkinter.END)
    ventanaCalib.insert(tkinter.END, textoCalib+formatoAngulos(ventana.L)+";")
    ventanaCalib.config(state=tkinter.DISABLED)
    enviar()

#centrar: regresa los valores de la calibracion a 0
def centrar():
    for i in range(8):
        barras[i].set(centro[i])

#enviar: Envia la calibracion actual al robot
def enviar():
    if ventana.serial.isOpen():
        mensaje= startChar
        mensaje+=bytes([90+offset for offset in ventana.L])+delay+poseChar
        mensaje+=todo*2
        mensaje+=endChar
        print(mensaje)
        ventana.serial.write(mensaje)
    return

#blueSelect: Cambia el nombre propuesto por el seleccionado
def blueSelect(x):
    ventana.sima=ventana.blueDev[bluetothList.curselection()[0]]
    print(ventana.sima)
    if existe(ventana.sima):
        cargar(ventana.sima)
    return

#verificar si existe una mac en la base de datos
def existe(mac):
    ventana.cursor.execute('SELECT * FROM calibraciones WHERE mac=? ',(mac,))
    existeMac=ventana.cursor.fetchone()
    if existeMac:
        return True
    else:
        return False

#blueSearch: Busca dispositivos bluetooth
def blueSearch():
    nearby_devices = bluetooth.discover_devices(lookup_names=True, flush_cache=True, duration=4)
    ventana.blueDev=[]
    bluetothList.delete(0, tkinter.END)
    for addr, name in nearby_devices:
        ventana.blueDev.append(addr)
        bluetothList.insert(tkinter.END,"%s - %s" % (addr, name))
    serialSearch()

#serialSearch: busca dispositivos seriales
def serialSearch():
    ventana.puertos = []
    for p in serial.tools.list_ports.comports():
        if "CP210x" in p.description:
            ventana.puertos.append(p.device)
            print(p)
    print(ventana.puertos)
    menu = tkinter.Menu(ventana)
    ventana.config(menu=menu)
    menuPuertos = tkinter.Menu(menu)
    menu.add_cascade(label="Puertos", menu=menuPuertos)
    for puerto in ventana.puertos:
        menuPuertos.add_command(label=puerto, command=lambda: conectar(puerto))
    if len(ventana.puertos) > 0:
        conectar(ventana.puertos[0])

#guardar: guarda una calibracion
def guardar():
    if ventana.sima:
        datos=tuple(ventana.L)
        mac=(ventana.sima,)
        try:
            ventana.cursor.execute(insertQuery,mac+datos)
            ventana.db.commit()
            print("guardado")
        except sqlite3.IntegrityError:
            print(ventana.L)
            ventana.cursor.execute(updateQuery, datos+mac)
            ventana.db.commit()
            print("Modificado")
    else:
        return

#cargar: carga una calibracion de la base de datos
def cargar(mac):
    ventana.cursor.execute(selectQuery,(mac,))
    calibracion=ventana.cursor.fetchone()
    for i in range(8):
        ventana.L[i]=calibracion[i]
        barras[i].set(calibracion[i])
    return

#formatoAngulos: Retorna un string con el formato de los comandos de angulos
def formatoAngulos(angulos):
    angText="{"
    for ang in angulos[:-1]:
        angText+= str(ang).rjust(3)+","
    return angText+str(angulos[-1]).rjust(3)+"}"

#conectar: Realiza la conexion al puerto serial especificado
def conectar(puerto):
    ventana.serial.close()
    ventana.serial.setPort(puerto)
    ventana.serial.open()
    print(puerto)

#INTERFAZ

#Definicion de frames y ventana
ventana = tkinter.Tk()
fDown=tkinter.Frame(ventana)
fUP=tkinter.Frame(ventana)
fBD=tkinter.Frame(fDown)
fBDButton=tkinter.Frame(fBD)
fValores = tkinter.Frame(fUP)
fC=tkinter.Frame(fDown)

#Variables del programa
ventana.L=centro.copy()
ventana.blueDev=[]
ventana.sima=""

#Barras para in elegir angulo
labels=["Talon I","Rodilla I", "Cadera I", "Hombro I", "Talon D","Rodilla D", "Cadera D", "Hombro D", "Delay"]
barras = []
texto = []
for i in range(8):
    tkinter.Label(fValores, text="Grados " + labels[i]).grid(row=0, column=i)
    barras.append(tkinter.Scale(fValores, from_=maxes[i], to=mines[i], length=360, tickinterval=20, command=updateBarra))
    barras[i].grid(row=1, column=i)
    barras[i].set(centro[i])
fValores.grid(row=0, column=0)
tkinter.Button(fUP, text="centrar", command=centrar).grid(row=1, column=0)

#Ventana de calibracion
tkinter.Label(fC, text="Calibracion").grid(row=0, column=0)
ventanaCalib=tkinter.Text(fC, width=60, height=2,)
ventanaCalib.grid(row=1, column=0)
tkinter.Button(fC, text="Guardar", command=guardar).grid(row=2, column=0)


#Lista de dispositivos Bluetooth
tkinter.Label(fBD, text="Dispositivos").grid(row=0, column=0)
bluetothList=tkinter.Listbox(fBD, selectmode=tkinter.SINGLE, exportselection=0, width=40)
bluetothList.bind('<<ListboxSelect>>', blueSelect)
bluetothList.grid(row=1, column=0)

#Botones de lista bluettoth
tkinter.Button(fBDButton, text="Buscar", command=blueSearch).grid(row=0, column=0)
fBDButton.grid(row=2, column=0)

#Definicion de geometria

fBD.pack(side=tkinter.LEFT)
fC.pack(side=tkinter.LEFT)
fUP.pack()
fDown.pack()

#Seleccionar puerto serial
ventana.puertos=[]
for p in serial.tools.list_ports.comports():
    if "CP210x" in p.description:
        ventana.puertos.append(p.device)
        print(p)
print(ventana.puertos)
menu=tkinter.Menu(ventana)
ventana.config(menu=menu)
menuPuertos=tkinter.Menu(menu)
menu.add_cascade(label="Puertos", menu=menuPuertos)
for puerto in ventana.puertos:
    menuPuertos.add_command(label=puerto, command=lambda: conectar(puerto))
ventana.serial=serial.Serial()
if len(ventana.puertos)>0:
    conectar(ventana.puertos[0])

#Seleccionar base de datos
archivo = tkinter.filedialog.askopenfilename(initialdir="/", title="Seleccionar base de datos de calibraciones",
                                              filetypes=(("Calibraciones", "*.db"), ("Todos los archivos", "*.*")))
if archivo != '':
    ventana.db=sqlite3.connect(archivo)
    ventana.cursor=ventana.db.cursor()
else:
    raise SinArchivo("Se debe cargar una base de datos")

ventana.mainloop()
#cerrar el puerto
ventana.serial.close()
ventana.db.close()
print("desconectado")