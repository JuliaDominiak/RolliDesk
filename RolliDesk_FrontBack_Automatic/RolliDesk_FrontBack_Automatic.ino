#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define DEV_I2C Wire
#define SerialPort Serial

VL53L4CX sensor_vl53l4cx_sat(&DEV_I2C, A1);
int distance=0;
int target=0;
int dif = 0;
int del=800;
int inc=100;
int end_point=0;


uint8_t NewDataReady = 0;
int no_of_object_found = 0, j;
char report[64];
int status;


void setup() {
  pinMode(9,OUTPUT); 
  pinMode(8,OUTPUT); 
  pinMode(6,OUTPUT); 
  pinMode(7,OUTPUT); 

  SerialPort.begin(115200);
  SerialPort.println("Starting...");

  DEV_I2C.begin();

  sensor_vl53l4cx_sat.begin();
  sensor_vl53l4cx_sat.VL53L4CX_Off();
  sensor_vl53l4cx_sat.InitSensor(0x12);
  sensor_vl53l4cx_sat.VL53L4CX_StartMeasurement();

}

void loop() {

  VL53L4CX_MultiRangingData_t MultiRangingData;
  VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
  NewDataReady = 0;
  no_of_object_found = 0;
  status = sensor_vl53l4cx_sat.VL53L4CX_GetMeasurementDataReady(&NewDataReady);


  if ((!status) && (NewDataReady != 0)) { 
    status = sensor_vl53l4cx_sat.VL53L4CX_GetMultiRangingData(pMultiRangingData); 
    distance=pMultiRangingData->RangeData[j].RangeMilliMeter;
    Serial.println(distance);
    if (status == 0) { 
      status = sensor_vl53l4cx_sat.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
  }

  if (Serial.available() > 0) { 
    String input = Serial.readString();

    Serial.print("I:");
    Serial.println(input);

    target = input.toInt();
    dif = target - distance;
    end_point=abs(dif*11);

    Serial.print("T:\t");
    Serial.println(target);
    Serial.print("D:\t");
    Serial.println(dif);

    if(dif!=0){
      Serial.println("set target");
      if(dif>0){
        digitalWrite(8,LOW);
        digitalWrite(6,HIGH);
        Serial.println("out");
        delay(200);
      }
      if(dif<0){
        digitalWrite(8,HIGH);
        digitalWrite(6,LOW); 
        Serial.println("to");
        delay(200);
      }

      Serial.flush();

      for(int i=0; i <= end_point; i++){
        digitalWrite(7,HIGH);
        digitalWrite(9,HIGH);
        delayMicroseconds(del);
        digitalWrite(7,LOW);
        digitalWrite(9,LOW);
        delayMicroseconds(del); 
      }

      dif=0;
      target=0;
      end_point=0;
      Serial.print("target!");    

    }
    Serial.flush();

  }

}