//#define ENCODER_DO_NOT_USE_INTERRUPTS
const int heightFreq = 10000;
int heightCount = 0;
int del = 1000; 
int distance;
#include <Encoder.h>

Encoder myEnc(12,13);

#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h>

#define ledPinPlus 8
#define ledPinMinus 9


float counter = 0; 
int aState;
int aLastState;  
float target = 0;
long lastChangeTime;
String a = "aaaaa";

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //  M0/ESP32/ESP8266/mega2560/Uno/Leonardo

void setup() { 

  u8g2.begin();           
  u8g2.setFontPosTop(); 
  u8g2.enableUTF8Print();   

  pinMode(ledPinMinus, OUTPUT);
  pinMode(ledPinPlus, OUTPUT);
  pinMode(9,OUTPUT); 
  pinMode(8,OUTPUT); 
  pinMode(6,OUTPUT); 
  pinMode(7,OUTPUT); 

  Serial.begin (9600);
  aLastState = myEnc.read()/4;  

  u8g2.setFont(u8g2_font_fur20_tf); 

  u8g2.clearBuffer();         
  u8g2.setCursor(/* x=*/0, /* y=*/5); 
  u8g2.print(String(99));  
  u8g2.sendBuffer();
  delay(1000);

} 

void loop() { 
  
  aState = myEnc.read()/4;
  if (aState != aLastState){ 
    if ((aState-aLastState)>0){counter=counter+1;}
    if ((aState-aLastState)<0){counter=counter-1;}

    Serial.print("Position: ");
    Serial.println(counter);
    aLastState = aState;
    lastChangeTime = millis(); 
  }  

  heightCount++;
  
  if (heightCount>heightFreq){
    heightCount = 0;
    u8g2.clearBuffer();         
    u8g2.setCursor(/* x=*/0, /* y=*/5); 
    u8g2.print(String(counter));  
    u8g2.sendBuffer();
  }
  
  if (millis() - lastChangeTime >= 1000 && counter != 0) {

  target=counter;
  Serial.print("Target: ");
  Serial.println(target);
  distance=abs(target*116);

  if (target > 0) {
    digitalWrite(8,HIGH);
    digitalWrite(6,LOW);      
  } 
  else {
    digitalWrite(8,LOW);
    digitalWrite(6,HIGH);
  }
        
  for(int i=0; i <= distance; i++){
    digitalWrite(7,HIGH);
    digitalWrite(9,HIGH);
    delayMicroseconds(del);
    digitalWrite(7,LOW);
    digitalWrite(9,LOW);
    delayMicroseconds(del); 
  }

  lastChangeTime = millis(); 
    target=0; 
    counter=0;       
    distance=0;     
  }
  
}
