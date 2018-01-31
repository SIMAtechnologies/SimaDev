/******************************************************************************
* SIMA Project - Manual Test
*
* @version 2016
* @author Jose Polanco
* https://github.com/urayi
******************************************************************************/
#include "SIMA.h"
#include "Servo.h"
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
int filas;
/*Estado de motores:
nada: ningun motores
pies: solo los pies
sup:solo brazos
inf: solo solo piernas
sPies: solo piernas sin los pies
sPiesBrazos: todo los motores excepto los pies
todo: todos los motores
*/ 
bool nada[8]={0,0,0,0,0,0,0,0};
bool pies[8]={1,0,0,0,1,0,0,0};
bool sup[8]={0,0,0,1,0,0,0,1};
bool inf[8] = {1,1,1,0,1,1,1,0};
bool sPies[8]={0,1,1,0,0,1,1,0};
bool sPiesBrazos[8]={0,1,1,1,0,1,1,1};
bool todo[8]={1,1,1,1,1,1,1,1};
//int offset[8]={7,14,0,0,-2,8,-5,0};

SIMA::SIMA(int calibracion[8]){
	offset = new int[8];
	memcpy(offset,calibracion,sizeof(int)*8);
}
///////////////////////////////////////
//- FUNCIONES BASICAS DE MOVIMIENTO -//
///////////////////////////////////////
//MOVIMIENTO INDEPENDIENTE DE ARTICULACIONES
int SIMA::Simatest(int cmd, Servo articulacion[], int angulo[]){
  //motores(true, articulacion,'a');
	if(cmd=='1'||cmd=='2'||cmd=='3'||cmd=='4'){
		if(cmd=='1'){cmd=0;}
		if(cmd=='2'){cmd=1;}
		if(cmd=='3'){cmd=2;}
		if(cmd=='4'){cmd=3;}
		if(cmd=='t'){cmd=4;}
		if(cmd=='y'){cmd=5;}
		if(cmd=='u'){cmd=6;}
		if(cmd=='i'){cmd=7;}
		angulo[cmd]=constrain(angulo[cmd]+1,0,180);
		//Serial.println(String(angulo[cmd]));
		articulacion[cmd].write(angulo[cmd]);
    delay(1);
	}
	if(cmd=='5'||cmd=='6'||cmd=='7'||cmd=='8'){
		if(cmd=='q'){cmd=0;}
		if(cmd=='w'){cmd=1;}
		if(cmd=='e'){cmd=2;}
		if(cmd=='r'){cmd=3;}
		if(cmd=='5'){cmd=4;}
		if(cmd=='6'){cmd=5;}
		if(cmd=='7'){cmd=6;}
		if(cmd=='8'){cmd=7;}
		angulo[cmd]=constrain(angulo[cmd]-1,0,180);
		//Serial.println("articulacion\t"+String(cmd)+":\t"+String(angulo[cmd]));
		articulacion[cmd].write(angulo[cmd]);
    delay(1);
	}
	if(cmd=='t'||cmd=='y'||cmd=='u'||cmd=='i'){
		if(cmd=='1'){cmd=0;}
		if(cmd=='2'){cmd=1;}
		if(cmd=='3'){cmd=2;}
		if(cmd=='4'){cmd=3;}
		if(cmd=='t'){cmd=4;}
		if(cmd=='y'){cmd=5;}
		if(cmd=='u'){cmd=6;}
		if(cmd=='i'){cmd=7;}
		angulo[cmd]=constrain(angulo[cmd]+1,0,180);
		//Serial.println(String(angulo[cmd]));
		articulacion[cmd].write(angulo[cmd]);
    delay(1);
	}
	if(cmd=='q'||cmd=='w'||cmd=='e'||cmd=='r'){
		if(cmd=='q'){cmd=0;}
		if(cmd=='w'){cmd=1;}
		if(cmd=='e'){cmd=2;}
		if(cmd=='r'){cmd=3;}
		if(cmd=='5'){cmd=4;}
		if(cmd=='6'){cmd=5;}
		if(cmd=='7'){cmd=6;}
		if(cmd=='8'){cmd=7;}
		angulo[cmd]=constrain(angulo[cmd]-1,0,180);
		//Serial.println("articulacion\t"+String(cmd)+":\t"+String(angulo[cmd]));
		articulacion[cmd].write(angulo[cmd]);
    delay(1);
	}
	return angulo[cmd];
}
//FUNCIÓN QUE EJECUTA EL MOVIMIENTO
int* SIMA::mover(int cmd, Servo articulacion[], int angulo[], int orden[], byte movimiento[][9],int filas){
	if(cmd=='C')
	{
	int comparacion = 1;
	int columnas = 8;
	for(int i = 0; i < filas; i++){
		int tdelay= movimiento[i][8];
		tarea:
		comparacion=0;
		for(int j = 0; j < columnas; j++){
			int anguloObjetivo=movimiento[i][orden[j]]+offset[orden[j]];
			if(angulo[orden[j]] < anguloObjetivo){
				angulo[orden[j]] = constrain(angulo[orden[j]]+1, 0, anguloObjetivo);
				articulacion[orden[j]].write(angulo[orden[j]]);
			}
			if(angulo[orden[j]] > anguloObjetivo){
				angulo[orden[j]] = constrain(angulo[orden[j]]-1, anguloObjetivo, 180);
				articulacion[orden[j]].write(angulo[orden[j]]);
			}
		comparacion+=abs(angulo[orden[j]]-anguloObjetivo);
		}delay(tdelay);
		
		if(comparacion != 0){
			goto tarea;
		}
	}
	return angulo;
	}
}
//FUNCIÓN PARA ATTACH Y DETACH MOTORES
void SIMA::motores(Servo articulacion[],bool activacion[]){
	for (int i = 0;i < 8; i++)
		{
			if(activacion[i]){
			  articulacion[i].attach(4+i);
			  articulacion[i].write(angulo[i]);
			}else{
			  articulacion[i].detach();
			}
		}
  }

