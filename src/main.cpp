// https://github.com/marcosaraujomvma/proj-sprayed-smart



#define flow_PIN1 2  // Flow Sensor Pin 1
#define flow_PIN2 3  // Flow Sensor Pin 2
#define flow_PIN3 4  // Flow Sensor Pin 3
#define flow_PIN4 5  // Flow Sensor Pin 4
#define pressure_PIN 7 // Pressure Sensor Pin
#define level_PIN 7 // Level Sensor Pin
#define flowRateConverter 0.078201369 // Convert from 0-1023 to 0-80 L/min -> 80.0 / 1023 = 0.078201369 (L/min)
#define pressureRateConverter 0.006739775 //Convert from 0-1023 to 0-100 Bar -> (100/1023) * 0.0689476 (Bar)
#define UPPER_LIMIT 18.5 // Upper limit for reading flow sensors L/min
#define LOWER_LIMIT 17.0 // LOWER limit for reading flow sensors L/min



float pressure, flow1, flow2, flow3, flow4, level;


void setup() {
  Serial.begin(9600);
  pinMode(flow_PIN1, INPUT);
  pinMode(flow_PIN2, INPUT);
  pinMode(flow_PIN3, INPUT);
  pinMode(flow_PIN4, INPUT);
  pinMode(pressure_PIN, INPUT);
  pinMode(level_PIN, INPUT); 
}

void loop() {

  flow1 = (map(analogRead(flow_PIN1),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow1 > UPPER_LIMIT || flow1 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 01 = ");
  Serial.print(flow1);
  }
  delay(1000); // wait 1s

  flow2 = (map(analogRead(flow_PIN2),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow2 > UPPER_LIMIT || flow2 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 02 = ");
  Serial.print(flow2);
  }
  delay(1000); // wait 1s

  flow3 = (map(analogRead(flow_PIN3),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow3 > UPPER_LIMIT || flow3 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 03 = ");
  Serial.print(flow3);
  }
  delay(1000); // wait 1s

  flow4 = (map(analogRead(flow_PIN4),0,1023,0,255)) * flowRateConverter; //Flow L/mim
  if (flow4 > UPPER_LIMIT || flow4 < LOWER_LIMIT){
    Serial.print("flow out of limit");
  }else{
  Serial.print("Flow Sensor 04 = ");
  Serial.print(flow4);
  }
  delay(1000); // wait 1s

  pressure = (map(analogRead(pressure_PIN),0,1023,0,255)) * pressureRateConverter; // Pressure Bar
  Serial.print("Pressure = ");
  Serial.print(pressure);
  delay(1000); // wait 1s

  level = map(analogRead(level_PIN),0,100,0,255)
  Serial.print("Level = ");
  Serial.print(level);
  delay(1000); // wait 1s


}
