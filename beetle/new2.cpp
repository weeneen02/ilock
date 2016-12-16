/*
   finish
   should add..
   1. button interrupt.
   2. timer value changes.
   3.
 */


#include<Servo.h>
#define PERIPHERAL
#define DEVINUM 1

#ifdef CENTRAL
#define SETTIME 10000000

/* variables */
int buttonPin = 5;
int buttonState = 0;

String readbuf = "";
String mac[2] = {"0xC4BE84E3A786", "0xC4BE84DE3D3F"};
typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;
modeState at_state = NORM_MODE;
int bindOrWait = 0;
int mac_index = 0;
int mode = 0;   /*0 is open, 1 is closed */
unsigned long t1;
Servo myservo;
int pos = 0;

void openDoor(void) {
    mode = 0;
    for ( pos = 0; pos <= 130; pos += 1) {
        myservo.write(pos);
        delay(2);
    }
}

void closeDoor(void) {
    mode = 1;
    for ( pos = 130; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(2);
    }
}

void setup(void) {
    Serial.begin(115200);

    pinMode(2, OUTPUT);     /* Debug led pin*/
    pinMode(4, OUTPUT);     /* Debug led pin*/
    pinMode(5, OUTPUT);     /* Button pin */
    myservo.attach(3);



}

void loop(void) {
    char chr;

    switch (bindOrWait) {
        case 0:

            digitalWrite(2, LOW);
            digitalWrite(4, HIGH);
            /*
               Serial.print("+");
               Serial.print("+");
               Serial.print("+");
               delay(1000);

               if ( mac_index >= DEVINUM ) {
               mac_index %= DEVINUM;
               }

               Serial.println("AT+BIND=" + mac[mac_index++]);
               delay(1000);

               Serial.println("AT+RESTART");
               delay(5000);
             */
            bindOrWait = 1;

            t1 = micros();
            break;

        case 1:
            readbuf = "";
            /* send data to peri*/
            Serial.flush();
            digitalWrite(2, HIGH);
            digitalWrite(4, LOW);
            Serial.println(mode);  /*0 is open 1 is closed*/
            Serial.flush();

            while ( micros() - t1 <= SETTIME ) {
                buttonState = digitalRead(buttonPin);
                if (buttonState == HIGH) {
                    if ( mode == 0 ) {
                        closeDoor();
                    } else if ( mode == 1 ) {
                        openDoor();
                    }
                }
                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    readbuf += chr;

                    if ( readbuf.indexOf("open") >= 0 ) {
                        if ( mode == 0 ) {
                            /* already open. */
                            closeDoor();

                        } else if ( mode == 1 ) {
                            openDoor();

                        }

                        readbuf = "";
                        if ( micros() - t1 >= SETTIME ) {
                            bindOrWait = 0;
                            break;
                        }
                    } 
                    /*else if ( readbuf.indexOf("close") >= 0 ) {
                      if ( mode == 1 ) {

                      } else if ( mode == 0 ) {
                      closeDoor();

                      }
                      readbuf = "";

                      if ( micros() - t1 >= SETTIME ) {
                      bindOrWait = 0;
                      break;
                      }
                      }
                     */
                }
                if ( bindOrWait == 0 ) {
                    break;
                }
            }
            bindOrWait = 0;
            break;
    }
}
#endif


#ifdef  PERIPHERAL
int openPin = 5;
int openButState = 0;

int mode = 0;
String readBuf = "";
char chr = 'v';

void setup(void) {
    pinMode(openPin, INPUT);

    Serial.begin(115200);
}

void loop(void) {
    openButState = digitalRead(openPin);

    chr = 'v';

    while (Serial.available() > 0 ) {
        chr = Serial.read();
    }

    if (chr == 'v') {
        /*nothing to come*/
    } else if ( chr == '1' ) {
        /*The door is now close state*/
    } else if ( chr == '0' ) {
        /*The door is now open state*/

    }

    if ( openButState == HIGH ) {
        /* Serial.write("hello");  either of them we use. */
        Serial.write("open");
        delay(1000);
    } 
    /*else if ( closeButState == HIGH ) {
      Serial.write("close");
      delay(1000);
      }
     */
}

#endif
