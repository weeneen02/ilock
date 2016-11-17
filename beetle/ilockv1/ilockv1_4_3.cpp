/*
   2016-11-12   :   Fixing getRole function.
   2016-11-13   :   Compile susccess.
   2016-11-13   :   New beetle..
   2016-11-15   :   this code is working well.

issue : Why coulndn't enter AT mode twice..
 */



#define CENTRAL
#define MAX 10

#ifdef  PERIPHERAL
#define BUTTON_PIN 3
#endif

int check = 0;

typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;
modeState at_state = NORM_MODE;

class Node {
    friend class Obj;

    private:
    String id;
    String mac;
    String rssi;
    String message;
    Node* next;

    public:
    Node(void) {
        id = "";
        mac = "";
        rssi = "";
        message = "";
        next = NULL;
    }

    Node(String id, String mac) {
        this->id = id;
        this->mac = mac;
        this->rssi = "";
        this->message = "";
        this->next = NULL;
    }

    ~Node(void) {

    }

    void enterAT(void) {
        int reading = 0;

        char chr;
        String buf = "";

        delay(10);
        if ( at_state == AT_MODE ) {

        } else if (at_state == NORM_MODE) {

            /* 둘다 상관 없음 */
            Serial.print("+++");
            //Serial.print("+");
            //Serial.print("+");
            //Serial.print("+");


            while (1) {
                if ( buf.length() > 16 ) {
                    check++;
               
                }

                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    if ( chr == 'E' ) {
                        buf = "";
                        reading = 1;
                    }
                    if ( reading ) {
                        buf += chr;
                    }

                    if ( reading && buf == "Enter AT Mode\r\n" ) {
                        at_state = AT_MODE;
                        reading = 0;
                        return;
                    }

                    if (at_state == AT_MODE) {

                    } else {
                        //Serial.println("couldn't enter at mode");
                    }
                } /* while end */
            }/* while(1) end*/
        }/*else if end*/
    }

    void exitAT(void) {
        int reading = 0;
        char chr;
        String buf = "";

        if ( at_state == AT_MODE ) {
            Serial.println("AT+EXIT");
            delay(10);

            while (1) {
                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    if ( chr == 'O') {
                        reading = 1;
                        buf = "";
                    }

                    if ( reading ) {
                        buf += chr;
                    }


                    if ( buf == "OK\r\n" ) {
                        at_state = NORM_MODE;
                        reading = 0;
                        return;
                    }
                }
            }

        } else {
            Serial.println("Error Not AT mode in exitAt");
        }
        if ( at_state == NORM_MODE ) {

        } else {
            Serial.println("error occured wrong answer from at_exit");
        }
    }

    String getRssi(void) {
        char chr;
        int reading = 0;
        String buf = "";

        if ( at_state == AT_MODE ) {
            Serial.println("AT+RSSI=?");
            delay(10);

            while ( Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == '-' ) {
                    reading = 1;
                    buf = "";
                }
                if ( reading ) {
                    buf += chr;
                }
                if ( reading && buf.length() == 6 ) {
                    rssi = buf;
                    reading = 0;
                }

            }
        } else {
            return rssi;
        }
    }

    void setBind() {
        int reading = 0;
        char chr;
        String buf = "";

        if ( at_state == AT_MODE ) {
            Serial.println("AT+BIND=" + mac);
            delay(10);

            while (1) {
                while (Serial.available() > 0 ) {
                    chr = Serial.read();
                    if ( chr == 'O' ) {
                        buf = "";
                        reading = 1;
                    }
                    if ( reading ) {
                        buf += chr;
                    }

                    if ( reading && buf == "OK\r\n" ) {
                        reading = 0;
                        return;
                    }
                }
            }
        } else {
            Serial.println("Not at mode in set bind func");
            return ;
        }
    }

    void setRole(int CorP) {
        /*
         */
        int reading = 0;
        char chr;
        String buf = "";

        if ( at_state == AT_MODE ) {
            if ( CorP == 0 ) {
                Serial.println("AT+ROLE=ROLE_CENTRAL");
            } else {
                Serial.println("AT+ROLE=ROLE_PERIPHERAL");
            }
            delay(10);
            while (1) {
                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    if ( chr == 'O' ) {
                        buf = "";
                        reading = 1;
                    }
                    if ( reading ) {
                        buf += chr;
                    }
                    if ( buf == "OK\r\n") {
                        reading = 0;
                        return ;
                    }
                }
            }
        } else {
            Serial.println("Error not at mode in setRole func");
        }
    }
};

class Obj {
    private:
        Node* head;
        Node* tail;
        Node* current;
        int state;  /* state == 1이면, 끝 state == 0이면 정상*/

    public :
        Obj(void) {
            head = NULL;
            tail = NULL;
            current = NULL;
            state = 0;
        }

        ~Obj(void) {

        }


        void done(int st) {
            state = st;
        }

        int getState(void) {
            return state;
        }

        void addNode(String id, String mac) {
            Node* add = new Node(id, mac);
            Node* temp = NULL;
            Node* ex = NULL;
            int i;

            if ( head == NULL ) {
                head = add;
                current = head;
                tail = head;
            } else {
                for ( temp = head; temp != NULL; temp = temp->next ) {
                    ex = temp;
                }
                ex->next = add;
                tail = add;
            }
        }

        Node* getCurrentNode(void) {
            return current;
        }

        Node* getNextDev(void) {
            if ( current == NULL ) {
                Serial.println("Error No Node");
            } else if ( current->next == NULL ) {
                current = head;
                return current;
            } else {
                return (current = current->next);
            }
        }
};


#ifdef CENTRAL
Obj obj;
void setup(void) {
    Serial.begin(115200);
    obj.addNode("1", "0xC4BE84DE2795");

    //obj.addNode("2", /*MAC*/);
}

void loop(void) {
    Node* temp = NULL;
    temp = obj.getCurrentNode();

    if (obj.getState() == 1) {

    } else if (obj.getState() == 0) {

        temp->enterAT();
        temp->setRole(0);  // node에서 이게 필요할까??
        temp->setBind();
        temp->exitAT();
        Serial.println(at_state);
        obj.done(3);

    } else if ( obj.getState() == 3 ) {
        temp->enterAT();
        temp->getRssi();
        temp->exitAT();

        delay(10);
        /*if rssi is over 0 (e.g connected),

          listen...
          check... message it got.

         */

        Serial.println("\r\n<<<");
        Serial.println(temp->getRssi());
        Serial.println(">>>\r\n");
        Serial.flush();
        Serial.println(at_state);

        obj.done(3);      // 왜 계속 실행이 되지 않는 거지...?
        obj.getNextDev();
    }
}
#endif



#ifdef PERIPHERAL
Obj obj;
void setup(void) {
    Serial.begin(115200);
    obj.addNode("1", "0xC4BE84E3A786");  /*Central MAC*/

}

void loop(void) {
    Node* temp = NULL;

    /* when button clicked. start? */
    if (obj.getState() == 1) {

    } else {
        temp = obj.getCurrentNode();

        temp->enterAT();
        temp->setRole(1);
        temp->setBind();
        temp->exitAT();

        /*
           send data.

         */

        obj.done(1);
        obj.getNextDev();
    }
}
#endif


