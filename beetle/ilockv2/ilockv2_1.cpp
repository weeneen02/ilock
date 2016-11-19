/*
   2016-11-12   :   Fixing getRole function.
   2016-11-13   :   Compile susccess.
   2016-11-13   :   New beetle..
   2016-11-15   :   this code is working well.

issue : couldn't enter AT mode twice. only one is okay.
        I think the problem is about while loop. 
 */

#define CENTRAL
#define MAX 10

#ifdef  PERIPHERAL
#define BUTTON_PIN 3
#endif

int check = 0;

typedef enum {AT_MODE = 0, NORM_MODE = 1} modeState;    // MODE 상태를 나타냄.
modeState at_state = NORM_MODE;     /* 초기값은 NORM_MODE로 시작.*/

/*
   Node는 각각 연결하고자 하는 맥 주소, id, rssi, message등을 가지고 있다.
   Node는 링크 리스트 형식으로 되어있는데 그 이유는 CENTRAL의 경우 여러가지 
   Devices를 각각 연결해야 하기 때문에.
   
 */
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

    /*
       ATmode에 진입한다. 이 함수 안에서 응답까지 받는다.
     */
    int enterAT(void) {
        int reading = 0;

        char chr;
        String buf = "";


        if ( at_state == AT_MODE ) {

        } else if (at_state == NORM_MODE) {
            delay(10);
            /* 둘다 상관 없음 */
            //Serial.print("+++");
            Serial.print("+");
            Serial.print("+");
            Serial.print("+");

            /*
               !!!!!!!!!!!!!!! ISSUE PART !!!!!!!!!!!!!!!!!
               처음 루프에서는 AT 모드 진입 후에 Enter AT Mode 값을 잘 받는데
               두번째부터는 while(1) 문을 빠져나오질 못한다. 그 이유는 알수가 없음..

             */
            while(1){
                while ( Serial.available() > 0 ) {
                    chr = Serial.read();
                    if ( chr == 'E' ) {
                        buf = "";
                        reading = 1;
                    }
                    if ( reading ) {
                        buf += chr;
                    }

                    if ( reading && buf.indexOf("Enter AT Mode\r\n") ) {
                        /* 진입된 응답값을 가지고 있으면 AT_MODE로 상태변경.*/
                        at_state = AT_MODE;
                        reading = 0;
                        return 0;
                    }
                } /* while end */
            }/* while(1) end   ,   for loop*/
        }/*else if end*/
    }

    /*
       AT 에서 나가기 위한 함수. 
       
       역시 while문에 에러가 생길 수있다.
     */
    int exitAT(void) {
        int reading = 0;
        char chr;
        String buf = "";

        if ( at_state == AT_MODE ) {
            Serial.println("AT+EXIT");
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
                        return 0;
                    }
                }
            }

        } else {
            //Serial.println("Error Not AT mode in exitAt");
        }
        if ( at_state == NORM_MODE ) {

        } else {
            //Serial.println("error occured wrong answer from at_exit");
        }
    }

    /*
       RSSI 값을 가지고 온다. 현재 리스트에서 current가 가리키는 Node에서
       RSSI 값 출력을 위해 AT 모드에 AT+RSSI를 던지고. 그 값을 RSSI변수에 저장.
       후에 NORM_MODE에서는 변수값을 출력.
     */
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

    /*
       바인드를 설정해주는 함수.
     */
    void setBind() {
        int reading = 0;
        char chr;
        String buf = "";

        if ( at_state == AT_MODE ) {
            Serial.println("AT+BIND=" + mac);

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
            //Serial.println("Not at mode in set bind func");
            return ;
        }
    }

    /*
       Role 설정해주는 함수.
       CorP에 따라서 Central로 할지, Peripheral로 할지 결정.
     */
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
            //Serial.println("Error not at mode in setRole func");
        }
    }

    void restartAT(void){

        if ( state == AT_MODE ){
            state = NORM_MODE;
            Serial.println("AT+RESTART");
            delay(10);

            /* answer is needed?*/

        } else {
            Serial.println("Error not at mode in restart func");
        }
    }

};

/*
   Node 구조체를 관리하기 위한 클래스.
   head 와 current, tail로 구성이 되어있다.
   head는 말그대로 노드의 처음 시작 부분으 가르키고.
   current는 현재 사용중인 Node를 가리키고 있는다.
   tail은 마지막을 나타냄.
 */
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


        /*
           loop() 문에서 더이상 돌리고 싶지 않을 때...
           st 값이 1이면 더이상 하지 않는다. 그외에는 다른 옵션들임.
         */
        void done(int st) {
            state = st;
        }

        /*
           현재 state에 저장되어있는 값을 불러오기 위한 getter 함수.
         */
        int getState(void) {
            return state;
        }

        /*
           Node를 추가할 때 사용하는 함수.
         */
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

        /*
           현재 current가 가리키는 노드의 주소값을 출력한다.
         */
        Node* getCurrentNode(void) {
            return current;
        }

        /*
           Node의 다음으로 이동할때.
           그 다음 노드를 출력하고, 만약 끝이면(tail)이면, 
           다시 head를 가리키도록 한다.
         */
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

/*
   Software적으로 reset을 하기 위한 함수인데 사용하지 않을 것임.
 */
void(* resetFunc)(void) = 0;

#ifdef CENTRAL
Obj obj;

void setup(void) {
    Serial.begin(115200);
    

    /*
       pin mode open
     */
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    obj.addNode("1", "0xC4BE84DE2795");

    //obj.addNode("2", /*MAC*/);
}

void loop(void) {
    Node* temp = NULL;
    temp = obj.getCurrentNode();

    if (obj.getState() == 1) {

    } else if (obj.getState() == 0) {

        if ( temp->enterAT() == 0 ){
           //pin open    
            digitalWrite(3, LOW);
            digitalWrite(4, HIGH); //green
        }
        temp->setRole(0);  // node에서 이게 필요할까??
        temp->setBind();
        if ( temp->exitAT() == 0 ){
            digitalWrite(4, LOW);
            digitalWrite(3, HIGH);  //blue
        }
        //Serial.println(at_state);
        obj.done(3);
        resetFunc();
    } else if ( obj.getState() == 3 ) {
        if ( temp->enterAT() == 0 ){
            digitalWrite(3, LOW);
            digitalWrite(4, HIGH);
        }
        temp->getRssi();
        if ( temp->exitAT() == 0 ){
            digitalWrite(4, LOW);
            digitalWrite(3, HIGH);
        }
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


