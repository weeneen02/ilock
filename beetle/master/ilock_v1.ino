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
*/
typedef enum {START, LISTEN, WORK} c_state;       // Central statement.

// When we read AT commend or Message from devices
typedef enum {START, 
    WAIT_AT, 
    WAIT_RSSI, 
    WAIT_VERSION, 
    WAIT_BIND, 
    WAIT_MESSAGE,
    WAIT_EXIT, 
    NORM_MODE, 
    _TMP} state_type;

typedef enum {

/* Variant */
state_type = START; 
bool reading = false;

String ATcomBuffer;        // from Serial monitor.
Stirng 



/* Beetle functions */
void switch(void);

/*
   When you check this working with Serial moniter. 
   It must be set with "no line ending"

   
 */
void EnterAt(void){         // To Enter AT mode on Sketch.
    delay(10);
    Serial.print("+");
    Serial.print("+");
    Serial.print("+");

    ATcomBuffer = "";

    //state_type = WAIT_AT;   
    // not change type..here.
}


/*
   This function will get the reply from "+++" question.
   w is parameter that shows which query is put in this function.
   RSSI

 */
void WaitAT(w){
    char c;
    ATcomBuffer = "";       // init ATcomBuffer

    while (Serial.available() > 0){
        c = Serial.read();

        if ( c != '\n' ){
            ATcomBuffer += c;
        }
        ATcomBuffer += '\n';
        Serial.print("Here :"+ATcomBuffer);

        return;
    }
}

/*
        if (ATcomBuffer == 'E'){
            reading = true;
            ATcomBuffer = "";
        }

        if (reading){
            ATcomBuffer += c;
        }

        else {  // Except for Enter AT mode. It could be a message from device
                // ISSUE : when pairing with a device, if another device sends message
                //         to Central??

            TmpBuffer += c;
        }

        if (reading && ATcomBuffer == "Enter AT mode\r\n"){
            // get into Enter AT mode

            return;
        }
    
        // just leave this space for the ISSUE
    }

    if (reading == false){
        //error print
        Serial.println(TmpBuffer);
    }
}
*/

/*
   This function is helping to branch each state functions.

   no return.
*/
void CheckState(void){
    if ( state_type == START){
        EnterAt();              
    } else if ( state_type == WAIT_AT ){
        WaitAT();       // wait for the answer about "+++" query or anything.
    } else if ( state_type == WAIT_RSSI ){
        WaitRSSI();
    } else if ( state_type == WAIT_VERSION ){
        WaitVersion();
    } else if ( state_type == WAIT_BIND ){
        WaitBind();
    } else if ( state_type == WAIT_MESSAGE ){
        WaitMessage();  // when message comes it works
    } else if ( state_type == WAIT_EXIT ){
        WaitExit();
    } else if ( state_type == NORM_MODE ){
        NormMode();
    }
}

void setup() {
    Serial.begin(115200);

    delay(10);

    // init Central Beetle.
    // 0. enter AT mode  
    state_type = START;
    CheckState();
    // put the next value of what we are gonna query AT command onto state_type.

    // 1. MAC 
    // 2. 
    

}

void loop() {

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
