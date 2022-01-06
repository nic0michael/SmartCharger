/*
 * 
 * NO WORKING YET problem with voltage sensor
 */
int DISABLE_CHARGER=1;
int CHARGING_BATTERY=0;

int ledPin=13;
int relayPin = 3;// Define the pin that switches on the relay
int tempSensPin = 2; //Thermistor NTC Resistor analog Pin
int batVoltPin = 1; //the analog pin of the Battery Voltage Sensor

float batteryVoltage= 0.0;
float batteryMaxVoltage= 0.0;
float batteryMinVoltage= 0.0;
float tempSenseVoltage= 0.0;



void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(relayPin, OUTPUT); //sets the relay pin to output
   Serial.begin(9600); // begin sending over serial port
}

void loop() {
  // the main code here will run repeatedly:
  Serial.println(" ");
  setBatteryMaxVoltage();
  setBatteryMinVoltage();

  Serial.print("  batteryMinVoltage ");
  Serial.print(batteryMinVoltage);

  Serial.print("  batteryMaxVoltage ");
  Serial.print(batteryMaxVoltage);
  
  float batteryLevel=analogRead(batVoltPin); //Read the battery voltage sensor level
  Serial.print("  batteryLevel ");
  Serial.print(batteryLevel);
  batteryVoltage= batteryLevel*15; //  float voltage = sensorValue * (5.00 / 1023.00) * 2;/1.123
  Serial.print("  batteryVoltage ");
  Serial.print(batteryVoltage);


  
  float tempSenseLevel=analogRead(tempSensPin); //Read the temperature sensor voltage level
  tempSenseVoltage = (tempSenseLevel*50) /2550;
  Serial.print("  tempSenseVoltage ");
  Serial.print(tempSenseVoltage);

    
  if(batteryVoltage>=batteryMaxVoltage ){
    stopChargingBattery();
    
  }  
  
  if(batteryVoltage<=batteryMinVoltage ){
    startChargingBattery();
    
  } 
  
  if(batteryVoltage<batteryMaxVoltage && batteryVoltage>batteryMinVoltage ){
    pulseOffLED();
    Serial.println(" ");
    Serial.println(" -------------> Battery is Normal");
    CHARGING_BATTERY=0;
  }
  
  Serial.print("  CHARGING_BATTERY ");
  Serial.print(CHARGING_BATTERY);
  
  Serial.println(" ------------------------------------ ");
  if(CHARGING_BATTERY==0){
    delay(500); // 5 minutes
  } else {
    delay(1000); // 2 minutes
  }
}


void startChargingBattery(){
  pulseOnLED();
  pulseOnLED();
  pulseOnLED();
  if( CHARGING_BATTERY ==0){
    CHARGING_BATTERY=1; 
    Serial.println(" ");
    Serial.println(" -------------> Started Charging");
  } else{
    Serial.println(" ");
    Serial.println(" -------------> Continue Charging");
    
  }
  if(DISABLE_CHARGER==0){
    digitalWrite(relayPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
}

void stopChargingBattery(){  
  pulseOffLED();
  digitalWrite(relayPin, LOW);   // turn the LED on (LOW is the voltage level) 
  if( CHARGING_BATTERY ==1){
  CHARGING_BATTERY=0;
  Serial.println(" ");
  Serial.println(" -------------> Stopped Charging");
  } else{
  Serial.println(" ");
  Serial.println(" -------------> Discontinue Charging");
  }   
}


void pulseOnLED(){
  digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300); 
}

void pulseOffLED(){
   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
  digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
}


void setBatteryMaxVoltage(){
  batteryMaxVoltage=15344; // 13.8V
}

void setBatteryMinVoltage(){
  batteryMinVoltage=15241; // 12.8V
}
