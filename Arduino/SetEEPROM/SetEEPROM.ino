#include <EEPROM.h>
int calibracion[8] = {  0,  0,  0,  0,  0,  0,  0,  0};

void setup() {

  EEPROM.put(0, calibracion);
}

void loop() {
  // put your main code here, to run repeatedly:

}
