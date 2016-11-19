package jeein2.ilock1;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

import static android.content.ContentValues.TAG;

public class MainActivity extends BlunoLibrary  {
/////////////////////////////////////////////////////////////////////////////
    private static final int PORT = 9100; //서버에서 설정한 PORT 번호
    //String ip="172.31.27.202"; //서버 단말기의 IP주소..
    String ip="192.168.43.169";

    int flag= 0;
    Socket socket;     //클라이언트의 소켓
    DataInputStream is;
    DataOutputStream os;
    TextView text_msg;  //서버로 부터 받은 메세지를 보여주는 TextView
    EditText edit_msg;  //서버로 전송할 메세지를 작성하는 EditText
    EditText edit_ip;   //서버의 IP를 작성할 수 있는 EditText
    String msg="";
 //////////////////////////////////////////////////////////////////////////////
    private Button buttonScan;
    private Button buttonSerialSend;
    private EditText serialSendText;
    private TextView serialReceivedText;
    private TextView connecteddevicename;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);//메모리상에 레이아웃을 객체화 시킨다. 숫서가 뒤바뀌면 오류가 발생하게 됨
        //다른 요소를 객체화 하려면  따로 또 셋컨텐트뷰를 만들어줘야 한다. 시스템서비스로 제공되는 기능들은 모두 get systemservice()f로 참조해줘야 한다.


        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this,"ble_not_supported", Toast.LENGTH_SHORT).show();
            finish();
        }
        else
        {
            Toast.makeText(this,"ble_supported", Toast.LENGTH_SHORT).show();
        }

        onCreateProcess();														//onCreate Process by BlunoLibrary


        serialBegin(115200);													//set the Uart Baudrate on BLE chip to 115200

        serialReceivedText=(TextView) findViewById(R.id.serialReveicedText);	//initial the EditText of the received data
        serialSendText=(EditText) findViewById(R.id.serialSendText);			//initial the EditText of the sending data
        connecteddevicename=(TextView) findViewById(R.id.connected_device_name);//연결한 디바이스의 정보를 표시해주고 나중에 이값을 이용하여 서버에 송신해야 할 값일 것이다.
        buttonSerialSend = (Button) findViewById(R.id.buttonSerialSend);		//initial the button for sending the data

