/******************************************************************************
* SIMA Project - Manual Test
*
* @version 2016
* @author Jose Polanco
* https://github.com/urayi
******************************************************************************/
#ifndef Simatest_H
#define Simatest_H
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Servo.h>
////////////////////////////
// Definitions            //
////////////////////////////
//#define L1_PIN 9	//Tobillo Izquierdo
//#define L2_PIN 10	//Rodilla Izquierda
//#define L3_PIN 11	//Cadera Izquierda
//#define L4_PIN 12	//Hombro Izquierdo
//#define L5_PIN 13	//Codo Izquierdo
//#define R1_PIN 4	//Tobillo Derecho
//#define R2_PIN 5	//Rodilla Derecha
//#define R3_PIN 6	//Cadera Derecha
//#define R4_PIN 7	//Hombro Derecho
//#define R5_PIN 8	//Codo Derecho
//define BAT_PIN A0	//Sensor de nivel de la bater�a
//#define FORWARD     1
//#define BACKWARD    -1
//#define LEFT        1
//#define RIGHT       -1
class SIMA
{
public:
	////////////////////////////
	// Enumerations           //
	////////////////////////////
	////////////////////////////
	// Variables              //
	////////////////////////////
	int *offset;
	////////////////////////////
	// Functions              //
	////////////////////////////
	/*Las variables de las funciones se defines y se le asignan valores predeterminados*/
	//Inicializar sima con calibracion
	SIMA(int calibracion[8]);
	/*1---> Simatest - Control individual de articulaciones*/
	int Simatest(int cmd, Servo articulacion[], int angulo[]);//Retornar el ángulo final
	/*2---> Funciones de Movimiento*/
	//Recibe los datos de la secuencia de movimiento
	int* mover(int cmd, Servo articulacion[], int angulo[], int orden[], int movimiento[][9], int filas);
	//camina, giro, curva, saludo, balanceo - Controles basicos para desplazamiento
	int control(int cmd, Servo articulacion[], int angulo[]);
	int* motor(int cmd, Servo articulacion[], int angulo[], int orden[], int movimiento[][9], int filas);
	void motores(Servo articulacion[], bool activacion[]);
	void* animation(int movimiento[][9], Servo articulacion[], int angulo[], int filas,bool mot_inicio[], bool mot_final[]);
	//-- Battery
	void voltajeBateria();
private:
	////////////////////////////
	// Enumerations           //
	////////////////////////////


	////////////////////////////
	// Variables              //
	////////////////////////////


	////////////////////////////
	// Functions              //
	////////////////////////////

};

#endif // Simatest_H //
