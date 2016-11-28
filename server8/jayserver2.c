#define _CRT_SECURE_NO_WARINIGS
#ifdef RaspberryPi

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define  BUFF_SIZE   1024

// Find Serial device on Raspberry with ~ls /dev/tty*
// ARDUINO_UNO "/dev/ttyACM0"
// FTDI_PROGRAMMER "/dev/ttyUSB0"
// HARDWARE_UART "/dev/ttyAMA0"
char device[]= "/dev/ttyACM0";
// filedescriptor
int fd;
unsigned long baud = 9600;
unsigned long time=0;

void loop(void);
void setup(void);

void setup() {
    printf("%s \n", "Raspberry Startup!");
    fflush(stdout);

    //get filedescriptor
    if ((fd = serialOpen(device, baud)) < 0){
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        exit(1); //error
    }

    //setup GPIO in wiringPi mode
    if (wiringPiSetup() == -1){
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        exit(1); //error
    }
}

int main(void){
    int     i, j;
    int     server_socket;
    int     client_socket;
    int     client_addr_size;
    int     nSockOpt;
    struct  sockaddr_in server_addr;
    struct  sockaddr_in client_addr;
    char    buff_rcv[BUFF_SIZE];
    char    buff_snd[BUFF_SIZE];
    char*   readbuff;

    /* sArr[0] is Phone number, sArr[1] is Bluno MAC. */
    char*   sArr[2] = {NULL,};
    char*   target;
    char    Message_hello[50] = "OK Server accepted and process your request. wait";

    /* After parsing received buffer, put it in this. */
    char    PhoneNumber[12]; 
    /* bluno number.*/
    char    BlunoNumber[18]; 

    /* token for seperating messages. */
    char    Separate[] = " ";
    int     menu = 0; 
    
    FILE    *fp;
    FILE    *fp2;
    


}




















