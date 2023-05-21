#include "M5Stack.h"
#define buzzer_pin 26
int buzzer_freq = 4000;
int ledChannel = 0;
int resolution = 10;

uint8_t old_status=0;
String status="NO POS";

float knotToKmph(float knot){
  float kmph = knot * 1.852;
  return kmph;
}

void setup() {
  ledcSetup(ledChannel,buzzer_freq,resolution);
  ledcAttachPin(buzzer_pin,ledChannel);
  ledcWrite(ledChannel,512);
  
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1,16,17);
  M5.Lcd.begin();
  M5.Lcd.fillScreen(BLACK);
 // M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextColor(WHITE);
  
}

void loop() {
  if (Serial2.available() > 0 ) {

  String line=Serial2.readStringUntil('\n');
  int i;
  int index=0;
  int len=line.length();
  String str="";
  
//  Serial.println(line);
//  M5.Lcd.print(line);
  String list[15];
  for (i=0;i<15;i++){
    list[i]="";
    }
    for(i=0;i<len;i++){
      if(line[i] == ','){
        list[index++]=str;
        str="";
        continue;
        }
        str +=line[i];
    }
    if (list[0] == "$GNGGA" || list[0] == "$GPGGA"){
      if (list[6] !="0"){
        uint8_t sw=list[6].toInt();
        switch (sw){
          case 1:
          status="Single";
          break;
          case 2:
          status="DGPS";
          break;
          case 3:
          status="3";
          break;
          case 4:
          status="RTK Fix";
          break;
          case 5:
          status="Float";
          break;
          }
          if (sw == 4){
            ledcWrite(ledChannel,0);
          } else {
            ledcWrite(ledChannel,512);
          }
          if (old_status != sw){
            M5.Lcd.fillScreen(BLACK);
          }
          Serial.println(status);
          M5.Lcd.setCursor(0,0);
          M5.Lcd.println(status);
          Serial.println(list[6]);
          old_status = sw;
      }
    }
    if (list[0] == "$GPRMC" || list[0] =="$GNRMC"){
      if(list[7] !="0"){
        float knotValue = list[7].toFloat();
        float kmphValue = knotToKmph(knotValue);
        Serial.println(kmphValue);
      }
    }
  }
}
