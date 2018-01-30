import tkinter
import bluetooth
import serial
import serial.tools.list_ports
from time import sleep
import time

#Conectar pin 3 a P1 y pin 4 a P2

#FUNCIONES

#blueSelect: Cambia el nombre propuesto por el seleccionado
def blueSelect(x):
    ventanaNombre.delete(1.0, tkinter.END)
    mac=ventana.blueDev[bluetothList.curselection()[0]]
    ventanaNombre.insert(tkinter.END, "SIMA-{}{}".format(mac[0:2],mac[15:17]))

#blueSearch: Busca dispositivos bluetooth
def blueSearch():
    ventana.response.set("")
    nearby_devices = bluetooth.discover_devices(lookup_names=True, flush_cache=True, duration=4)
    ventana.blueDev=[]
    bluetothList.delete(0, tkinter.END)
    for addr, name in nearby_devices:
        ventana.blueDev.append(addr)
        bluetothList.insert(tkinter.END,"%s - %s" % (addr, name))

#conectOk: verifica si la conexion esta correcta
def conectOk(modulo):
    ventana.serial.write(b'AT'+ventana.mod[modulo])
    sleep(0.6)
    return b'OK' in ventana.serial.read(ventana.serial.inWaiting())

#rename: Envia el comando para cambiar el nombre
def rename():
    try:
        modulo=bluetothMod.get(bluetothMod.curselection()[0])
    except IndexError:
        return
    ventana.response.set("")
    if not conectOk(modulo):
        ventana.response.set("Fallo")
        return
    name=bytes(ventanaNombre.get("1.0",'end-1c'),"utf8")
    print(b'AT+NAME'+name+ventana.mod[modulo])
    ventana.serial.write(b'AT+NAME'+name+ventana.mod[modulo])
    tic = time.time()
    respuesta=b''
    while (time.time()-tic)<2 and not b'\n' in respuesta:
        respuesta+=ventana.serial.read(ventana.serial.inWaiting())
    ventana.response.set(respuesta)
    ventana.serial.flushInput()
    return

#conectar: Realiza la conexion al puerto serial especificado
def conectar(puerto):
    ventana.serial.close()
    ventana.serial.setPort(puerto)
    ventana.serial.open()
    print(puerto)

#INTERFAZ

#Definicion de frames y ventana
ventana = tkinter.Tk()
fBD=tkinter.Frame(ventana)
fBN=tkinter.Frame(ventana)
fBDButton=tkinter.Frame(fBD)

ventana.blueDev=[]
ventana.mod={"HC06":b"\n","BC04":b"\r\n"}

#Lista de dispositivos Bluetooth
tkinter.Label(fBD, text="Dispositivos").grid(row=0, column=0)
bluetothList=tkinter.Listbox(fBD, selectmode=tkinter.SINGLE, exportselection=0, width=30)
bluetothList.bind('<<ListboxSelect>>', blueSelect)
bluetothList.grid(row=1, column=0)

#Lista de modulos bluetooth
tkinter.Label(fBD, text="Modulo Bluetooth").grid(row=0, column=1)
bluetothMod=tkinter.Listbox(fBD, selectmode=tkinter.SINGLE, exportselection=0, width=6)
bluetothMod.grid(row=1, column=1)
for mod,val in ventana.mod.items() :
    bluetothMod.insert(tkinter.END, mod)

#Botones de lista bluettoth
tkinter.Button(fBDButton, text="Buscar", command=blueSearch).grid(row=0, column=0)
fBDButton.grid(row=2, column=0)

#Ventana de nombre
tkinter.Label(fBN, text="Nombre").grid(row=0, column=0)
ventanaNombre=tkinter.Text(fBN, width=20, height=2)
ventanaNombre.grid(row=1, column=0)
tkinter.Button(fBN, text="Cambiar nombre", command=rename).grid(row=2, column=0)
ventana.response=tkinter.StringVar()
ventana.response.set("")
tkinter.Label(fBN, textvariable=ventana.response).grid(row=3, column=0)

#Definicion de geometria
fBD.pack(side=tkinter.LEFT)
fBN.pack(side=tkinter.LEFT)

#Seleccionar puerto serial

ventana.puertos=[]
for p in serial.tools.list_ports.comports():
    if "CH340" in p.description:
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

ventana.mainloop()
#cerrar el puerto
ventana.serial.close()
print("desconectado")