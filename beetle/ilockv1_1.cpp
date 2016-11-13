/*
   2016-11-12   :   Fixing getRole function.
   2016-11-13   :   Compile success!
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
    Node(void){
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

    void enterAT(void){
        char chr;
        String buf = "";

        delay(10);

        Serial.print("+");
        Serial.print("+");
        Serial.print("+");

        delay(10);

        while ( Serial.available() > 0 ){
            chr = Serial.read();
            buf += chr;

            if ( buf == "Enter AT mode\r\n" ){
                state = AT_MODE;
                break;
            } 
        } 
    }

    void exitAT(void){
        char chr; 
        String buf = "";

        if ( state == AT_MODE ){
            Serial.print("AT+EXIT");
            delay(10);

            while( Serial.available() > 0 ){
                chr = Serial.read();
                buf += chr;

                if ( buf == "OK\r\n" ){
                    state = NORM_MODE;
                    break;
                }
            }
        } else {
            Serial.println("Error Not AT mode");
        }
    }

    String getRssi(void){
        char chr;
        String buf = "";

        if ( state == AT_MODE ){
            Serial.print("AT+RSSI=?");
            delay(10);

            while( Serial.available() > 0 ){
                chr = Serial.read();
                buf += chr;

                if ( buf.length() == 6 ){
                    rssi = buf;
                    return rssi; 
                }

            }
        } else {
            Serial.println("Error Not AT mode");
        }
        return "";
    }

    void getRole(void){
        /*
         */

    }


};

class Obj {
    private:
        Node* head;
        Node* tail;
        Node* current;
        int state;  /* state == 1이면, 끝 state == 0이면 정상*/

    public :
        Obj(void){
            head = new Node;
            tail = head;
            current = head;
            state = 0;
        }

        ~Obj(void){

        }


        void done(void){
            this->state = 1;
        }

        int getState(void){
            return state;
        }

        void addNode(String id, String mac){
            Node* add = new Node(id, mac); 
            Node* temp = NULL;
            int i;
            for ( temp = head; temp->next != NULL; temp = temp->next );
            temp->next = add;
            tail = add;
        }

        Node* getCurrentNode(void){
            return current;
        }

        Node* getNextDev(void){
            if ( current == tail ){
                current = head;
                return current;
            } else {
                return (current = current->next);
            }
        }
};



Obj obj;


void setup(void){

    Serial.begin(115200);
    //obj.addNode("1", /*MAC*/);
    //obj.addNode("2", /*MAC*/);

}

void loop(void){
    Node* temp = NULL;

    if (obj.getState() == 0){

    } else {
        temp = obj.getCurrentNode(); 
        temp->enterAT();
        temp->getRssi();
        temp->exitAT();
        Serial.println(temp->getRssi());
        obj.done();
        obj.getNextDev();
    }
}


