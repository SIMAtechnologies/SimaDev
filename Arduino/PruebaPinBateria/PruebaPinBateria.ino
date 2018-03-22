int battPin=A0;
int batt=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
batt+=(analogRead(battPin)-batt)>>4;
Serial.println(batt);
delay(100);
}
