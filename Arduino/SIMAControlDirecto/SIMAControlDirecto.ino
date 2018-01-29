 /*
 *DISTRIBUCIÓN DE VARIABLES EN EL CUERPO
 *               CABEZA 
 *       L4-D5  -HOMBROS  - D9-D4
 *       L3-D4  -CADERAS  - D8 -D3
 *       L2-D3  -RODILLAS - D7 -D2
 *       L1-D2  -TALONES  - D6 -D1
 */


#include <SIMA.h>
#include <EEPROM.h>
 //Caracteres de mensaje
 byte messIn=253;
 byte poseEnd = 254;
 byte messEnd = 255;
 //Bufer serial
 byte buff[10];
 //Almacenamiento de movimiento
 byte comando[110][9];
//condiciones iniciales de las articulaciones
int _init[8]={90,90,85,90,90,90,95,90};
//calibracion
//int calibracion[8]={0,0,0,0,0,0,0,0};
int calibracion[8]={7,14,0,0,-4,8,-5,0};
//1int calibracion[8]={0,10,10,0,-3,-3,-14,0};

//Posicion inicial
byte initcomand[1][9]={{90,90,90,90,90,90,90,90,30}};
int ang[8];
//Variables de Sevos
//Servo L1, L2, L3, L4; //Izquierda:  Tobillo, Rodilla, Cadera, Hombro. 
//Servo R1, R2, R3, R4; //Derecha:    Tobillo, Rodilla, Cadera, Hombro.
Servo articulacion[8];
SIMA sima(calibracion);
int addr = 0;
byte val;

String palabra;
//Definicion de motores activos
bool all[8]={1,1,1,1,1,1,1,1};
void setup()
{
  //Inicio del puerto Serial
  Serial.begin(9600);
  //pinMode(13, OUTPUT); 
  //Inicialización de servos
  for (int i = 0;i < 8; i++)
  {
    ang[i]=_init[i];
    articulacion[i].attach(4+i);//, 800, 2200);
    //articulacion[i].write(_init[i]);
  }
  //Mover a posicion inicil
  sima.animation(initcomand, articulacion, ang, 1,  all, all );

  //limpiar buffer serial
  while(Serial.available() > 0){
   Serial.read();
  }
   
  //A espera del primer comando
  //Serial.print("Ingrese un comando\n");
  while(Serial.available()<=0){
    delay(500);
  }
}

void loop() {
  addr = 0;
  val = 0;
  int i=0;
  int k=0;
  bool in=false;
  bool correcto=true;
  //Lee todos los caracteres hasta que termina el mensaje
  while((val != messEnd) && (addr <=990)){
    if(Serial.available()>0){
      val = Serial.read();
      //Serial.println(val);
      if (in) 
      {
       addr++;
       if (i<9) {
        comando[k][i]=val;
        i++;
       }
       else if (val==poseEnd) 
       {
        Serial.print("buff: ");
        Serial.println(Serial.available());
        Serial.println(comando[k][8]);
        k++;
        i=0;
       }
       else 
       {
        correcto =false;
        break;
       }
      }
      //Inicio del mensaje
      if (val==messIn) in=true;
      
    }
  }
  //Ejecutar los comandos
  if(val == messEnd && correcto) 
  {
    Serial.print("len::");
    Serial.println(String(addr-1));
    sima.animation(comando, articulacion, ang, k,  all, all );
  }
}



