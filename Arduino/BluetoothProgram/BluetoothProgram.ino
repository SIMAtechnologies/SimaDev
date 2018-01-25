#include <SoftwareSerial.h>
SoftwareSerial BT1(3,4); // RX, TX recordar que se cruzan
//Conectar pin 3 a P1 y pin 4 a P2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT1.begin(9600);
}

void loop()
   {
       if (BT1.available())
           Serial.write(BT1.read());
           
       if (Serial.available())
          {  
            String S = GetLine();
             BT1.print(S);
          }
}

String GetLine()
   {   String S = "" ;
       if (Serial.available())
          {    char c = Serial.read(); ;
                while ( c != '\n')            //Hasta que el caracter sea intro
                  {     S = S + c ;
                        delay(25) ;
                        c = Serial.read();
                  }
                return( S) ;
          }
   }
