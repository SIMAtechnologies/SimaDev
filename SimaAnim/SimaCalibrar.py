import tkinter
import bluetooth
import serial
import serial.tools.list_ports

#VARIABLES
maxes= [ 45, 45, 45, 45, 45, 45, 45, 45]
mines= [-45,-45,-45,-45,-45,-45,-45,-45]
centro=[ 90, 90, 90, 90, 90, 90, 90, 90, 15]
L=centro.copy()

#FUNCIONES

#blueSelect: Cambia el nombre propuesto por el seleccionado
def blueSelect(x):
    return
    #ventanaNombre.delete(1.0, tkinter.END)
    #mac=ventana.blueDev[bluetothList.curselection()[0]]
    #ventanaNombre.insert(tkinter.END, "SIMA-{}{}".format(mac[0:2],mac[15:17]))

#blueSearch: Busca dispositivos bluetooth
def blueSearch():
    ventana.response.set("")
    nearby_devices = bluetooth.discover_devices(lookup_names=True, flush_cache=True, duration=4)
    ventana.blueDev=[]
    bluetothList.delete(0, tkinter.END)
    for addr, name in nearby_devices:
        ventana.blueDev.append(addr)
        bluetothList.insert(tkinter.END,"%s - %s" % (addr, name))

#INTERFAZ

#Definicion de frames y ventana
ventana = tkinter.Tk()
fBD=tkinter.Frame(ventana)
fBDButton=tkinter.Frame(fBD)
fBN=tkinter.Frame(ventana)

#Lista de dispositivos Bluetooth
tkinter.Label(fBD, text="Dispositivos").grid(row=0, column=0)
bluetothList=tkinter.Listbox(fBD, selectmode=tkinter.SINGLE, exportselection=0, width=30)
bluetothList.bind('<<ListboxSelect>>', blueSelect)
bluetothList.grid(row=1, column=0)

#Botones de lista bluettoth
tkinter.Button(fBDButton, text="Buscar", command=blueSearch).grid(row=0, column=0)
fBDButton.grid(row=2, column=0)

#Definicion de geometria
fBD.pack(side=tkinter.LEFT)
fBN.pack(side=tkinter.LEFT)


ventana.mainloop()
#cerrar el puerto
#ventana.serial.close()
print("desconectado")