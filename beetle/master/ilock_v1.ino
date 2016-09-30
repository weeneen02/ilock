


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
