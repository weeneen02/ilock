// Bluno beetle 현관문.
// Raspberry pi, Bluno beetle, Phone 로 부터 받는다. 

#include <SoftwareSerial.h>

#define OPEN_PIN 3
#define CLOSE_PIN 4

SoftwareSerial mySerial(0, 1);  //Bluetooth Connection.

String anymessage[50]; // get message 
int i = 0; 
struct Device {
    unsigned int ID;  // Device ID.
    int connection;   // 0 is not connected, 1 is connected.
    char message[20]; // from RP3
    char passcode[10]; // passcode from Raspberry pi3 
    char auth[10]; // to authenticate message
};

typedef struct Device DEVICE;

void setup(){
    DEVICE devi[10]; // total 10 devices.


    Serial.begin(115200);  // Board rate 115200
    mySerial.begin(115200); // Bluetooth board rate 115200

    delay(5000);
    /*
       if (Serial.available()){
       Serial.write(Serial.read());

       }
     */
    Serial.write("hello\n");
    Serial.write("+++");    // enter AT mode 
    Serial.write("AT+MAC=?\r\n");   //맥 주소를 확인한다.
    Serial.write("AT+BIND=?\r\n"); // 현재 바인딩 상황을 화인

    delay(5000);

    //Serial.write("myserial\n");
    //Serial.write(Serial.read());
    //mySerial.write("+++");
    delay(5000);
    //Serial.write(mySerial.read());

}

void loop(){
    while (mySerial.available()) {  // device 로부터 메세지를 받을 때.
        anymessage = mySerial.read();   // get message from devi


        while (check (anymessage, Devi[i].message)); // 이 함수는 아무 Device에서 받은 메세지와 기존의 가지고 있는
        // Devi 구조체의 message와 대조하여 인증된 사용자인지 확인한다.
        if (res == 1){   //                                       
            mySerial.write("error");            // message가 맞지 않으면 해당 디바이스에 error 메세지 전송.
            pinMode(CLOSE_PIN, OUTPUT);         // 문을 열지 않도록 한다.
        }

        else {                                // 맞는 디바이스가 있을 때.
            Serial.println("Devi[i].ID" + Devi[i].ID);
            Serial.println("open!");
            pinMode(OPEN_PIN, OUTPUT);          // OPEN the door.
        }
    }
}

