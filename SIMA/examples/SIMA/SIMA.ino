/*  Autor: José Polanco
    https://github.com/urayi
    Enero2017
 *DISTRIBUCIÓN DE VARIABLES EN EL CUERPO
 *               CABEZA 
 *       R4-D5  -HOMBROS  - D10-L4
 *       R3-D4  -CADERAS  - D9 -L3
 *       R2-D3  -RODILLAS - D8 -L2
 *       R1-D2  -TALONES  - D7 -L1
 */
#include <SIMA.h>
#include <EEPROM.h>
//condiciones iniciales de las articulaciones
int _init[8]={90,90,85,90,90,90,95,90};
//int _init[8]={90,60,55,90,90,120,125,90};

int ang[8];
//Variables de Sevos
//Servo L1, L2, L3, L4; //Izquierda:  Tobillo, Rodilla, Cadera, Hombro. 
//Servo R1, R2, R3, R4; //Derecha:    Tobillo, Rodilla, Cadera, Hombro.
Servo articulacion[8];
SIMA sima;
int addr = 0;
int val;
int comando;
String palabra;

void setup()
{
  //Inicio del puerto Serial
  Serial.begin(9600);
  //pinMode(13, OUTPUT);
  //Inicialización de servos
  for (int i = 0;i < 8; i++)
  {
    ang[i]=_init[i];
    articulacion[i].attach(4+i, 800, 2200);
    articulacion[i].write(_init[i]);
  }
  /*
  //Configuracion de nuevas salidas digitales para led indicador de nivel
  pinMode(A1, OUTPUT);//ROJO
  pinMode(A2, OUTPUT);//VERDE
  pinMode(A3, OUTPUT);//AZUL
  delay(100);
  //A espera del primer comando
  //Serial.print("Ingrese un comando\n");*/
  while(Serial.available()<=0){
    delay(1000);
  }
}

void loop() {
  /*palabra = "";
  while(!palabra.endsWith(";")){
    if(Serial.available()>0){
      palabra += char(Serial.read());
    }
  }*/
  addr = 0;
  val = 0;
  while((val != 59) && (addr != EEPROM.length())){
    if(Serial.available()>0){
      //val = Serial.read();
      EEPROM.write(addr, val = Serial.read());
      //Serial.println(String(EEPROM[addr]));
      addr++;
    }
  }
  if((val == 59) || (addr == EEPROM.length()))
  {
    //Serial.println(String(addr-1));
    for(int i=0; i<addr-1; i++){
      comando = EEPROM[i];
      //MODO MANUAL
      sima.Simatest(comando, articulacion, ang);
      //MODO PRE-PROGRAMADO DE MOVIMIENTOS
      sima.control(comando, articulacion, ang);
      //LECTURA DE BATERÍA
      //sima.voltajeBateria();
      //Serial.println(comando);
      //Serial.println(String(ang[0])+"\t"+String(ang[1])+"\t"+String(ang[2])+"\t"+String(ang[3])+"\t"+String(ang[4])+"\t"+String(ang[5])+"\t"+String(ang[6])+"\t"+String(ang[7]));
    }
  }
}
