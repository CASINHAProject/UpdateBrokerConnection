#include <Ultrasonic.h>
#include <SPI.h>

#define pino_trigger 4
#define pino_echo 5

Ultrasonic ultrasonic(pino_trigger, pino_echo);

int luz1 = 7;
int luz2 = 8;
int luz3 = 9;

void setup() {
  pinMode(luz1, OUTPUT);
  pinMode(luz2, OUTPUT);
  pinMode(luz3, OUTPUT);
  digitalWrite(luz1, LOW);
  digitalWrite(luz2, LOW); 
  digitalWrite(luz3, LOW); 
  Serial.begin(9600);

}

void loop() {
    float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - Distancia em polegadas: ");
  Serial.println(inMsec);
  if(cmMsec > 400){
    digitalWrite(luz1, LOW);
    digitalWrite(luz2, LOW); 
    digitalWrite(luz3, LOW);
  }else if(cmMsec < 200 && cmMsec > 100){
    digitalWrite(luz1, HIGH);
    digitalWrite(luz2, LOW); 
    digitalWrite(luz3, LOW);
  } else if(cmMsec < 100 && cmMsec > 50){
    digitalWrite(luz1, HIGH);
    digitalWrite(luz2, HIGH); 
    digitalWrite(luz3, LOW);
  }else if(cmMsec < 50){
    digitalWrite(luz1, HIGH);
    digitalWrite(luz2, HIGH); 
    digitalWrite(luz3, HIGH);
  }
  delay(500);

}
