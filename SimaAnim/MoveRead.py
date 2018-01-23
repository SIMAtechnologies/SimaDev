import tempfile
import os
#Excepcion de archivo modificado
class ArchivoModificado(Exception):
    pass

# Clase para manejar archivo con movimientos
class Movimientos:
    # Crea un objeto Movimientos con el archivo
    def __init__(self, archivo):
        assert type(archivo) == str
        self.__archivo = archivo
        self.listaMovi = []
        self.scan()

    # Scan: Busca movimientos en el archivo
    def scan(self):
        try:
            f = open(self.__archivo)
            pos = 0
            for line in f:
                ind = line.find("')//M")
                if ind > 0:
                    self.listaMovi.append((line[ind + 4:ind + 7], pos))
                pos += (len(line) + 1)
            f.close()
        except MemoryError:
            print("Memoria llena")
        return

    # leerMov: Lee un movimiento del archivo
    #num corresponde al indice en al lista de movimientos
    def leerMov(self, num):
        assert type(num) == int
        movimientos = []
        f = open(self.__archivo)
        f.seek(self.listaMovi[num][1])
        header = f.readline()
        try:
            comando=header[header.find('cmd')+6]
            descripcion = header[header.find('-') + 2:-1]
            f.readline()
            f.readline()
            linea = f.readline()
            while '{' in linea:
                pose = linea.strip('\t\n,{} ').split(',')
                pose = [int(val) for val in pose]
                movimientos.append(pose)
                linea = f.readline()
            # Buscar motores
            f.readline()
            funcion = f.readline().split(',')
            mot_inicio = funcion[4]
            mot_final = funcion[5].rstrip('\n;)')
        except (IndexError, ValueError):
            f.close()
            raise ArchivoModificado("El archivo original ha sido modificado o no tiene el formato correcto")
        f.close()
        return movimientos, comando, self.listaMovi[num][0][1:3], descripcion, mot_inicio, mot_final

    # escribirMov: escribe un movimiento en el archivo
    #formato como (movimientos,comando,codigo, descripcion, mot_inicio,_mot_final)
    def escribirMov(self, mov):
        assert len(mov)==6
        #buscar posicon para escribir
        i=0
        pos=self.listaMovi[0][1]
        try:
            while mov[2]>int(self.listaMovi[i][0][1:3]):
                pos=self.listaMovi[i+1][1]
                i=i+1
        #insertar al final
        except IndexError:
                pos=self.__buscarFin(i)

        #i marca el siguiente
        if mov[2]==int(self.listaMovi[i][0][1:3]):
            #cambiar al medio
            if i+1<len(self.listaMovi) :
                nextPos = self.listaMovi[i+1 ][1]
            #cambiar el utlimo
            else:
                nextPos=self.__buscarFin(i)
        #agregar al medio o al final
        else:
            nextPos = pos
        #copiar anterior
        with tempfile.NamedTemporaryFile(dir='.', delete=False) as tmp, \
                open(self.__archivo, 'rb') as f:
            while f.tell() <pos:
                tmp.write(f.readline())
            tmp.write(bytes(self.__textoAnimacion(mov[0],mov[1],mov[2],mov[3],mov[4],mov[5]),"utf8"))
            f.seek(nextPos)
            tmp.write(f.read())
        os.replace(tmp.name,self.__archivo)

        return

    #BuscarFin:busca la ultima linea de un movimientos
    def __buscarFin(self,mov):
        f=open(self.__archivo)
        f.seek(self.listaMovi[mov][1])
        f.readline()
        f.readline()
        f.readline()
        linea=f.readline()
        while '{' in linea:
            linea=f.readline()
        f.readline()
        f.readline()
        f.readline()
        pos=f.tell()
        f.close()
        return pos

    # formatoAngulos: Retorna un string con el formato de los comandos de angulos
    def __formatoAngulos(self,angulos):
        angText = "{"
        for ang in angulos[:-1]:
            angText += str(ang).rjust(3) + ","
        return angText + str(angulos[-1]).rjust(3) + "}"

    # printAnimacion:actualiza la animacion de la pantalla
    def __textoAnimacion(self, movimientos, com, cod, descp,mot_inicio,mot_final):
        texto=""
        texto+="""  if(cmd=='{}')//M{} - {}
    {{
        byte movimiento[][9] = {{\n""".format(com,cod,descp)
        if len(movimientos) > 0:
            for angulos in movimientos[:-1]:
                texto+="\t\t\t" + self.__formatoAngulos(angulos) + ", \n"
            texto+="\t\t\t" + self.__formatoAngulos(movimientos[-1]) + "\n"
        texto += """		}};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,{},{});
    }}\n""".format(mot_inicio, mot_final)
        return texto