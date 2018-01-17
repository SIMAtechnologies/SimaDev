 /*
 *DISTRIBUCIÓN DE VARIABLES EN EL CUERPO
 *               CABEZA 
 *       LL4-D5  -HOMBROS  - D9-D4
 *       L3-D4  -CADERAS  - D8 -D3
 *       L2-D3  -RODILLAS - D7 -D2
 *       L1-D2  -TALONES  - D6 -D1
 */

 //var in=253
 //var etapa anim = 254
 //var end = 255
 
#include <SIMA.h>
#include <EEPROM.h>
//condiciones iniciales de las articulaciones
int _init[8]={90,90,85,90,90,90,95,90};
//calibracion
int calibracion[8]={0,0,0,0,0,0,0,0};
//int calibracion[8]={7,14,0,0,-4,8,-5,0};

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
bool all[8]={1,1,1,1,1,1,1,1};

int minP[8]={544,544,544,554,544,544,544,564};
int maxP[8]={2400,2400,2400,2103,2400,2400,2400,2290};
void setup()
{
  //Inicio del puerto Serial
  Serial.begin(9600);
  //pinMode(13, OUTPUT); 
  //Inicialización de servos
  for (int i = 0;i < 8; i++)
  {
    ang[i]=_init[i];
    articulacion[i].attach(4+i,minP[i],maxP[i]);//, 800, 2200);
    //articulacion[i].write(_init[i]);
  }
  sima.animation(initcomand, articulacion, ang, 1,  all, all );
  /*
  //Configuracion de nuevas salidas digitales para led indicador de nivel
  pinMode(A1, OUTPUT);//ROJO
  pinMode(A2, OUTPUT);//VERDE
  pinMode(A3, OUTPUT);//AZUL
  delay(100);*/
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
  /*palabra = "";
  while(!palabra.endsWith(";")){
    if(Serial.available()>0){
      palabra += char(Serial.read());
    }
  }*/
  addr = 0;
  val = 0;
  bool in=false;
  while((val != 255) && (addr != EEPROM.length())){
    if(Serial.available()>0){
      val = Serial.read();
      Serial.println(val);
      if (in) 
      {
        EEPROM.write(addr, val);
        addr++;
      }
      
      if (val==253) in=true;
      
      
      //Serial.println(String(EEPROM[addr]));
      
    }
  }
  
  if(val == 255) 
  {
    Serial.print("len::");
    Serial.println(String(addr-1));
    int k=0; 
    while((10*k)<(addr-1))
    {
      for(int i=0; i<9; i++){
        comando[0][i] = byte(EEPROM[10*k+i]);
        //MODO MANUAL
        Serial.print("k:");
        Serial.println(k);
        Serial.println(int(comando[0][i]));
      }
      if (int(EEPROM[10*k+9])!=int(254)) break;
      k++;
      sima.animation(comando, articulacion, ang, 1,  all, all );
    }
  }
}
