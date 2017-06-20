
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
 
// Update these with values suitable for your network.
int luz1 = 7;
int luz2 = 8;
int luz3 = 9;
int cont = 1;
const int LM35 = A0; // Define o pino que lera a saída do LM35


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

char * floatToString(char * outstr, float value, int places, int minwidth=0, bool rightjustify=false) {
    // this is used to write a float value to string, outstr.  oustr is also the return value.
    int digit;
    float tens = 0.1;
    int tenscount = 0;
    int i;
    float tempfloat = value;
    int c = 0;
    int charcount = 1;
    int extra = 0;
    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
    // if this rounding step isn't here, the value  54.321 prints as 54.3209

    // calculate rounding term d:   0.5/pow(10,places)  
    float d = 0.5;
    if (value < 0)
        d *= -1.0;
    // divide by ten for each decimal place
    for (i = 0; i < places; i++)
        d/= 10.0;    
    // this small addition, combined with truncation will round our values properly 
    tempfloat +=  d;

    // first get value tens to be the large power of ten less than value    
    if (value < 0)
        tempfloat *= -1.0;
    while ((tens * 10.0) <= tempfloat) {
        tens *= 10.0;
        tenscount += 1;
    }

    if (tenscount > 0)
        charcount += tenscount;
    else
        charcount += 1;

    if (value < 0)
        charcount += 1;
    charcount += 1 + places;

    minwidth += 1; // both count the null final character
    if (minwidth > charcount){        
        extra = minwidth - charcount;
        charcount = minwidth;
    }

    if (extra > 0 and rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }

    // write out the negative if needed
    if (value < 0)
        outstr[c++] = '-';

    if (tenscount == 0) 
        outstr[c++] = '0';

    for (i=0; i< tenscount; i++) {
        digit = (int) (tempfloat/tens);
        itoa(digit, &outstr[c++], 10);
        tempfloat = tempfloat - ((float)digit * tens);
        tens /= 10.0;
    }

    // if no places after decimal, stop now and return

    // otherwise, write the point and continue on
    if (places > 0)
    outstr[c++] = '.';


    // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
    for (i = 0; i < places; i++) {
        tempfloat *= 10.0; 
        digit = (int) tempfloat;
        itoa(digit, &outstr[c++], 10);
        // once written, subtract off that digit
        tempfloat = tempfloat - (float) digit; 
    }
    if (extra > 0 and not rightjustify) {
        for (int i = 0; i< extra; i++) {
            outstr[c++] = ' ';
        }
    }


    outstr[c++] = '\0';
    return outstr;
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
  else if (strcmp(topic, "temp") == 0){
    //Serial.print("Luz 1 ligada");
    if(service(payload, length).equals("state") == true){
      float temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
      char buffer[25];
      mqttClient.publish("rs/temp",floatToString(buffer, temperatura, 3));
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
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

}


