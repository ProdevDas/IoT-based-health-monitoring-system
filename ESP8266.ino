#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = "***********";   // your network SSID (name) 
char pass[] = "***********";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = ***********;
const char * myWriteAPIKey = "***********";

// Initialize our values
float h=0;
float t=0;


void setup() {
  Serial.begin(9600);  // Initialize serial
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  //Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  if (Serial.available() > 0)
  {
    String data=Serial.readStringUntil('\n');
    Serial.println(data);
    int comma=data.indexOf(','); 
    if(comma != -1){
      h=data.substring(0,comma).toFloat();
      t=data.substring(comma+1).toFloat();
      Serial.print(h);
      Serial.println(t);
    }
  }

  // set the fields with the values
  ThingSpeak.setField(1, String(h));
  ThingSpeak.setField(2, String(t));
  
  //write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(15000); // Wait 15 seconds to update the channel again
}
