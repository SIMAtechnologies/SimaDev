/*Autor: Javier Urrutia
  Email: javier.urrutia.r@ug.uchile.cl
*/

/*
  DISTRIBUCIÓN DE VARIABLES EN EL CUERPO
                CABEZA
        L4-D5  -HOMBROS  - D9-D4
        L3-D4  -CADERAS  - D8 -D3
        L2-D3  -RODILLAS - D7 -D2
        L1-D2  -TALONES  - D6 -D1
*/

#include <SIMA.h>
#include <EEPROM.h>
//Caracteres de mensaje
byte messIn = 253;
byte poseEnd = 254;
byte messEnd = 255;

//Estado de bateria(V=(battLevel+53)/210)
int battLevel;
int battFull = 807; //4V
int battEmpty = 618; //3.2V
int battReset = 702; //3.6V
int battPin = A0; //Pin de lectura
bool battSafe = true; //Controla que la bateria es suficiente para realizar animaciones
long int tUlL = 0; //Tiempo de la ultima lectura
long int tUlS = 0; //Tiempo de ultimo envío
int dT = 2000; //Periodo de envio de estado

//calibracion
int calibracion[8] = {  0,  0,  0,  0,  0,  0,  0,  0};


//Almacenamiento de movimiento
byte comando[110][9];

//condiciones iniciales de las articulaciones
int _init[8] = {90, 90, 85, 90, 90, 90, 95, 90};

//Posicion inicial
byte initcomand[1][9] = {{90, 90, 90, 90, 90, 90, 90, 90, 30}};
int ang[8];
//Variables de Sevos
//Servo L1, L2, L3, L4; //Izquierda:  Tobillo, Rodilla, Cadera, Hombro.
//Servo R1, R2, R3, R4; //Derecha:    Tobillo, Rodilla, Cadera, Hombro.
Servo articulacion[8];

//Inicio de sima
SIMA sima(calibracion);

//Variables del programa
int addr = 0;
byte val;

//Definicion de motores activos
/*Estado de motores:
  nada[0]: ningun motores
  pies[1]: solo los pies
  sup[2]:solo brazos
  inf[3]: solo solo piernas
  sPies[4]: solo piernas sin los pies
  sPiesBrazos[5]: todo los motores excepto los pies
  todo[6]: todos los motores
*/
bool estadoMotores[7][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, //nada
  {1, 0, 0, 0, 1, 0, 0, 0}, //pies
  {0, 0, 0, 1, 0, 0, 0, 1}, //sup
  {1, 1, 1, 0, 1, 1, 1, 0}, //inf
  {0, 1, 1, 0, 0, 1, 1, 0}, //sPies
  {0, 1, 1, 1, 0, 1, 1, 1}, //sPiesBrazos
  {1, 1, 1, 1, 1, 1, 1, 1} //todo
};

//Funcion que monitorea la baeria
void battMonitor() {
  //Revisar bateria
  if ((millis() - tUlL) > 100) {
    battLevel += (analogRead(battPin) - battLevel) >> 4;

    //Desactivacion a los 3.2 y reactivacion a los 3.6
    if ((battLevel <= (battEmpty + 25)) && battSafe) {
      battSafe = false;
    }
    else if ((battLevel >= battReset) && !battSafe) {
      battSafe = true;
    }

    tUlL = millis();
    //Serial.println(battLevel);
  }
  //Enviar estado de la bateria
  if (millis() - tUlS > dT) {
    if (battLevel <= 900) { //Enviar porcentaje

      int porcentaje = constrain((battLevel - battEmpty) * 100 / (battFull - battEmpty), 0, 100);
      Serial.print("Bateria = ");
      Serial.println(porcentaje);
    }
    else Serial.println("Estado = C");//Cargando
    tUlS = millis();
  }
}

//Funcion que envia la calibracion actual
void printCalib() {
  
}
  
void setup()
{
  //Inicio del puerto Serial
  Serial.begin(9600);
  //pinMode(13, OUTPUT);
  //Cargar calibracion
  EEPROM.get(0,calibracion);
  sima.setCalib(calibracion);
  //Inicialización de servos
  for (int i = 0; i < 8; i++)
  {
    ang[i] = _init[i];
    articulacion[i].attach(4 + i); //, 800, 2200);
    //articulacion[i].write(_init[i]);
  }
  //Mover a posicion inicial
  sima.animation(initcomand, articulacion, ang, 1,  estadoMotores[6], estadoMotores[4] );

  //limpiar buffer serial
  while (Serial.available() > 0) {
    Serial.read();
  }

  //A espera del primer comando
  //Serial.print("Ingrese un comando\n");
  //  while (Serial.available() <= 0) {
  //    delay(500);
  //  }
}

void loop() {
  addr = 0;
  val = 0;
  int mot_inicio = 6;
  int mot_final = 6;
  int i = 0;
  int k = 0;
  bool in = false;
  bool correcto = true;


  //Lee todos los caracteres hasta que termina el mensaje
  while ((val != messEnd) && (addr <= 990)) {
    battMonitor();//Mmonitorear bateria

    if (Serial.available() > 0) {
      val = Serial.read();
      //Serial.println(val);
      if (in)
      {
        addr++;
        if (i < 9) { //guardar angulos
          comando[k][i] = val;
          i++;
        }
        else if (val == poseEnd) //Fin de pose
        {
          //Serial.print("buff: ");
          //Serial.println(Serial.available());
          //Serial.println(comando[k][8]);
          k++;
          i = 0;
        }
        else //mensaje incorecto
        {
          correcto = false;
          break;
        }
      }
      //Inicio del mensaje
      if (val == messIn) in = true;
    }
  }

  //Verificacion final
  if (val == messEnd)
  {
    if (i == 3) //Recibir estado de motores
    {
      mot_inicio = comando[k][0];
      mot_final = comando[k][1];
      //
      //**Accion de calibracion**
      if (mot_inicio == 9 && mot_final == 9) { 
        for (int i = 0; i < 8; i++) {
          calibracion[i] = (int) comando[0][i] - 30; //Cambiar rango de valores para pemirtir negativos
          //Serial.println(calibracion[i]);
        }
        sima.setCalib(calibracion);
        EEPROM.put(0,calibracion);
        Serial.println("SET");
        correcto = false; //No ejecutar movimiento
      } 
      //
      //**Enviar calibracion actual**
      else if (mot_inicio == 10 && mot_final == 10) { 
          Serial.write(byte(252));
        for (int i = 0; i < 8; i++) {
          Serial.write(byte(calibracion[i]+30)); //Cambiar rango de valores para evitar negativos
        }
        correcto = false; //No ejecutar movimiento
      }
    }
    else correcto = false;
    //Ejecutar los comandos
    if (correcto)
    {
      //Serial.print("len::");
      //Serial.println(String(addr-1));
      if (battSafe) {
        Serial.println("Recibido");
        sima.animation(comando, articulacion, ang, k,  estadoMotores[mot_inicio], estadoMotores[mot_final] );
        Serial.println("k");
      }
      else {
        Serial.println("BattLow");
      }
    }
  }

}


