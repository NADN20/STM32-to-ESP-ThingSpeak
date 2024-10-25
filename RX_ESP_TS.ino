#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>

// Define RX and TX pins
const int RX_PIN = 1; 
const int TX_PIN = 3; 

// WiFi credentials
const char* ssid = "globalnet";   
const char* password = "70937017"; 

SoftwareSerial mySerial(RX_PIN, TX_PIN);

WiFiClient client;
unsigned long myChannelNumber = 2709690; 
const char* myWriteAPIKey = "DIBRXQ72D1XNSIN8"; // Replace with your ThingSpeak write API key

void setup() {
  Serial.begin(9600);     
  mySerial.begin(9600);   

  delay(10);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi!");
  ThingSpeak.begin(client);
}

void loop() {
  if (mySerial.available()) {
    String receivedString = mySerial.readStringUntil('\n'); 
    Serial.print("Received: ");
    Serial.println(receivedString);
    int valueToSend = receivedString.toInt(); 
    int responseCode = ThingSpeak.writeField(myChannelNumber, 1, valueToSend, myWriteAPIKey); 
    
    delay(100); 
  }
}
