/*
   This code is a sample code for BLE communication.
   But it is also a not perfect one. 
   Just for a referrence.
*/


#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8);

void setup()
{
    char sensorValue[32] ="";
    Serial.begin(9600); 
    mySerial.begin(9600); 
    Serial.println("\r");

    //Wait for a second while the modem sends an "OK"
    delay(1000);                    

    //Because we want to send the SMS in text mode
    Serial.println("AT+");    
    delay(1000);

    mySerial.println("AT+CADC?");     //Query the analog input for data
    Serial.println(Serial.available());    
    Serial.println(Serial.read());    //Print out result???

    //Start accepting the text for the message
    //to be sent to the number specified.
    //Replace this number with the target mobile number.
    Serial.println("AT+CMGS=\"+MSISDN\"\r");    


    delay(1000);
    Serial.println("!");   //The text for the message
    delay(1000);
    Serial.write(26);  //Equivalent to sending Ctrl+Z 
}

void loop()
{
    /*
       if (mySerial.available())
       Serial.write(mySerial.read());
       if (Serial.available())
       mySerial.write(Serial.read());  
     */
}
