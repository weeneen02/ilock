
#define CENTRAL
#define MAX 10

#ifdef  PERIPHERAL
#define BUTTON_PIN  3
#endif


typedef enum {AT_MODE, NORM_MODE} state;    // mode states.
typedef enum {JONE_IN, JONE_OUT} zone;
typedef enum {BUNKNOWN = 0, BEETLE = 1, PHONE = 2, RASPBERRY = 3} devID;

typedef struct obj {
    devID id;
    String mac;
    int rssi;
    String message;
}OBJ;


/* Variant */

/* Only Central */
#ifdef CENTRAL
OBJ peri[MAX] = { {BEETLE, "0xC4BE84DE3D1C", 0, ""},
                  {BEETLE, "0xC4BE84E3A7E1", 0, ""}, 0};
int periCount = 0;
#endif

OBJ me = {BEETLE, "", 0, ""};
String retBuffer;   // return from putATcom function.

/* for interrupt */
const byte ledPin = 5;
const byte interruptPin = 2;
volatile byte state = LOW;

/* Set up */
void setup() {

    /* Serial boud rate. */
    Serial.begin(115200);
    delay(10);

    /* Enter AT mode. */
    entAT();

    /* print infos for Peripheral */
#ifdef PERIPHERAL
    
    /* Interrupt pins for when the button pressed? */
    /* should be fixed.*/
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

    /* Set bind. */
    putATcom("AT+BIND=0xC4BE84DE3D5E");
    Serial.print("bind is? "+retBuffer);

    /* set the role, peripheral. */
    putATcom("AT+SETTING=DEFPERIPHERAL");
    Serial.print("Is that DEFPERIPHERAL? "+retBuffer);

#endif

#ifdef CENTRAL

    /* set the role, central. */
    putATcom("AT+SETTING=DEFCENTRAL");
    Serial.print("Is that DEFCENTRAL? "+retBuffer);

#endif

    /* print AT version */
    putATcom("AT+VERSION=?");   
    Serial.print("version = "+retBuffer);

    /* print current UART */
    putATcom("AT+UART=?");      
    Serial.print("uart = "+retBuffer);

    /* print current CMODE */
    putATcom("AT+CMODE=?"); 
    Serial.print("cmode = "+retBuffer);

    /* print This device's mac addr */
    putATcom("AT+MAC=?");       
    me.mac = retBuffer;
    Serial.print("mac = "+retBuffer);

    /* check this setting */
    putATcom("AT+SETTING=?");
    Serial.print("setting = "+retBuffer);

}


void loop() {

}


/* Beetle functions */
void entAT(void){
    char chr = 0;       // read from serial monitor one byte.
    delay(10);

    Serial.print("+");
    Serial.print("+");
    Serial.print("+");

    while (Serial.available() > 0){
        chr = Serial.read();
        retBuffer += chr;
    }

    if (retBuffer == "Enter AT mode\r\n"){
        Serial.println("We enter AT");
        state = AT_MODE;    // mode is AT mode.
        return;
    }

    else {
        state = NORM_MODE;
        Serial.println("Couldn't Enter AT");
    }
}

void extAT(void){
    delay(10);
    retBuffer = "";     // init retbuffer;
    state = NORM_MODE;
    Serial.println("AT_RESTART");   // restart Bluetooth.
    /* I think after AT_RESTART, AT mode is shut down.*/
}

void putATcom(String a){
    char chr = 0;

    if (state == AT_MODE){
        Serial.println(a);
        delay(10);

        /* get the answer */
        while (Serial.available() > 0){
            if( (chr = Serial.read() != '\n') ){
                retBuffer += chr;
            }
        }
        retBuffer += '\n';
    }

    else {
        Serial.println("Not AT_MODE");
    }
}

/* When interrupt ocurred. */
/* should edit here.*/
void blink(void) {

}






















