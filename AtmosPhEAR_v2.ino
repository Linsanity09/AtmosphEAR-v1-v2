// AtmosphEAR version 2 
// Function: This code reads the pressure value of a pressue transducer over time.  
// Last modified: 09/21/2026

// ---------- CONFIGURATION ----------
const int SENSOR_PIN = A0;
const float ADC_REF_VOLTAGE = 5.0;    
const int   ADC_RESOLUTION  = 1023;  

const float VOLT_MIN = 0.5;            
const float VOLT_MAX = 4.5;            

const float PRESSURE_MIN = 0.0;        // PSI
const float PRESSURE_MAX = 150.0;      // sensor's rated maximum

const unsigned long SAMPLE_INTERVAL_MS = 500;  

unsigned long lastSampleTime = 0;

//setup
void setup() {
  Serial.begin(9600);
  Serial.println("Pressure Transducer Reader Starting...");
  Serial.println("RawADC\tVoltage(V)\tPressure(PSI)");
}

//helper function

// Convert raw ADC count to voltage
float adcToVoltage(int adcValue) {
  return (adcValue * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
}

// Convert voltage to pressure using linear transducer scaling
float voltageToPressure(float voltage) {
  // Clamp voltage to expected range to avoid nonsense readings from noise
  if (voltage < VOLT_MIN) voltage = VOLT_MIN;
  if (voltage > VOLT_MAX) voltage = VOLT_MAX;

  float pressure = (voltage - VOLT_MIN) * (PRESSURE_MAX - PRESSURE_MIN)
                    / (VOLT_MAX - VOLT_MIN) + PRESSURE_MIN;
  return pressure;
}

//main loop
void loop() {
  unsigned long now = millis();

  if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = now;

    int raw = analogRead(SENSOR_PIN);
    float voltage = adcToVoltage(raw);
    float pressure = voltageToPressure(voltage);

    Serial.print(raw);
    Serial.print("\t");
    Serial.print(voltage, 3);
    Serial.print("\t\t");
    Serial.println(pressure, 2);
  }
}

