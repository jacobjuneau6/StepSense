//No bluetooth
//beep at 3.5m distance
//stop beeping after backing away > 4m
//sleep when battery reaches 3.7v
#include <Wire.h>
#include <VL53L1X.h>
#define SDA_PIN D8
#define SCL_PIN D9
VL53L1X distance;
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);   //battery sense pin
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!distance.init()) {
    Serial.println("VL53L1X not connected");
    while (1);
  }
  distance.setDistanceMode(VL53L1X::Long);
  distance.setMeasurementTimingBudget(50000);
  distance.startContinuous(50);
  pinMode(D7, OUTPUT);
}

void loop() {
  //battery checks 
  uint32_t Vbatt = 0;
  for (int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(A0);
  }
  float Vbattf = 2 * Vbatt /16 / 1000.0;
  Serial.println(Vbattf, 3);
  if (Vbattf < 3.8){
    esp_deep_sleep_start();
  } else {}
  //read VL53L1X
  int measurement = distance.read();
  Serial.print("Distance: ");
  Serial.println(measurement);
  if (measurement < 3) {
    digitalWrite(D7, HIGH);
  } else {
    digitalWrite(D7, LOW);
  }

}
