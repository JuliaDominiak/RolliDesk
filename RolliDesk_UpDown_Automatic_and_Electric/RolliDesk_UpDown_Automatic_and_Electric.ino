#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

const int Pin1 = 11;
const int Pin2 = 10;

int16_t distance;

int height=0;
int target=0;
int dif;
int more = 102;

void setup() {
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);

  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("Adafruit VL53L1X sensor demo"));

  Wire.begin();
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }

  Serial.println(F("Ranging started"));

  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());
  delay(1000);
   if (vl53.dataReady()) {
    height = vl53.distance()+more;
    if (height == -1) {
      return;
    }
    vl53.clearInterrupt();
  }

  Serial.println(height);
}

const int heightFreqHoming = 100;
const int heightFreq = 1000;
int heightCount = 0;

void loop() {

  if (vl53.dataReady()) {
    height = vl53.distance() + more;
    if (height == -1) {
      return;
    }
    vl53.clearInterrupt();
  }
  heightCount++;
  if (heightCount>heightFreq || (dif!= 0 && heightCount>heightFreqHoming)){
    heightCount = 0;
    Serial.print("H:\t");
    Serial.println(height);
  }

  if (Serial.available() > 0) {
    String input = Serial.readString();
    Serial.print("I:");
    Serial.println(input);
    target = input.toInt();
    dif = target - height;
    Serial.print("T:\t");
    Serial.println(target);
    Serial.print("D:\t");
    Serial.println(dif);
    if(dif!=0){
      Serial.println("target set");
      if(dif>0){
        digitalWrite(Pin1, HIGH);
        digitalWrite(Pin2, LOW);
        Serial.println("up");
      }
      if(dif<0){
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, HIGH);
        Serial.println("down");
      } 
    }
    Serial.flush();
  }

  if(height>=(target-6) && dif>0){
    digitalWrite(Pin1, LOW);
    dif=0;
    Serial.println("target 1");
    Serial.println(height);
    delay(1000);
  }

  if(height<=(target+12) && dif<0){
    digitalWrite(Pin2, LOW);
    dif=0;
    Serial.println("target 2");
    Serial.println(height);
    delay(1000);
  }

}
