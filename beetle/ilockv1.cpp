/*
   2016-11-12   :   Fixing getRole function.
 */



#define CENTRAL
#define MAX 10
#ifdef  PERIPHERAL
#define BUTTON_PIN 3
#endif


typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;


class Obj {
    private:
        Node* head;
        Node* tail;

    public :
        Obj(void){
            head = NULL;
            tail = NULL;
        }

        ~Obj(void){

        }

};


class Node {
    
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

        void getRssi(void){
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
                        return; 
                    }

                }
            } else {
                Serial.println("Error Not AT mode");
            }
            return ;
        }
        
        void getRole(void){
            /*
               
             */

        }


};


