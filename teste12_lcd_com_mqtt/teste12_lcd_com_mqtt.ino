
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <LiquidCrystal.h>

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,0,122);
IPAddress server(34,200,51,91); // IP DO BROKER

String a;
String b;

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

String service(byte* payload, unsigned int length){
  a = "";
  for(int i=0;i<length;i++){
    a += ((char)payload[i]);
  }
  
  if(a == "on"){
    return "1";
  } else if (a == "off"){
    return "0";
  } else{
    return "state";
  }
  
}

char *convert(int key){
  if(key){
    return {"1"};
  }
  return {"0"};
}


void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "m") == 0){
      //Limpa a tela
      lcd.clear();
      
      //Posiciona o cursor na coluna 3, linha 0;
      lcd.setCursor(1, 0);
      //Envia o texto entre aspas para o LCD
      lcd.print("Mensagem: ");
      lcd.setCursor(1, 1);
      lcd.print((char*)payload);
      delay(5000);
  }
}


 
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("newconnection", "sdnlditz", "iyLDGNSrLU4U")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{

  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);

  Serial.begin(9600);
 
  mqttClient.setServer(server, 10611);
  mqttClient.setCallback(callback);
 
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}
 
void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

}


