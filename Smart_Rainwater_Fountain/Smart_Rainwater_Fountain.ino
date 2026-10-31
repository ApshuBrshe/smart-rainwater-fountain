#define TdsSensorPin A1
#define WaterSensorPin A7
#define earthPin A0
#define pomp1 3
#define pomp2 2  // Pin assigned to pump 2
#define VREF 5.0          
#define SCOUNT 30       

int analogBuffer[SCOUNT];   
int analogBufferIndex = 0;  
float averageVoltage = 0;   
float tdsValue = 0;         
float temperature = 25.0;   

bool pump2Active = false;
unsigned long pump2StartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TdsSensorPin, INPUT);
  pinMode(WaterSensorPin, INPUT);
  pinMode(earthPin, INPUT);
  pinMode(pomp1, OUTPUT);
  pinMode(pomp2, OUTPUT);

  digitalWrite(pomp1, HIGH); // Pump 1 starts enabled
  digitalWrite(pomp2, LOW);  // Pump 2 starts disabled
}

void loop() {
  static unsigned long analogSampleTime = millis();
  static unsigned long printTime = millis();
  // Pump 1 control
  int waterLevel = analogRead(WaterSensorPin);
  if (waterLevel > 100){
    digitalWrite(pomp1, 0);  // Turn off the pump
  } else {
    digitalWrite(pomp1, 1); // Turn on the pump
  }

  if (analogRead(A0) > 600) {
    digitalWrite(2, 1);
  }
  else {
    digitalWrite(2, 0);
    }
  // Read and process TDS sensor data
  if (millis() - analogSampleTime > 40) {
    analogSampleTime = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;  
    }
  }

  if (millis() - printTime > 1000) {
    printTime = millis();
    averageVoltage = getMedianVoltage();  

    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);  
    float compensationVoltage = averageVoltage / compensationCoefficient;
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage 
               - 255.86 * compensationVoltage * compensationVoltage 
               + 857.39 * compensationVoltage) * 0.5; 

    Serial.print("Voltage: ");
    Serial.print(averageVoltage);
    Serial.print("V  TDS: ");
    Serial.print(tdsValue);
    Serial.println(" ppm");

    checkWaterQuality(tdsValue);
  }
}

float getMedianVoltage() {
  int sortedBuffer[SCOUNT];
  memcpy(sortedBuffer, analogBuffer, SCOUNT * sizeof(int));
  for (int i = 0; i < SCOUNT - 1; i++) {
    for (int j = i + 1; j < SCOUNT; j++) {
      if (sortedBuffer[i] > sortedBuffer[j]) {
        int temp = sortedBuffer[i];
        sortedBuffer[i] = sortedBuffer[j];
        sortedBuffer[j] = temp;
      }
    }
  }
  int medianValue = sortedBuffer[SCOUNT / 2];  
  return (medianValue * VREF) / 1024.0;  
}

void checkWaterQuality(float tds) {
  if (tds < 50) {
    Serial.println("⚠️ Very low TDS level. Consider adding nutrients for plants.");
  } else if (tds >= 50 && tds <= 150) {
    Serial.println("✅ Excellent rainwater quality for irrigation!");
  } else if (tds > 150 && tds <= 300) {
    Serial.println("✅ Good water quality.");
  } else if (tds > 300 && tds <= 500) {
    Serial.println("⚠️ High TDS level. Use with caution.");
  } else {
    Serial.println("❌ Very high TDS level. Not recommended for irrigation!");
  }
}
