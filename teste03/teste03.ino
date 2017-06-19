
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
 
// Update these with values suitable for your network.
int luz1 = 7;
int luz2 = 8;
int luz3 = 9;

#define pino_trigger 2
#define pino_echo 3

Ultrasonic ultrasonic(pino_trigger, pino_echo);

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,0,115);
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
  if (strcmp(topic, "luz1") == 0){
    //Serial.print("Luz 1 ligada");
     if(service(payload, length).equals("1") == true){
      digitalWrite(luz1, HIGH);
      mqttClient.publish("r/luz1","1");
    }else if(service(payload, length).equals("0") == true){
      digitalWrite(luz1, LOW);
      mqttClient.publish("r/luz1","0");
    }else if(service(payload, length).equals("state") == true){
      mqttClient.publish("rs/luz1",convert(digitalRead(luz1)));
      Serial.println(convert(digitalRead(luz1)));
    }
  }else if (strcmp(topic, "luz2") == 0){
    //Serial.print("Luz 1 ligada");
     if(service(payload, length).equals("1") == true){
      digitalWrite(luz2, HIGH);
      mqttClient.publish("r/luz2","1");
    }else if(service(payload, length).equals("0") == true){
      digitalWrite(luz2, LOW);
      mqttClient.publish("r/luz2","0");
    }else if(service(payload, length).equals("state") == true){
      mqttClient.publish("rs/luz2",convert(digitalRead(luz2)));
      Serial.println(convert(digitalRead(luz2)));
    }
  }else if (strcmp(topic, "luz3") == 0){
    //Serial.print("Luz 1 ligada");
     if(service(payload, length).equals("1") == true){
      digitalWrite(luz3, HIGH);
      mqttClient.publish("r/luz3","1");
    }else if(service(payload, length).equals("0") == true){
      digitalWrite(luz3, LOW);
      mqttClient.publish("r/luz3","0");
    }else if(service(payload, length).equals("state") == true){
      mqttClient.publish("rs/luz3",convert(digitalRead(luz3)));
      Serial.println(convert(digitalRead(luz3)));
    }
  }
  Serial.println();
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

  pinMode(luz1, OUTPUT);
  pinMode(luz2, OUTPUT);
  pinMode(luz3, OUTPUT);
  digitalWrite(luz1, HIGH);
  digitalWrite(luz2, HIGH); 
  digitalWrite(luz3, HIGH); 
  Serial.begin(9600);
 
  mqttClient.setServer(server, 10611);
  mqttClient.setCallback(callback);
 
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}
 
void loop()
{
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
  Serial.print(" - Distancia em polegadas: ");
  Serial.println(inMsec);
  delay(1000);
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

