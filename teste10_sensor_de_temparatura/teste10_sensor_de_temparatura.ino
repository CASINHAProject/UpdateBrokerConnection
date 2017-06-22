//Sensor de temperatura usando o LM35
 
const int LM35 = A0; // Define o pino que lera a saída do LM35
int temperatura; // Variável que armazenará a temperatura medida

int led1 = 8;
int led2 = 9;

unsigned long previousMillis = 0; // will store last time LED was updated
// constants won't change :
const long interval = 500; // interval at which to blink (milliseconds)
 
//Função que será executada uma vez quando ligar ou resetar o Arduino
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  ;
  Serial.begin(9600); // inicializa a comunicação serial
}
 
//Função que será executada continuamente
void loop() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
  // save the last time you blinked the LED
  previousMillis = currentMillis;
  // if the LED is off turn it on and vice-versa:
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  }
}
