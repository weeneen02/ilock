
/*
 *
 * 주의 사항.
 * AT 모드에서 나오고 나서 Serial.println("\r\n<<<");
 *                         Serial.println("쓸것");
 *                         Serial.println(">>>\r\n");
 *                         Serial.flush(); <--- 반드시 해줘야함.
 *
 * 문자열 출력 할 때는 AT+EXIT 해주고 그다음에 써줘야.
 *
 * 
 *
 *
 * */

typedef enum {ENTER_AT = 0, SET_MAC, SET_ROLE, SET_BIND, SET_SETTING, 
              WAIT_AT, ASK_MAC, ASK_ROLE, ASK_BIND, ASK_SETTING} query;

String answer[10] = {"Enter AT Mode\r\n","OK\r\n", "OK\r\n", "OK\r\n",
                        "OK\r\n", "OK\r\n", "", 

class List {
    private:
    Node* head;

    public:
    /*Add, remove, move, clear*/

};

class Node {

    private:
    String id;
    String mac;
    String rssi;
    Stirng message;
    Node* next;

    public:

    Node(void) {

    }
    
    ~Node(void) {

    }

    void connect(string mac){

    }

    void disconnect(string mac){

    }

    void get_rssi(void){

    }

    void print_message(void){

    }
};


void setup() {

}

void loop() {

}
