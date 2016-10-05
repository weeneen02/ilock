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
                                    : Plz make sure that you set the ROLE in this sketch.

            : 2016-10-05            : We decided to make a function to handle AT commands 
                                    : I made a function which sends AT commands and receives
                                    : I am editting the loop part. 
                                        The ISSUES are 1. how can I set the time limit.
                                                       2. how can I handle the messages from devs

                                    
*/


// CENTRAL OR PERIPHERAL. Choose!
#define CENTRAL

typedef enum {AT_MODE, NORM_MODE} state;    // mode states.
typedef enum {BUNKNOWN = 0, BEETLE, PHONE, RASPBERRY} devID;
// This is peripheral info
typdef struct obj {
    devID id;                  
    String mac;
    int rssi;
    //int time;
    String message;
}OBJ;



/* Variant */
String retBuffer;       // from Serial monitor.
OBJ peri[10] = {{BEETLE,"0xC4BE84DE3D1C",0,""},{BEETLE,"0xC4BE84E3A7E1", 0, ""},0};               // Peripheral's info
int periCount = 0;




void setup() {

    Serial.begin(115200);
    delay(10);

    /* Enter AT mode */
    entATcom();        

    /* print infos */
#ifdef PERIPHERAL 
    /* set the role, peripheral */
    putATcom("AT+SETTING=DEFPERIPHERAL");
    Serial.print("Is that DEFPERIPHERAL? "+retBuffer);
#endef

#ifdef CENTRAL
    /* set the role, central */
    putATcom("AT+SETTING=DEFCENTRAL");
    Serial.print("Is that DEFCENTRAL? "+retBuffer);
#endef

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



#endef

#ifdef PERIPHERAL

    /* 
       When the button pressed, send the message to Central.
       Which pin is good for button?


    1. check RSSI
        if not 0
            send message
    2. return.

    */
#endef

}




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


-----------------------------------------Referrence----------------------------

const int ledBLE = 13;
typedef enum {START, WAIT_AT, WAIT_RSSI, WAIT_EXIT, NORM_MODE, _TMP} state_type;
state_type state = START;
char chr;
String buffer, rssi, data;
bool reading = false;

void setup() {
    Serial.begin(115200);  //Initiate the Serial comm
    pinMode(ledBLE, OUTPUT);
    state = START;
}

void loop() {
    if (state == START) {
        start();
    } else if (state == WAIT_AT) {
        wait_at();
    } else if (state == WAIT_RSSI) {
        wait_rssi();
    } else if (state == WAIT_EXIT) {
        wait_exit();
    } else if (state == NORM_MODE) {
        norm_mode();
    }
}

void start() {
    delay(10);
    Serial.print("+");
    Serial.print("+");
    Serial.print("+");
    data = "";
    state = WAIT_AT;
}

void wait_at() {
    while (Serial.available() > 0) {
        chr = Serial.read();
        if (chr == 'E') {
            reading = true;
            buffer = "";
        }
        if (reading) {
            buffer += chr;
        } else {
            data += chr;
        }
        if (reading && buffer == "Enter AT Mode\r\n") {
            Serial.println("AT+RSSI=?");
            reading = false;
            state = WAIT_RSSI;
        } else if (data == "led_on") {
            digitalWrite(ledBLE, HIGH);
            data = "";
        } else if (data == "led_off") {
            digitalWrite(ledBLE, LOW);
            data = "";
        }
    }
}

void wait_rssi() {
    while (Serial.available() > 0) {
        chr = Serial.read();
        if (chr == '-') {
            reading = true;
            buffer = "";
        }
        if (reading) {
            buffer += chr;
        }
        if (buffer.length() == 4) {
            rssi = buffer;
            Serial.println("AT+EXIT");
            reading = false;
            state = WAIT_EXIT;
            if (rssi == "-000") {
                digitalWrite(ledBLE, LOW);
            }
        }
    }
}

void wait_exit() {
    while (Serial.available() > 0) {
        chr = Serial.read();
        if (chr == 'O') {
            reading = true;
            buffer = "";
        }
        if (reading) {
            buffer += chr;
        }
        if (buffer == "OK\r\n") {
            reading = false;
            state = NORM_MODE;
        }
    }
}

void norm_mode() {
    Serial.println("\r\n<<<");
    Serial.println(rssi);
    Serial.println(">>>\r\n");
    Serial.flush();
    state = START;
}
