#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>




const char* ssid = "xxxxxxx"; // ssid
const char* password = "xxxxxxx";// password
IPAddress ip(192, 168, 1, 25); //set static ip
IPAddress gateway(192, 168, 1, 1); //set getteway
IPAddress subnet(255, 255, 255, 0);//set subnet
 

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress DestIp(192,168,1,226);        // remote IP of the destination PC
const unsigned int DestPort = 9001;          // remote port 
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)


int PIRPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status




void setup() 
{
  pinMode(PIRPin, INPUT);     // declare sensor as input
  Serial.begin(115200);


Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
}





 
void loop(){
  val = digitalRead(PIRPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
     // pirState = 1;
      OSCMessage msg("/PIRSensor/1");
        msg.add(pirState);
       Udp.beginPacket(DestIp, DestPort);
        msg.send(Udp);
       Udp.endPacket();
       msg.empty();
       delay(5);
    }
  } else {

    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    //  pirState = 0;
        OSCMessage msg("/PIRSensor/1");
         msg.add(pirState);
       Udp.beginPacket(DestIp, DestPort);
        msg.send(Udp);
       Udp.endPacket();
       msg.empty();
       delay(5);;
      
    }
  }
}
