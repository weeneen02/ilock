/*
   Project Name : Ilock
Authors      : 임재환, 문지인
Functions    : Operating the servo motor when specific conditionals meet.
1. Pairing with Bluno beetles
2. Analyze the Pubilc key
3. Open the Door or Close it.

Note         :
DeadLine     : 2016_12_18   
 */


#include<Servo.h>
#define CENTRAL
#define DEVINUM 2

/*-------------------CENTRAL BLUNO-----------------------*/
#ifdef CENTRAL      
#define SETTIME 10000000

/* variables */
String readbuf = "";
//String mac[DEVINUM] = {"0xC4BE84E3A786"};
typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;
modeState at_state = NORM_MODE;
int bindOrWait = 0;
int mac_index = 0;
int mode = 0;   /*0 is open, 1 is closed */
unsigned long t1;
Servo myservo;
int pos = 0;

void openDoor(void) {
    for ( pos = 0; pos <= 180; pos += 1) {
        myservo.write(pos);
        delay(5);
    }
}

void closeDoor(void) {
    for ( pos = 180; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(5);
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
            bindOrWait = 1;
            t1 = micros();
            break;

        case 1:
            /* send data to peri*/
            digitalWrite(2, HIGH);
            digitalWrite(4, LOW);
            Serial.println(mode);  /*0 is open 1 is closed*/
            Serial.flush();

            while ( micros() - t1 <= SETTIME ) {
                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    readbuf += chr;

                    if ( readbuf.indexOf("open") >= 0 ) {
                        if ( mode == 0 ) {
                            /* already open. */
                            openDoor();
                            mode = 1;

                        } else if ( mode == 1 ) {
                            closeDoor();
                            mode = 0;
                        }

                        readbuf = "";
                        if ( micros() - t1 <= SETTIME ){
                            bindOrWait = 0;
                            break;
                        }
                    } 
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

/*--------------------------------END--------------------------------*/

/*------------------------PERIPHERIAL BLUNO--------------------------*/
#ifdef  PERIPHERAL
int openPin = 5;
int openButState = 0;
int closePin = 2;
int closeButState = 0;
int mode = 0;
String readBuf = "";
char chr = 'v';

void setup(void) {
    pinMode(openPin, INPUT);
    pinMode(closePin, INPUT);
    Serial.begin(115200);
}

void loop(void) {
    openButState = digitalRead(openPin);
    closeButState = digitalRead(closePin);
    chr = 'v'; 

    while(Serial.available() > 0 ){
        chr = Serial.read();
    }

    if (chr == 'v'){
        /*nothing to come*/
    } else if ( chr == '1' ){
        /*The door is now close state*/
        mode = 1;
    } else if ( chr == '0' ){
        /*The door is now open state*/
        mode = 0;
    }

    if ( openButState == HIGH ) {
        /* Serial.write("hello");  either of them we use. */
        Serial.write("open");
        delay(1000);
    } 
}

#endif
/*------------------------------END--------------------------------*/