////////////////////////////////////////////////////////////////////////////////////
        text_msg=(TextView)findViewById(R.id.text_massage_from_server);
        edit_msg=(EditText)findViewById(R.id.edit_message_to_server);
        edit_ip=(EditText)findViewById(R.id.edit_addressofserver);
        edit_ip.setText(ip);
        edit_msg.setText("");
        ////////////////////////////////////////////////////////////////////////






        buttonSerialSend.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub

                serialSend(serialSendText.getText().toString());				//send the data to the BLUNO 여기서 쓴글자를 보내게 된다. 블루노라이브러리 클래스에 정의되어 있음 거기로 가라
            }
        });

        buttonScan = (Button) findViewById(R.id.buttonScan);					//initial the button for scanning the BLE device
        buttonScan.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub

                buttonScanOnClickProcess();										//Alert Dialog for selecting the BLE device
            }
        });
    }

    protected void onResume(){
        super.onResume();
        System.out.println("BlUNOActivity onResume");
        onResumeProcess();														//onResume Process by BlunoLibrary
    }



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        onActivityResultProcess(requestCode, resultCode, data);					//onActivityResult Process by BlunoLibrary
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onPause() {
        super.onPause();
        onPauseProcess();														//onPause Process by BlunoLibrary
    }

    protected void onStop() {
        super.onStop();
        onStopProcess();														//onStop Process by BlunoLibrary
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        onDestroyProcess();														//onDestroy Process by BlunoLibrary
    }

    @Override
    public void onConectionStateChange(connectionStateEnum theConnectionState) {//Once connection state changes, this function will be called
        switch (theConnectionState) {											//Four connection state
            case isConnected:
                buttonScan.setText("Connected");
                break;
            case isConnecting:
                buttonScan.setText("Connecting");
                break;
            case isToScan:
                buttonScan.setText("Scan");
                break;
            case isScanning:
                buttonScan.setText("Scanning");
                break;
            case isDisconnecting:
                buttonScan.setText("isDisconnecting");
                break;
            default:
                break;
        }
    }

    @Override
    public void onSerialReceived(String theString) {							//Once connection data received, this function will be called
        // TODO Auto-generated method stub
        serialReceivedText.append(theString);							//append the text into the EditText
        //The Serial data from the BLUNO may be sub-packaged, so using a buffer to hold the String is a good choice.
        ((ScrollView)serialReceivedText.getParent()).fullScroll(View.FOCUS_DOWN);
    }


    //내가 추가한 부분 필요한 부분 봐서 지워라
    @Override
    public void onDeviceNameReceived(String theString) {							//Once connection data received, this function will be called
        // TODO Auto-generated method stub
        connecteddevicename.setText(theString);							//append the text into the EditText

        edit_msg.setText(theString);
        //The Serial data from the BLUNO may be sub-packaged, so using a buffer to hold the String is a good choice.

    }

    public void mOnClick(View v){
        switch(v.getId()){
            case R.id.btn_connectserver://서버에 접속하고 서버로 부터 메세지 수신하기
                //Android API14버전이상 부터 네트워크 작업은 무조건 별도의 Thread에서 실행 해야함.
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub
                        try {
                            ip= edit_ip.getText().toString();//IP 주소가 작성되어 있는 EditText에서 서버 IP 얻어오기
                            Log.d(TAG, "ip is"+ip);
                            //서버와 연결하는 소켓 생성..
                            socket= new Socket(InetAddress.getByName(ip), PORT );
                            Log.d(TAG, "socket created..");
                            //여기까지 왔다는 것을 예외가 발생하지 않았다는 것이므로 소켓 연결 성공..
                            //서버와 메세지를 주고받을 통로 구축
                            is=new DataInputStream(socket.getInputStream());
                            os=new DataOutputStream(socket.getOutputStream());

                            BufferedReader in = new BufferedReader(new InputStreamReader(
                                    socket.getInputStream()));

                            msg = in.readLine();
                            //msg= is.readUTF(); //서버 부터 메세지가 전송되면 이를 UTF형식으로 읽어서 String 으로 리턴
                            //서버로부터 읽어들인 메시지msg를 TextView에 출력..
                            //안드로이드는 오직 main Thread 만이 UI를 변경할 수 있기에
                            //네트워크 작업을 하는 이 Thread에서는 TextView의 글씨를 직접 변경할 수 없음.
                            //runOnUiThread()는 별도의 Thread가 main Thread에게 UI 작업을 요청하는 메소드임.
                            runOnUiThread(new Runnable() {

                                @Override
                                public void run() {
                                    // TODO Auto-generated method stub
                                    if(msg != "")
                                    {
                                        text_msg.setText(msg);
                                    }
                                }
                            });
                            socket.close();
                            Log.d(TAG, "socket closed..");
                            //////////////////////////////////////////////////////////////////////////
                            ////////////////////////////////////////////////////////////
                        } catch (IOException e) {

                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }//run method...

                }).start();//Thread 실행..
            case R.id.btn_send_client: //서버로 메세지 전송하기...
                if(os==null){text_msg.setText("you didnt connet server...");
                    return;}   //서버와 연결되어 있지 않다면 전송불가..
                //네트워크 작업이므로 Thread 생성//
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub
                        //서버로 보낼 메세지 EditText로 부터 얻어오기
                        String msg= edit_msg.getText().toString();
                        Log.d(TAG, "string is "+msg+"...");
                        msg = msg.concat("\0"); //테스트용 해보고 안디면 지우기 이렇게 하는게 문자열의 끝을 알리는데는 좋다..
                        try {
                            PrintWriter out = new PrintWriter(new BufferedWriter(
                                    new OutputStreamWriter(socket.getOutputStream())), true);
                            out.flush();

                            //msg = msg.concat("\n");//이렇게 보내야지 c로 잔 프로그램에서 스트링의 끝을 알 수 있습니다.
                            out.println(msg);
                            out.flush();
                            // os.writeChars(msg);  //서버로 메세지 보내기.UTF 방식으로(한글 전송가능...)
                            // os.flush();        //다음 메세지 전송을 위해 연결통로의 버퍼를 지워주는 메소드..
                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }//run method..

                }).start(); //Thread 실행..
                break;
        }
    }


}