void* SIMA::animation(byte movimiento[][9], Servo articulacion[], int angulo[], int filas,bool mot_inicio[], bool mot_final[]){
  /*int movimiento[x][8] = {
    //{D4, D5, D6, D7, D8, D9,D10,D11}
    //{1 , 2 , 3 , 4 , 5 , 6 , 7 ,  8}
    //{TOB_DER,ROD_DER,CAD_DER,HOM_DER,TOB_IZQ,ROD_IZQ,CAD_IZQ,HOM_IZQ}
  };*/
  motores(articulacion,mot_inicio);
  int orden[8] = {0, 4, 1, 5, 2, 6, 3, 7};
  mover('C', articulacion, angulo, orden, movimiento, filas);
  motores(articulacion,mot_final);
}
//camina, giro y curva - Controles basicos para desplazamiento
int SIMA::control(int cmd, Servo articulacion[], int angulo[]){
    if(cmd=='a')//M01 - Adelante
	{
    byte movimiento[][9] = {
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15},
			{ 80,100, 95, 85, 80, 95,100, 95, 15},
			{ 90,105,100, 80, 90,100,105,100, 15},
			{100, 90, 85, 75,100, 90, 95,105, 15},
			{100, 80, 75, 80,100, 80, 85,100, 15},
			{ 90, 85, 80, 85, 90, 85, 90, 95, 15},
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15} 
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
	}
	if(cmd=='s')//M02 - Atrás
	{
  	byte movimiento[][9] = {
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15},
			{ 90, 85, 80, 85, 90, 85, 90, 95, 15},
			{100, 80, 75, 80,100, 80, 85,100, 15},
			{100, 90, 85, 75,100, 90, 95,105, 15},
			{ 90,100, 95, 80, 90,100,105,100, 15},
			{ 80, 95, 90, 85, 80, 95,100, 95, 15},
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
	}
	if(cmd=='d')//M03 - Derecha
	{
    byte movimiento[][9] = {
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15},
			{ 90, 40, 40, 90, 90, 40, 40, 90, 15},
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15},
			{ 90,140,140, 90, 90,140,140, 90, 15},
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,inf,sPies);
	}
	if(cmd=='f')//M04 - Izquierda
	{
    byte movimiento[][9] = {
			{ 90, 90, 85, 90, 90, 90, 95, 90, 15},
			{ 90, 30, 35, 90, 90,130,130, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,inf,sPies);
	}
	if(cmd=='g')//M05 - Giro Derecha
	{
		byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15},
			{ 90, 40, 40, 90, 90, 40, 40, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,inf,sPies);
	}
	if(cmd=='h')//M06 - Giro Izquierda
	{
		byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15},
			{ 90,140,140, 90, 90,140,140, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,inf,sPies);
	}
	if(cmd=='j')//M07 - Parar
	{
		byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
	}
	if(cmd=='k')//M08 - Saludar
	{
		byte movimiento[][9] = { 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 10},
			{ 90, 90, 90, 90, 95, 90, 90,130, 10},
			{ 90, 90, 90, 90, 95, 90, 90,170,  7},
			{ 90, 90, 90, 90, 95, 90, 90,130,  7},
			{ 90, 90, 90, 90, 95, 90, 90,170,  7},
			{ 90, 90, 90, 90, 90, 90, 90, 90, 10}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
	}
  if(cmd=='l')//M09 - Bailar
	{
		byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15},
			{ 95, 92, 90, 90,100, 92, 95,170, 15},
			{ 85, 88, 90, 10, 90, 88, 95, 90, 15},
			{ 95, 92, 90, 90,100, 92, 95,170, 15},
			{ 85, 88, 90, 10, 90, 88, 95, 90, 15},
			{ 95, 92, 90, 90,100, 92, 95,170, 15},
			{ 85, 88, 90, 10, 90, 88, 95, 90, 15},
			{ 95, 92, 90, 90,100, 92, 95,170, 15},
			{ 85, 88, 90, 10, 90, 88, 95, 90, 15},
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
	}
  if(cmd=='z')//M10 - baja mano izquierda
  {
    byte movimiento[][9] = {
      { 90, 90, 90,180, 90, 90, 90, 90, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
  }
  if(cmd=='x')//M11 - baja mano derecha
  {
    byte movimiento[][9] = {
      { 90, 90, 90, 90, 90, 90, 90, 0, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
  }
  if(cmd=='c')//M12 - Desconexión
    {
        byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,nada);
    }
  if(cmd=='v')//M13 - Balancear
	{
		byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15},
			{105, 90, 90, 90,100, 90, 90, 90, 15},
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15},
			{ 80, 90, 90, 90, 75, 90, 90, 90, 15},
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
	}
  if(cmd=='b')//M14 - celebra con una ola (subir y bajar brazos)
  {
    byte movimiento[][9] = {
			{ 90, 90, 90,  0, 90, 90, 90,180, 10}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 10}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
  }
  if(cmd=='n')//M15 - manos arriba
  {
    byte movimiento[][9] = {
      { 90, 90, 90, 0, 90, 90, 90,180, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,sup,sPies);
  }
  if(cmd=='m')//M16 - manos abajo
  {
    byte movimiento[][9] = {
      { 90, 90, 90,180, 90, 90, 90, 0, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,sup,sPies);
  }
  if(cmd=='o')//M17 - sube mano izquierda
  {
    byte movimiento[][9] = {
      { 90, 90, 90, 0, 90, 90, 90, 90, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,sup,sPies);
  }
  if(cmd=='p')//M18 - sube mano derecha
  {
    byte movimiento[][9] = {
      { 90, 90, 90, 90, 90, 90, 90, 180, 15}
    };
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,sup,sPies);
  }
  if(cmd=='9')//M19 - Patada Izquierda
    {
        byte movimiento[][9] = {
			{ 65, 90, 90, 90, 73, 90, 90, 90, 15}, 
			{ 90, 40, 50, 90, 73, 90, 90, 90, 10}, 
			{ 90, 90, 90, 90, 73, 90, 90, 90,  2}, 
			{ 90,130,145, 90, 73, 90, 90, 90,  2}, 
			{ 90,130,110, 90, 73, 90, 90, 90,  2}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90,  2}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='0')//M20 - Patada Derecha
    {
        byte movimiento[][9] = {
			{105, 90, 90, 90,110, 90, 90, 90, 15}, 
			{105, 90, 90, 90, 90,140,130, 90, 10}, 
			{105, 90, 90, 90, 90, 90, 90, 90,  2}, 
			{105, 90, 90, 90, 90, 50, 35, 90,  2}, 
			{105, 90, 90, 90, 90, 50, 70, 90,  2}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90,  2}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='D')//M21 - Sorpresa
    {
        byte movimiento[][9] = {
			{ 90, 90, 90,  0, 90, 90, 90,180,  1}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,sup,sPies);
    }
  if(cmd=='S')//M22 - Saludo derecho
    {
        byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 30}, 
			{ 99, 90, 90,100,102, 90, 90,100, 30}, 
			{ 99, 90, 90,100,102, 90, 90,140, 10}, 
			{ 99, 90, 90,100,102, 90, 90,120,  5}, 
			{ 99, 90, 90,100,102, 90, 90,140,  5}, 
			{ 99, 90, 90,100,102, 90, 90,120,  5}, 
			{ 99, 90, 90,100,102, 90, 90,140,  5}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 30}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
    }
  if(cmd=='F')//M23 - Saludo con la mano
    {
        byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}, 
			{ 90, 90, 90,168, 90, 90, 90, 65, 13}, 
			{ 90, 90, 90,168, 90, 90, 90, 75, 10}, 
			{ 90, 90, 90,168, 90, 90, 90, 56, 10}, 
			{ 90, 90, 90,168, 90, 90, 90, 75, 10}, 
			{ 90, 90, 90,168, 90, 90, 90, 65, 10}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='G')//M24 - Balanceo de una pierna
    {
        byte movimiento[][9] = {
			{ 90, 90, 88, 90, 90, 90, 92, 90, 30}, 
			{103, 90, 88, 90,102, 90, 90, 90, 30}, 
			{103, 90, 88, 90, 90, 80,100, 90, 15}, 
			{103, 90, 90, 90, 90, 80,115, 90, 15}, 
			{103, 90, 88, 90, 90, 90, 70, 90, 12}, 
			{103, 90, 88, 90, 90, 80,115, 90, 15}, 
			{103, 90, 88, 90, 90, 90, 90, 90, 14}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 30}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='H')//M25 - Balanceo pie izquierdo
    {
        byte movimiento[][9] = {
			{ 90, 90, 88, 90, 90, 90, 92, 90, 40}, 
			{ 50, 90, 80, 90, 70, 87, 86, 90, 30}, 
			{ 80, 90, 70, 90, 70, 87, 83, 90, 15}, 
			{ 80,105, 60, 90, 70, 87, 83, 90, 13}, 
			{ 80, 90,110, 90, 70, 87, 83, 90, 10}, 
			{ 80, 80,115, 90, 70, 87, 87, 90, 10}, 
			{ 80, 90, 65, 90, 70, 86, 82, 90, 10}, 
			{ 90, 90, 88, 90, 90, 90, 92, 90, 20}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='J')//M26 - Ola
    {
        byte movimiento[][9] = {
			{ 90, 90, 90,160, 90, 90, 90, 20, 10}, 
			{ 90, 90, 90,160, 90, 90, 90, 70, 10},   
			{ 90, 90, 90, 60, 90, 90, 90,180, 10}, 
			{ 90, 90, 90,  0, 90, 90, 90,120, 10}, 
			{ 90, 90, 90,120, 90, 90, 90, 20, 10}, 
			{ 90, 90, 90,160, 90, 90, 90, 20, 10}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
    }
  if(cmd=='K')//M27 - Caminata
    {
        byte movimiento[][9] = {
			{ 90, 90, 88, 90, 90, 90, 92, 90, 30}, 
			{100, 93, 88, 90,102, 90, 90, 90, 30}, 
			{100, 93, 88, 90, 90, 80, 60, 90, 20}, 
			{ 90, 93, 88, 90, 90, 80, 70, 90, 30}, 
			{ 74, 95, 87, 90, 85, 80, 77, 90, 20}, 
			{ 74,100,105, 90, 85, 80, 82, 90, 20}, 
			{ 90, 90, 88, 90, 90, 90, 92, 90, 15}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
    }
  if(cmd=='L')//M28 - Estiramiento derecho
    {
        byte movimiento[][9] = {
			{ 90, 90, 88, 90, 90, 90, 92, 90, 20}, 
			{ 95, 91, 88,130,130, 90, 90,130, 16}, 
			{ 95, 91, 88,130,130, 90, 90,170, 10}, 
			{ 95, 91, 88,130,112, 90, 90,170, 10}, 
			{ 95, 91, 88,130,130, 90, 90,170, 10}, 
			{ 95, 91, 88,130,112, 90, 90,170, 10}, 
			{ 95, 91, 88,130,130, 90, 90,170, 10}, 
			{ 90, 91, 88,130, 90, 90, 90,170, 20}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 20}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='Z')//M29 - Estiramiento izquierdo
    {
        byte movimiento[][9] = {
			{ 90, 90, 88, 90, 90, 90, 92, 90, 20}, 
			{ 50, 90, 88, 60, 85, 89, 92, 60, 16}, 
			{ 50, 90, 88, 20, 85, 89, 92, 60, 10}, 
			{ 68, 90, 88, 20, 85, 89, 92, 60, 10}, 
			{ 50, 90, 88, 20, 85, 89, 92, 60, 10}, 
			{ 68, 90, 88, 20, 85, 89, 92, 60, 10}, 
			{ 50, 90, 88, 20, 85, 89, 92, 60, 10}, 
			{ 90, 90, 88, 50, 90, 89, 92, 60, 20}, 
			{ 90, 90, 90, 90, 90, 90, 90, 90, 20}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,todo);
    }
  if(cmd=='X')//M30 - Abrazo
    {
        byte movimiento[][9] = {
			{ 90, 90, 90, 90, 90, 90, 90, 90, 15}, 
			{ 90, 90, 90, 55, 90, 90, 90,125, 20}
		};
    filas = sizeof(movimiento)/sizeof(movimiento[0]);
    animation(movimiento, articulacion, angulo, filas,todo,sPies);
    }

}
////////////////////////////
//- MONITOREO DE BATERIA -//
////////////////////////////
/*void SIMA::voltajeBateria(){
	//lectura analogica en el pin A7 y A6
	float bat2=2.0*analogRead(A7)*5/1024;
  //float bat1=analogRead(A6)*5/1024;
	if(bat2 >= 6.5){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  Serial.println("BAT:"+String(bat2));
}*/
/*
////////////////////////////
//- lECTURA DE COMANDOS -//
////////////////////////////
//FORMATO DE COMANDOS "char@int;"
//ejemplo a@10; diez pazos al frente
//cmd = a;
//pasos = 10;
void SIMA::leeComandos(){


}*/
