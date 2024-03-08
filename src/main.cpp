// https://github.com/marcosaraujomvma/proj-sprayed-smart
//#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Editado-Gabriel
#include <WiFi.h>
#include "HTTPClient.h"
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
String serverName = "http://192.168.1.106:1880/update-sensor";
//Fim-Editado-Gabriel

//flow sensor
#define flow_PIN1 2  // Flow Sensor Pin 1
#define flow_PIN2 3  // Flow Sensor Pin 2
#define flow_PIN3 4  // Flow Sensor Pin 3
#define flow_PIN4 5  // Flow Sensor Pin 4
//flow sensor end
#define pressure_PIN 6 // Pressure Sensor Pin
#define level_PIN 7 // Level Sensor Pin
#define flowRateConverter 0.078201369 // Convert from 0-1023 to 0-80 L/min -> 80.0 / 1023 = 0.078201369 (L/min)
#define pressureRateConverter 0.006739775 //Convert from 0-1023 to 0-100 Bar -> (100/1023) * 0.0689476 (Bar)
#define UPPER_LIMIT 18.5 // Upper limit for reading flow sensors L/min
#define LOWER_LIMIT 17.0 // LOWER limit for reading flow sensors L/min

#define trigPin1 8
#define echoPin1 9
#define trigPin2 10
#define echoPin2 11
#define trigPin3 12
#define echoPin3 13
#define pulseDuration 0.0034 //  constant representing the time it takes for sound to travel one centimeter in the air

//GPS Sensor
#define RXPin 14;
#define TXPin 15;

static const uint32_t GPSBaud = 9600;   // Baudrate of the GPS module


//start Variables
float pressure, flow1, flow2, flow3, flow4, level;
long duration1, distance1, duration2, distance2, duration3, distance3;


//Start GPS use

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial object to communicate with the GPS module
SoftwareSerial ss(RXPin, TXPin);

//Editado-Gabriel
void sendMessage(String typeData, float floatValue, long longValue, bool isFloat) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      if (isFloat){
        String serverPath = serverName + "?"+ typeData + "=" + String(floatValue);
      }else{
        String serverPath = serverName + "?"+ typeData + "=" + String(longValue);
      }
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
//Fim-Editado-Gabriel


void setup() {
  Serial.begin(9600);
  pinMode(flow_PIN1, INPUT);
  pinMode(flow_PIN2, INPUT);
  pinMode(flow_PIN3, INPUT);
  pinMode(flow_PIN4, INPUT);
  pinMode(pressure_PIN, INPUT);
  pinMode(level_PIN, INPUT); 

  //Sensor localization
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  ss.begin(GPSBaud); 

//Editado-Gabriel
WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
//FIM-Editado-Gabriel

}

void loop() {

  //Sensors Block
  flow1 = (map(analogRead(flow_PIN1),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow1 > UPPER_LIMIT || flow1 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 01 = ");
  Serial.print(flow1);
  sendMessage("flow1", flow1, 0, true);
  }
  delay(1000); // wait 1s

  flow2 = (map(analogRead(flow_PIN2),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow2 > UPPER_LIMIT || flow2 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 02 = ");
  Serial.print(flow2);
  sendMessage("flow2", flow2, 0, true);
  }
  delay(1000); // wait 1s

  flow3 = (map(analogRead(flow_PIN3),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow3 > UPPER_LIMIT || flow3 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 03 = ");
  Serial.print(flow3);
  sendMessage("flow3", flow3, 0, true);
  }
  delay(1000); // wait 1s

  flow4 = (map(analogRead(flow_PIN4),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow4 > UPPER_LIMIT || flow4 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 04 = ");
  Serial.print(flow4);
  sendMessage("flow4", flow4, 0, true);
  }
  delay(1000); // wait 1s

  pressure = (map(analogRead(pressure_PIN),0,1023,0,255)) * pressureRateConverter; // Pressure Bar
  Serial.print("Pressure = ");
  Serial.print(pressure);
  sendMessage("pressure", pressure, 0, true);
  delay(1000); // wait 1s

  level = map(analogRead(level_PIN),0,100,0,255)
  Serial.print("Level = ");
  Serial.print(level);
  sendMessage("level", level, 0, true);
  delay(1000); // wait 1s

  //Sensors Block End


  //Sensors Localization block

  digitalWrite(trigPin1, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * pulseDuration / 2;
  
  digitalWrite(trigPin2, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * pulseDuration / 2;

  digitalWrite(trigPin3, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * pulseDuration / 2;

  Serial.print("Distance from Sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  sendMessage("distance1", 0, distance1, false);

  Serial.print("Distance from Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  sendMessage("distance2", 0, distance2, false);

  Serial.print("Distance from Sensor 3: ");
  Serial.print(distance3);
  Serial.println(" cm");
  sendMessage("distance3", 0, distance3, false);
  //Sensors localization Block end

  //Relative Position Block
  if (distance1 < distance2 && distance1 < distance3) {
    Serial.println("Near Sensor 1");
  } else if (distance2 < distance1 && distance2 < distance3) {
    Serial.println("Near Sensor 2");
  } else if (distance3 < distance1 && distance3 < distance2) {
    Serial.println("Near Sensor 3");
  } else {
    Serial.println("No position");
  }

  float x = (distance1 * distance1 - distance2 * distance2 + 10000) / 200;
  float y = (distance1 * distance1 - distance3 * distance3 + 10000) / 200;

  Serial.print("Relative Position (x, y): ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
  // Relative Position Block
  delay (1000)

  //GPS Position Block

  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (gps.location.isValid()) {
        // Get latitude and longitude
        double latitude = gps.location.lat();
        double longitude = gps.location.lng();

        // Print latitude and longitude
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
      } else {
        Serial.println("GPS signal not valid");
      }
    }
  }

  //GPS Position Block end

}