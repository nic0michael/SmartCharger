#define ANALOG_IN_PIN A0

int RELAY = 3;
float REFERENCE_VOLTAGE = 2.1;
float DISCHARGED_MAX_VOLTAGE = 13.9;  

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float inputVoltage = 0.0;

boolean charging = false;

// Floats for resistor values in divider (in ohms)
float R1 = 27000.0;
float R2 = 2510.0;


int chargeDelay = 1;  // start with 15 seconds
int dischargeCount = 0;
int adc_value = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  Serial.begin(115200);
  Serial.println("==============================");
  Serial.println("ZS6BVR Smart Charger ");
  chargeDelay = 1;
}

void loop() {

  delay(500);
  Serial.println("==============================");
  Serial.println(" ");

  adc_value = analogRead(ANALOG_IN_PIN);

  // Determine voltage at ADC input
  adc_voltage = (adc_value * REFERENCE_VOLTAGE) / 1024.0;

  // Calculate voltage at divider input
  inputVoltage = adc_voltage / (R2 / (R1 + R2));


  // Print results to Serial Monitor to 2 decimal places

  if (inputVoltage <= DISCHARGED_MAX_VOLTAGE) {
    Serial.println("Battery voltage is low");
    Serial.println("======================");
    charging = true;
    charge(chargeDelay);
    chargeDelay++;

    if (chargeDelay > 4) {
      chargeDelay = 1;
    }

  } else {
    Serial.println("Battery voltage is high");
    Serial.println("=======================");
    Serial.println("We are discharging the battery");
    dischargeCount++;
    disharge();

  }

  Serial.println();
}

void disharge() {
  digitalWrite(RELAY, LOW);
  Serial.println("Switched Off Relay");

  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1500);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
    printDischargeStatus();
  }
}

void charge(int chargeDelay) {
  digitalWrite(RELAY, HIGH);
  Serial.println("Switched On Relay");

  int iterrations = makeIterrations(chargeDelay);

  for (int i = 0; i < iterrations; i++) {  // 1 second per iteration
    digitalWrite(LED_BUILTIN, HIGH);       // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    printChargeStatus();
  }
}

void chargeWithHighVoltage(int chargeDelay) {

  digitalWrite(RELAY, HIGH);
  Serial.println("High Voltage Charge : Switched On Relay");

  int iterrations = makeIterrations(chargeDelay);

  for (int i = 0; i < iterrations; i++) {  // 1 second per iteration
    digitalWrite(LED_BUILTIN, HIGH);       // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
    printChargeStatus();
  }

}

int makeIterrations(int chargeDelay) {
  int iterrations = 1;

  switch (chargeDelay) {
    case 1:
      iterrations = 10;
      break;

    case 2:
      iterrations = 20 * 60;
      break;

    case 3:
      iterrations = 20;
      break;

    case 4:
      iterrations = 40 * 60;
      break;

    default:
      iterrations = 60;
  }

  return iterrations;
}

void printChargeStatus(){
  digitalWrite(RELAY, HIGH);
  Serial.println("Charging");
  printStatus();
}

void printDischargeStatus(){
  digitalWrite(RELAY, LOW);
  Serial.println("Discharging");
  printStatus();
}

void printStatus(){
  Serial.print("Input Voltage = ");
  Serial.println(inputVoltage, 2);

  Serial.print("chargeDelay:");
  Serial.println(chargeDelay);

  Serial.print("dischargeCount");
  Serial.println(dischargeCount);
  Serial.println("=--------------------------------");
  Serial.println(" ");
}
