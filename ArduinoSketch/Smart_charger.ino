#define ANALOG_IN_PIN A0



// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

float previousVoltage = 0.0;

boolean charging = false;
boolean shouldCharge = false;
boolean shouldDischarge = false;
boolean relayIsOn = false;

// Floats for resistor values in divider (in ohms)
float R1 = 27000.0;
float R2 = 2300.0;

// Float for Reference Voltage
float ref_voltage = 2.1;

float dischargedVoltage = 12.9;
float runningVoltage = 13.8;
float fullyChargedVoltage = 14.9;
int chargeDelay = 1;  // start with 15 seconds
int dischargeCount = 0;
// Integer for ADC value
int adc_value = 0;
int relay = 3;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
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
  adc_voltage = (adc_value * ref_voltage) / 1024.0;

  // Calculate voltage at divider input
  in_voltage = adc_voltage / (R2 / (R1 + R2));


  // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);

  Serial.print("chargeDelay:");
  Serial.println(chargeDelay);

  Serial.print("dischargeCount");
  Serial.println(dischargeCount);

  if (dischargeCount > 187) {  //evry 55 minutes (225 =1hr)
    dischargeCount = 0;
    chargeDelay = 2;  // charge for 20 minutes
    charge(chargeDelay);
  } else if (in_voltage <= dischargedVoltage) {
    charging = true;
    charge(chargeDelay);
    chargeDelay++;
    if (chargeDelay > 4) {
      chargeDelay = 1;
    }

  } else if (in_voltage >= fullyChargedVoltage) {
    charging = false;
    dischargeCount++;
    disharge();

  } else if (in_voltage > runningVoltage) {
    charging = false;
    dischargeCount++;
    disharge();
  } else {
    Serial.println("In between values");
    dischargeCount++;
    disharge();
  }

  Serial.println();
}

void disharge() {
  digitalWrite(relay, LOW);
  Serial.println("Switched Off Relay");

  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1500);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
  }
}

void charge(int chargeDelay) {
  digitalWrite(relay, HIGH);
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
  }
}

int makeIterrations(int chargeDelay) {
  int iterrations = 1;

  switch (chargeDelay) {
    case 1:
      iterrations = 15;
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
