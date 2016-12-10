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

int chooseMenu(void){
    int select;
    printf("what do you want? please typing your command\n");
    printf("1.server launch continue.\n");
    printf("2.add new phoneNumber..\n");
    printf("3.add new BlunoDevice..\n");
    printf("4.terminate program...\n");
    printf(" input here : ");
    scanf("%d", &select);
    return select;
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
    char    readbuff[18] = {0,};

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

    setup();
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    /*Error*/
    if ( server_socket == -1 ){
        printf("server socket error");
        exit(1);
    } else {
        printf("socket created : %d\n", socket);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(9100);  /* to communicate with on network. */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    nSockOpt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));

    if ( bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) ){
        printf("bind error...\n");
        exit(1);
    } else {
        printf("socket binded\n");
    }

    switch(chooseMenu()){
        case 1:
            /*server launch*/
            if ( listen(server_socket, 5) == -1 ){
                exit(1);
            } else {
                printf("listening...\n");
            }

            client_addr_size = sizeof(client_addr);
            client_socket = accept(server_socket, (struct sockaddr*)%client_addr,
                    &client_addr_size);

            if ( client_socket == -1 ){
                exit(1);
            } else {
                printf("client socket created...\n");
            }

            /* read buffer */
            read(client_socket, buff_rcv, BUFF_SIZE);
            
            i = 0;
            target = strtok(buff_rcv, " ");   
            while(target != NULL){
                sArr[i] = target;
                i++;
                target = strtok(NULL, " ");
            } 

            /*files open*/
            fp = fopen("PhoneList.txt", "r");
            fp2 = fopen("BlunoList.txt", "r");
            
            memset(readbuff, 0, sizeof(readbuff));
            /* phone number in readbuff */
            while(fgets(readbuff, 12, fp)){
                /*sArr[0] is the phone number from buff_rcv. */
                if (!strcmp(sArr[0], readbuff)){
                    memset(readbuff, 0, sizeof(readbuff));
                    while(fgets(readbuff, 18, fp2)){
                        /* sArr[1] is MAC address from buff_rcv*/
                        if (!strcmp(sArr[1], readbuff)){
                            /*fd is file descriptor of device"/dev/ttyACM0"*/
                            serialPuts(fd, sArr[1]);
                            serialPuts(fd, "\n");
                        } else {
                            printf("you have a right phone number in my list
                                    but we don't have any bluno in our list...\n");
                        }
                        /*close BlunoList.txt.*/
                        fclose(fp2);
                        break;
                    }
                    break;
                } else {
                    printf("no... this is not the same phone number...\n");
                }
            }
            fclose(fp);
            close(client_socket);
            
            memset(buff_rcv[0], 0, sizeof(buff_rcv[0]));
            break; /*case 1's break*/
        case 2:
            /*add new phoneNumber*/
            printf("you selected phone number add\n");
            printf("please type your phone number :");
            scanf("%s", PhoneNumber);

            fp = fopen("PhoneList.txt", "a");
            fprintf(fp, PhoneNumber);
            fprintf(fp, "\n");
            printf("Write success...\n");
            fclose(fp);
            break;
        case 3:
            /*add new bluno*/
            printf("you selected bluno add..\n");
            printf("please type your new bluno device.. usually 17words..\n");
            printf("format is XX:XX:XX:XX:XX:XX \n");
            scanf("%s",BlunoNumber);
            printf("wiriting..\n");
            fp2=fopen("BlunoList.txt","a");
            fprintf(fp2,BlunoNumber);
            fprintf(fp2,"\n");
            printf("write success..\n");
            fclose(fp2);

            break;
        case 4:
            /*terminate program*/
            printf("you selected program end....\n");
            exit(1);
            break;
        default :
            printf("only you can input 1~4 numbers\n");
            break;
    }

}

#endif /* ifdef RaspberryPi */

