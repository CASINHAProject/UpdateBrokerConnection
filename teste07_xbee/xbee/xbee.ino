String received;
int rele9 = 8;
int rele10 = 9;
const int LM35 = A0; // Define o pino que lera a saída do LM35
int temperatura; // Variável que armazenará a temperatura medida

//unsigned long previousMillis = 0; // will store last time LED was updated
// constants won't change :
//const long interval = 1000;

void setup(){
  pinMode(rele9, OUTPUT);
  pinMode(rele10, OUTPUT);
  Serial.begin(9600);
}

char *convert(int key){
  if(key){
    return {"1"};
  }
  return {"0"};
}

void loop(){
  /*
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
    Serial.println("rs/temp/"+temperatura);
  }*/
  received = "";
  if(Serial.available()){
   while(Serial.available()){
    char mesagem = Serial.read();
    received += mesagem;
    delay(10);
   }
   if(received.indexOf("tempstate") >=0){
      temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
      String myString = String(temperatura);
      Serial.println("rs/temp/"+myString);
   }
   if(received.indexOf("luz1on") >=0){
      digitalWrite(rele9,HIGH);
      Serial.println("r/luz1/on");
   }
   if(received.indexOf("luz1off") >=0){
      digitalWrite(rele9,LOW);
      Serial.println("r/luz1/off");
   }
   if(received.indexOf("luz1state") >=0){
      if (convert(digitalRead(rele9)) == "1"){
        Serial.println("rs/luz1/on");
      }else{
        Serial.println("rs/luz1/off");
      }
   }
   if(received.indexOf("luz2on") >=0){
      digitalWrite(rele10,HIGH);
      Serial.println("r/luz2/on");
   }
   if(received.indexOf("luz2off") >=0){
      digitalWrite(rele10,LOW);
      Serial.println("r/luz2/off");
   }
   if(received.indexOf("luz2state") >=0){
      if (convert(digitalRead(rele10)) == "1"){
        Serial.println("rs/luz2/on");
      }else{
        Serial.println("rs/luz2/off");
      }
   }
   
   
}
delay(50);
}
