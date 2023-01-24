#define ANALOG_IN_PIN A0

int RELAY = 3;
float REFERENCE_VOLTAGE = 2.1;
float DISCHARGED_MAX_VOLTAGE = 13.9;
int SIX_MINUTES = 360;

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float inputVoltage = 0.0;

boolean charging = false;

// Floats for resistor values in divider (in ohms)
float R1 = 27000.0;
float R2 = 2750.0;

int adc_value = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  Serial.begin(115200);
  Serial.println("==============================");
  Serial.println("ZS6BVR Smart Charger ");
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
    charge(SIX_MINUTES);

  } else {
    Serial.println("Battery voltage is high");
    Serial.println("=======================");
    Serial.println("We are discharging the battery");
    disharge(SIX_MINUTES);
  }

  Serial.println();
}

void disharge(int delaySeconds) {
  digitalWrite(RELAY, LOW);
  Serial.println("Switched Off Relay");
  int iterations = delaySeconds / 8;
  printDischargeStatus();

  for (int i = 0; i < iterations; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1500);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
  }
}

void charge(int delaySeconds) {
  digitalWrite(RELAY, HIGH);
  Serial.println("Switched On Relay");

  int iterrations = delaySeconds * 4;

  printChargeStatus();

  for (int i = 0; i < iterrations; i++) {  // 1 second per iteration
    digitalWrite(LED_BUILTIN, HIGH);       // turn the LED on (HIGH is the voltage level)
    delay(125);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(125);
  }
}


void printChargeStatus() {
  digitalWrite(RELAY, HIGH);
  Serial.println("Charging");
  printStatus();
}

void printDischargeStatus() {
  digitalWrite(RELAY, LOW);
  Serial.println("Discharging");
  printStatus();
}

void printStatus() {
  Serial.print("Input Voltage = ");
  Serial.println(inputVoltage, 2);

  Serial.print("Timed Delay: ");
  Serial.print(SIX_MINUTES);
  Serial.println(" seconds");


  Serial.println("=--------------------------------");
  Serial.println(" ");
}
