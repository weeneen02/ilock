/*
   2016-11-12   :   Fixing getRole function.
   2016-11-13   :   Compile susccess.
   2016-11-13   :   New beetle.. 
   Let's make it. 
 */



#define CENTRAL
#define MAX 10
#ifdef  PERIPHERAL
#define BUTTON_PIN 3
#endif


typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;


class Node {
    friend class Obj;

    private:
    String id;
    String mac;
    String rssi;
    String message;
    modeState state;
    Node* next;

    public:
    Node(void) {
        state = NORM_MODE;
        id = "";
        mac = "";
        rssi = "";
        message = "";
        state = NORM_MODE;
        next = NULL;
    }

    Node(String id, String mac) {
        state = NORM_MODE;
        this->id = id;
        this->mac = mac;
        this->rssi = "";
        this->message = "";
        this->state = NORM_MODE;
        this->next = NULL;
    }

    ~Node(void) {

    }

    void enterAT(void) {
        int reading = 0;
        char chr;
        String buf = "";

        delay(10);

        Serial.print("+");
        Serial.print("+");
        Serial.print("+");
        state = AT_MODE;

        delay(20);
        if ( state == AT_MODE )
            while ( Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == 'E' ){
                    buf = "";
                    reading = 1;
                }
                if ( reading ){
                    buf += chr;
                }

                if ( reading && buf == "Enter AT Mode\r\n" ) {
                    state = AT_MODE;
                    reading = 0;
                }
            }
        if (state == AT_MODE){

        } else {
            Serial.println("couldn't enter at mode");
        }
    }

    void exitAT(void) {
        int reading = 0;
        char chr;
        String buf = "";

        if ( state == AT_MODE ) {
            Serial.println("AT+EXIT");
            delay(10);

            while ( Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == 'O'){
                    reading = 1;
                    buf = "";
                }

                if ( reading ) {
                    buf += chr;
                }


                if ( buf == "OK\r\n" ) {
                    state = NORM_MODE;
                    reading = 0;

                }
            }
        } else {
            Serial.println("Error Not AT mode in exitAt");
        }
        if ( state == NORM_MODE ){

        } else {
            Serial.println("error occured wrong answer from at_exit");
        }
    }

    String getRssi(void) {
        char chr;
        int reading = 0;
        String buf = "";

        if ( state == AT_MODE ) {
            Serial.println("AT+RSSI=?");
            delay(10);

            while ( Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == '-' ){
                    reading = 1;
                    buf = "";
                }
                if ( reading ){
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

        if ( state == AT_MODE ) {
            Serial.println("AT+BIND=" + mac);
            delay(10);

            while (Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == 'O' ){
                    buf = "";
                    reading = 1;
                }
                if ( reading ){
                    buf += chr;
                }

                if ( reading && buf == "OK\r\n" ) {
                    reading = 0;
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

        if ( state == AT_MODE ) {
            if ( CorP == 0 ) {
                Serial.println("AT+ROLE=ROLE_CENTRAL");
            } else {
                Serial.println("AT+ROLE=ROLE_PERIPHARAL");
            }
            delay(10);

            while ( Serial.available() > 0 ) {
                chr = Serial.read();
                if ( chr == 'O' ){
                    buf = "";
                    reading = 1;
                }
                if ( reading ){
                    buf += chr;
                }
                if ( buf == "OK\r\n") {
                    reading = 0;
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


        void done(void) {
            this->state = 1;
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
            if ( current == tail ) {
                current = head;
                return current;
            } else {
                return (current = current->next);
            }
        }
};

Obj obj;
void setup(void) {
    Serial.begin(115200);
    obj.addNode("1", "0xC4BE84DE2795");

    //obj.addNode("2", /*MAC*/);

}

void loop(void) {
    Node* temp = NULL;

    if (obj.getState() == 1) {

    } else {
        temp = obj.getCurrentNode();

        temp->enterAT();
        temp->setRole(0);  // node에서 이게 필요할까??
        temp->setBind();
        temp->exitAT();

        temp->enterAT();
        temp->getRssi();
        temp->exitAT();

        Serial.println(temp->getRssi());
        obj.done();
        obj.getNextDev();
    }
}


