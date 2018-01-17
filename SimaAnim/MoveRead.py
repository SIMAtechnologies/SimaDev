# Clase para manejar archivo con movimientos
class Movimientos:
    # Crea un objeto Movimientos con el archivo
    def __init__(self, archivo):
        assert type(archivo) == str
        self.__archivo = archivo
        self.listaMovi = []
        self.__scan()

    # Scan: Busca movimientos en el archivo
    def __scan(self):
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
    def leerMov(self, num):
        assert type(num) == int
        movimientos = []
        f = open(self.__archivo)
        f.seek(self.listaMovi[num][1])
        header = f.readline()
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
        return movimientos, comando, self.listaMovi[num][0][1:3], descripcion, mot_inicio, mot_final

        # escribirMov: escribe un movimiento en el archivo

    def escribirMov(self, Mov):
        return
