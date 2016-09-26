/*
========== Arduino EEPROM Eraser ==========
 이 프로그램은 아두이노에 내장된 EEPROM을 초기화 시켜주는 프로그램 입니다. 
제로필 방식으로 초기화를 진행합니다. 
제작자 : 어벤저X (http://avengerhard.tistory.com)
코드버전 : 1.0v
*/
#include <EEPROM.h> //EEPROM Access

//===== 환경설정 =====
byte Erase_value = 0; //초기화값
int Erase_type = 2; //초기화 타입
//====================
int tmp=0; //중복 초기화 방지

void setup() {
  //작업상태를 보여주기 위한 시리얼 모니터를 이용
  Serial.begin(9600); //Serial Speed (Custom)
}

void loop() {
  int max_num;
  
  if(tmp == Erase_value){
    Serial.println("Arduino EEPROM Eraser (1.0v)");
    Serial.println("Program State : OK!");
    
    delay(3000);
    
    if(Erase_type == 1)
      part_Erase();
    else if(Erase_type == 2)
      full_Erase();
  }
}

void part_Erase()
{
  int num;
  byte tmp_data;
  Serial.println("Erase Start!  (Erase Mode : PART Mode)");
  
  for(num=0; num<EEPROM.length(); num++){
    tmp_data = EEPROM.read(num);
    if(tmp_data != 0){ //해당 주소값이 NULL이 아니라면 초기화
      EEPROM.write(num, Erase_value);
      Serial.println("Erase! EEPROM Address : " + String(num) + "/" + String(EEPROM.length()-1));
    }
  }
  
  tmp=1; //중복 초기화 방지
  Serial.println("Erase Success!");
}

void full_Erase()
{
  int num;
  Serial.println("Erase Start!  (Erase Mode : FULL Mode)");
  
  for(num=0; num<EEPROM.length(); num++){
    EEPROM.write(num, Erase_value);
    Serial.println("Erase! EEPROM Address : " + String(num) + "/" + String(EEPROM.length()-1));
  }
  
  tmp=1; //중복 초기화 방지
  Serial.println("Erase Success!");
}

