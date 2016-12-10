/*
   should add..
   1. button interrupt.
   2. timer value changes.
   3. 
 */


#include<Servo.h>
#define CENTRAL


#ifdef CENTRAL
#define SETTIME 10000000

/* variables */
String readbuf = "";
String mac[4] = {"0xABBAABBAABBA", "0xC4BE84E397A9", "0xAAAAAAAAAAAA", "0xC4BE84E3A786"};
typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;
modeState at_state = NORM_MODE;
int bindOrWait = 0;
int mac_index = 0;
unsigned long t1;
Servo myservo;
int pos = 0;

void openDoor(void){

    for ( pos = 0; pos <= 180; pos += 1) {
        myservo.write(pos);
        delay(15);
    }
    for ( pos = 180; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(15);
    }


}

void setup(void){
    Serial.begin(115200);

    pinMode(2, OUTPUT);     /* Debug led pin*/
    pinMode(4, OUTPUT);     /* Debug led pin*/
    pinMode(5, OUTPUT);     /* Button pin */
    myservo.attach(3);



}

void loop(void){
    char chr;

    switch (bindOrWait){
        case 0:
            digitalWrite(2, LOW);
            digitalWrite(4, HIGH);

            Serial.print("+");
            Serial.print("+");
            Serial.print("+");
            delay(1000);

            if ( mac_index >= 4 ){
                mac_index %= 4;
            }
            Serial.println("AT+BIND=" + mac[mac_index++]); 
            delay(1000);

            Serial.println("AT+RESTART");
            bindOrWait = 1;

            t1 = micros();
            break;

        case 1:
            digitalWrite(2, HIGH);
            digitalWrite(4, LOW); 
            
            while ( micros() - t1 <= SETTIME ){
                while ( Serial.available() > 0 ){
                    chr = Serial.read();
                    readbuf += chr;

                    if ( readbuf.indexOf("open") >= 0 ){

                        digitalWrite(5, HIGH);
                        delay(1000);
                        digitalWrite(5, LOW);
                        delay(1000);
                        digitalWrite(5, HIGH);
                        delay(1000);
                        digitalWrite(5, LOW);

                        openDoor();
                        readbuf = "";
                        bindOrWait = 0;
                        break;
                    } else if ( readbuf.indexOf("hello") >= 0 ){

                        digitalWrite(5, HIGH);
                        delay(100);
                        digitalWrite(5, LOW);
                        delay(100);
                        digitalWrite(5, HIGH);
                        delay(100);
                        digitalWrite(5, LOW);

                        openDoor();
                        readbuf = "";
                        bindOrWait = 0;
                        break;

                    }
                }
                if ( bindOrWait == 0 ){
                    break;
                }
            }
            bindOrWait = 0;
            break;
    }

}
#endif


#ifdef  PERIPHERAL
int buttonPin = 5;
int buttonState = 0;

void setup(void) {
    pinMode(buttonPin, INPUT);
    Serial.begin(115200);
}

void loop(void) {
    buttonState = digitalRead(buttonPin);

    if ( buttenState == HIGH ){
        /* Serial.write("hello");  either of them we use. */
        Serial.write("open");
        delay(100);
    } 
#endif



