/*
   This program is for Central Bluno beetle bluetooth.

Name        : ilock

author      : Lim Jae Hwan

purpose     : 1. Connection with peripheral devices
              2. Switch devices in every 2 seconds

first date  : 2016_09_30

functions   : 1. Switch             : Switch devices
                A. Connection       : Connect devices
                B. Disconnection    : Disconnect devices
              2. Setting.           : set the configuration of Bluno
              3. Checking.          : Check the RSSI values.

modified    : 2016-10-02            : branch feature 001 : beetle to beetle.
                                    : We are gonna make Central and Peripheral
                                    : with one code by using #define CENT, PERI
                                    : So, I want you to compile with this, 
                                    : Plz make sure that you set the ROLE in 
                                      this sketch.

            : 2016-10-05            : We decided to make a function to handle 
                                      AT commands 
                                    : I made a function which sends AT commands 
                                      and receives
                                    : I am editting the loop part. 
                                        The ISSUES are 1. how can I set the time
                                                          limit.
                                                       2. how can I handle the 
                                                          messages from devs

            : 2016-10-12            : It was so late.
                                    : We need to check that beetle's connected
                                      or not. I thought AT+RSSI could check this
                                      but it was wrong. Mento said that "RSSI 
                                      is not a function to check, It's just let
                                      you know how long devices are. So, I need
                                      one routine to deal with this problem. 
                                      How about sending data to the arduino I 
                                      think it's already connected. Maybe the 
                                      device will return data if connected.
function brief  : 
1. putATcom     : the parameter is String that has a query. and this function 
                  doesn't use return. global variable "retBuffer" has the 
                  return value which is string.

2. entAT        : Get into AT mode. 
3. extAT        : Exit AT mode.
                                    


when restarted;
*/


// CENTRAL OR PERIPHERAL. Choose!
#define CENTRAL

#ifdef  PERIPHERAL
#define BUTTON_PIN  3
#endif




typedef enum {AT_MODE, NORM_MODE} state;    // mode states.
typedef enum {JONE_IN, JONE_OUT} zone;
typedef enum {BUNKNOWN = 0, BEETLE = 1, PHONE = 2, RASPBERRY = 3} devID;


// This is peripheral info
typedef struct obj {
    devID id;                  
    String mac;
    int rssi;       // convert string to int?
    //int time;
    String message;
}OBJ;




/* Variant */
String retBuffer;       // from Serial monitor.
OBJ peri[10] = {{BEETLE,"0xC4BE84DE3D1C", 0, ""},
                {BEETLE,"0xC4BE84E3A7E1", 0, ""},0};        // Peripheral's info
OBJ me = {BEETLE,"",0,""};
int periCount = 0;

// for interrupt 
const byte ledPin = 5;
const byte interruptPin = 2;
volatile byte instate = LOW;


/* Beetle functions */
void entAT(void){
    char chr = 0;
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
        state = AT_MODE;
        return;
    }
    
    else {
        state = NORM_MODE;
        Serial.println("Couldn't Enter AT");
    }
}

void extAT(void){
    delay(10);
    retBuffer = "";
    state = NORM_MODE;
    Serial.println("AT_RESTART");  // Restart bluetooth.
    //Serial.println("AT_EXIT");
}

void putATcom(String a)
{
    char chr = 0;

    if (state == AT_MODE){
        Serial.println(a);
        delay(10);

        // get the answer
        while (Serial.available() > 0){
            if ( (chr = Serial.read() != '\n') ) ;
            retBuffer += chr;
        }
        retBuffer += '\n';
    }

    else {
        Serial.println("Not AT_MODE");
    }
}


/* When interrupt ocurrs*/
void blink() {

    // here! when button clicked what's gonna do?

    /*
       1. restart.
       2. check rssi.
            zone setting.
    */

    
}




void setup() {

    Serial.begin(115200);
    delay(10);

    /* Enter AT mode */
    entATcom();        

    /* print infos */
#ifdef PERIPHERAL 
    
    /* Interrupt pins */
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

    /* Set bind.*/
    putATcom("AT+BIND=0x.....");
    Serial.print("bind is? "+retBuffer);

    /* set the role, peripheral */
    putATcom("AT+SETTING=DEFPERIPHERAL");
    Serial.print("Is that DEFPERIPHERAL? "+retBuffer);
#endif

#ifdef CENTRAL
    /* set the role, central */
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
    //while looping 
#ifdef CENTRAL
    /* Central's rules */
    /*
       At Central, Checking dev every given time.
       time interrupt. if time is out. stopping send or receiving 
                        message.

       1. binding
       2. restart.  ISSUE : what if it restarts this dev... not bluetooth.
       3. check RSSI.
       4. listening.  get the message from current bound dev.
                if time out 
                    next peri
                else
                    keep listening.
     */



#endif

#ifdef PERIPHERAL
   

    /* 
       When the button pressed, send the message to Central.
       Which pin is good for button?

    0. state == open?

    1. restart

    2. check RSSI
        if not 0
            send message
    3. return.

    when button clicked, restart, and rssi check. ok?
        
    the structure of the message.
    
    1. MAC


    =-----------=
    to sync with central. we need to keep this link long time..

    */

    // state check
    if ( zone == ZONE_IN ){

        // light green

        putATcom("AT+RSSI=?");
        if (retBuffer != "-000\r\n"){
            Serial.println("0000" + me.mac + me.id + "open" + "0000");
        }


    }

    else if ( zone == ZONE_OUT ){
        // light red

    }

#endif

}




