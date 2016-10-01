package jeein2.androidsocket1;

import android.app.Activity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;
import android.view.View;

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

/**
 * Created by HANSUNG on 2016-09-30.
 */

public class ClientActivity extends Activity {

    private static final int PORT = 9100; //서버에서 설정한 PORT 번호
    String ip="172.31.28.184"; //서버 단말기의 IP주소..
    //본 예제는 Genymotion 에뮬레이터 2대로 테스한 예제입니다.
    //Genymotion을 실행하면 각 에뮬레이터의 IP를 확인할 수 있습니다.

    int flag= 0;
    Socket socket;     //클라이언트의 소켓
    DataInputStream is;
    DataOutputStream os;

    TextView text_msg;  //서버로 부터 받은 메세지를 보여주는 TextView
    EditText edit_msg;  //서버로 전송할 메세지를 작성하는 EditText
    EditText edit_ip;   //서버의 IP를 작성할 수 있는 EditText

    String msg="";

    boolean isConnected=true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_client);

        text_msg=(TextView)findViewById(R.id.text_massage_from_server);
        edit_msg=(EditText)findViewById(R.id.edit_message_to_server);
        edit_ip=(EditText)findViewById(R.id.edit_addressofserver);
        edit_ip.setText(ip);
        ///////////////////////////
        edit_msg.setText("basic");

    }

    //Button 클릭시 자동으로 호출되는 callback 메소드
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

                            //서버와 연결하는 소켓 생성..
                            socket= new Socket(InetAddress.getByName(ip), PORT );


                            //여기까지 왔다는 것을 예외가 발생하지 않았다는 것이므로 소켓 연결 성공..
                            //서버와 메세지를 주고받을 통로 구축
                            is=new DataInputStream(socket.getInputStream());
                            os=new DataOutputStream(socket.getOutputStream());

                        } catch (IOException e) {

                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }


                        //서버와 접속이 끊길 때까지 무한반복하면서 서버의 메세지 수신
                        while(true){
                            try {
                                if(flag == 0) {
                                    BufferedReader in = new BufferedReader(new InputStreamReader(
                                            socket.getInputStream()));
                                    msg = in.readLine();

                                    flag =1;
                                }
                                //msg= is.readUTF(); //서버 부터 메세지가 전송되면 이를 UTF형식으로 읽어서 String 으로 리턴

                                //서버로부터 읽어들인 메시지msg를 TextView에 출력..
                                //안드로이드는 오직 main Thread 만이 UI를 변경할 수 있기에
                                //네트워크 작업을 하는 이 Thread에서는 TextView의 글씨를 직접 변경할 수 없음.
                                //runOnUiThread()는 별도의 Thread가 main Thread에게 UI 작업을 요청하는 메소드임.
                                runOnUiThread(new Runnable() {

                                    @Override
                                    public void run() {
                                        // TODO Auto-generated method stub
                                        text_msg.setText(msg);

                                    }
                                });
                                //////////////////////////////////////////////////////////////////////////
                              ///////////////////////////////////////
                            } catch (IOException e) {
                                // TODO Auto-generated catch block
                                e.printStackTrace();
                            }

                        }//while

                    }//run method...

                }).start();//Thread 실행..

                break;

            case R.id.btn_send_client: //서버로 메세지 전송하기...

                if(os==null){text_msg.setText("you didnt connet server...");
                    return;}   //서버와 연결되어 있지 않다면 전송불가..

                //네트워크 작업이므로 Thread 생성
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub

                        //서버로 보낼 메세지 EditText로 부터 얻어오기
                        String msg= edit_msg.getText().toString();

                        try {

                            PrintWriter out = new PrintWriter(new BufferedWriter(
                                    new OutputStreamWriter(socket.getOutputStream())), true);
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