#define _CRT_SECURE_NO_WARNINGS  
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

void setup(){

		printf("%s \n", "Raspberry Startup!");
		fflush(stdout);

		//get filedescriptor
		if ((fd = serialOpen (device, baud)) < 0){
				fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
				exit(1); //error
		}

		//setup GPIO in wiringPi mode
		if (wiringPiSetup () == -1){
				fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
				exit(1); //error
		}

}



int   main( void)
{
		int i,j;
		int   server_socket;
		int   client_socket;
		int   client_addr_size;
		int  nSockOpt; //my option
		struct sockaddr_in   server_addr;
		struct sockaddr_in   client_addr;
		char   buff_rcv[BUFF_SIZE];
		char   buff_snd[BUFF_SIZE];
		char* readbuff;

		char* sArr[2]={NULL,};//여기에다가 폰번호와 블루노 주소 각각 읽어서 저장할거임
		char* target;
		char   Message_hello[50] ="OK Server accepted and process your request. wait";

		char PhoneNumber[12];//폰으로부터 받은 소켓메세지에서 번호부분을 분리해서여기 넣을거임
		char BlunoNumber[18];//마찬가지로 소켓메세지에서 스페이스를 기점으로 분뢰된 블루노 주소부분을 여기다가 넣는다 함수는 토큰이나 기타 문자열 함수 사용할거임

		char Separate[]=" ";//분리할 토큰 폰으로 부터 올 메세지는 "폰번호""공백""블루노주소" 이런식으로 올것이기 때문이다. 

		int menu;//메뉴 선택용
		FILE *fp ; //파일 포인터 폰 주소 목록 파일 볼때
		FILE *fp2;//블루투스디바이스 주소 목록 파일볼때



		setup();//와이어링파이 관련 세팅 함수 시작;
		server_socket  = socket( PF_INET, SOCK_STREAM, 0);

		if( -1 == server_socket)
		{ 
				printf( "server socket ?앹꽦 ?ㅽ뙣n");
				exit( 1);
		} else {
				printf("Socket created : %d\n",socket);
		}

		memset( &server_addr, 0, sizeof( server_addr));

		server_addr.sin_family     = AF_INET;
		server_addr.sin_port       = htons(9100);
		server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
		nSockOpt=1;//my sujung

		setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));//바인드 부분임 재 소켓 연결을 하더라도 허용할 수 있도록 해준다. 

		if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
		{
				printf("bind error....\n");
				exit( 1);
		} else { 
				printf("socket binded\n/");
		}

		// 여기서부터 기초 세팅이 시작되었고 이제 메뉴를 선택하자... 1 서버 돌리기, 2 폰넘버를 새로 추가하기 3 블루노 추가하기 4 프로그램 끄기.. 봐가면서 삭제도
		printf("what do you want? please type your command\n");
		printf("1.server launch continue.\n");
		printf("2.add new phoneNumber..\n");
		printf("3.add new BlunoDevice..\n");
		printf("4.terminate program...\n");

		printf(" input here : ");
		scanf("%d",&menu);

		if(menu==1)
		{
				//여기서부터는 설정된뒤에는 계속해서 돌아가야 하는 부분임 잘 보고 만들어보도록 하자. 
				while( 1)
				{
						if( -1 == listen(server_socket, 5))
						{
								exit( 1);
						} else {
								printf("listening.....\n");
						}

						client_addr_size  = sizeof( client_addr);
						client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
						if ( -1 == client_socket)
						{
								exit( 1);
						} else {
								printf("client socket created...\n");
						}
						buff_rcv[0]='\0'; //받기전에 한번 초기화 해주자
						read ( client_socket, buff_rcv, BUFF_SIZE);//이부분에서 굳이 buff size만큼 읽을 필요 없었음...
						printf( "receive: %s\n", buff_rcv);//buff_rcv에 클라이언트가 보낸 메세지를 저장하게 된다. 
						printf("size of received message is %d...\n",strlen(buff_rcv));
						printf("after string received..\n");       

						i = 0;//토큰작업
						target = strtok(buff_rcv," ");//자세한건 씨언어의 토큰메서드 참조
						while(target !=NULL)
						{
								sArr[i]=target;
								i++;
								target=strtok(NULL," ");
						}
						printf("phonnumber is %s...size is %d ...\n",sArr[0],strlen(sArr[0]));
						printf("blunonumber is %s...size is %d...\n",sArr[1],strlen(sArr[1]));//블루노와 폰넘버를 분리했다





						fp=fopen("PhoneList.txt","r");//폰리스트 텍스트파일을 읽어오자..
						readbuff=(char*)malloc(12*sizeof(char));
						while(fgets(readbuff,12,fp))
						{ printf("this str from PhoneList.txt is %s...\n",readbuff);
								printf("size of Phonelists str size is %d...\n",strlen(readbuff));
								if(!strcmp(sArr[0],readbuff))
								{
										//printf("i found a same phonenumber in my list.. so admit\n");
										free(readbuff);
										//이제는 블루투스 디바이스의 주소목록을담은 파일을 읽어들여서 조사해보자!!
										fp2 = fopen("BlunoList.txt","r");
										readbuff=(char*)malloc(18*sizeof(char));
										while(fgets(readbuff,18,fp2))
										{
												if(!strcmp(sArr[1],readbuff))
												{
														printf("this str from BlunoList.txt is %s...\n",readbuff);
														printf("size of this str is %d...\n",strlen(readbuff));
														printf("i found a same blunonumber and phonenumber in my list.. so admit\n");
														
														serialPuts (fd, sArr[1]);
														serialPuts(fd,"\n");
														
														fclose(fp2);
														break;
												}
												else
												{
														printf("you have a right phonenumber in my list but we dont have any bluno in our list..\n");
														fclose(fp2);
														break;
												}
										}
										break;

								}
								else
								{
										printf("no... this is not same phonenumber ....\n");


								}
						}
						fclose(fp);
						free(readbuff);


						// sprintf( buff_snd, "%d : %s", strlen( buff_rcv), buff_rcv);
						write(client_socket,Message_hello,strlen(Message_hello));
						// write( client_socket, buff_snd, strlen( buff_snd)+1); 
						close( client_socket);//한번의 작업이 끝나면 소켓을 끊도록 만들었다...
						buff_rcv[0]='\0';//받고 나서도 초기하 해주자.


				}
		}
		else if(menu ==2)
		{
			printf("you selected phone numer add\n");
				//여기서부터는 전하번호가 적혀 있는 파일 열어서 입력한 전화번호를 추가하도록 하자
			printf("please type your phone number.. usually 11words... : ");
			scanf("%s",PhoneNumber);
			printf("writing....\n");
			fp=fopen("PhoneList.txt","a");
			fprintf(fp,PhoneNumber);
			fprintf(fp,"\n");
			printf("write successs...\n");
			fclose(fp);


			
		}
		else if(menu ==3)
		{
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

		}

		else if (menu == 4)
		{
				printf("you selected program end....\n"); 
				exit(1);
		}//프로그램 종료


}

#endif //#ifdef RaspberryPi 조건부 컴파일 종료
