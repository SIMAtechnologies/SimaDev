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
 byte in=253;
 byte poseEnd = 254;
 byte messEnd = 255;
 
//condiciones iniciales de las articulaciones
int _init[8]={90,90,85,90,90,90,95,90};
//calibracion
int calibracion[8]={0,0,0,0,0,0,0,0};
//int calibracion[8]={7,14,0,0,-4,8,-5,0};

//Posicion inicial
byte initcomand[1][9]={{90,90,90,90,90,90,90,90,30}};
int ang[8];
//Variables de Sevos
//Servo L1, L2, L3, L4; //Izquierda:  Tobillo, Rodilla, Cadera, Hombro. 
//Servo R1, R2, R3, R4; //Derecha:    Tobillo, Rodilla, Cadera, Hombro.
Servo articulacion[8];
SIMA sima(calibracion);
int addr = 0;
int val;
byte comando[1][9];
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
  bool in=false;
  //Lee todos los caracteres hasta que termina el mensaje
  while((val != messEnd) && (addr != EEPROM.length())){
    if(Serial.available()>0){
      val = Serial.read();
      Serial.println(val);
      if (in) 
      {
        EEPROM.write(addr, val);
        addr++;
      }
      //Inicio del mensaje
      if (val==in) in=true;
      
      
      //Serial.println(String(EEPROM[addr]));
      
    }
  }
  //Ejecutar los comandos
  if(val == messEnd) 
  {
    //Serial.print("len::");
    //Serial.println(String(addr-1));
    int k=0; 
    while((10*k)<(addr-1))
    {
      for(int i=0; i<9; i++){
        comando[0][i] = byte(EEPROM[10*k+i]);
        //MODO MANUAL
        //Serial.print("k:");
        //Serial.println(k);
        //Serial.println(int(comando[0][i]));
      }
      if (int(EEPROM[10*k+9])!=int(poseEnd)) break;
      k++;
      sima.animation(comando, articulacion, ang, 1,  all, all );
    }
  }
}
