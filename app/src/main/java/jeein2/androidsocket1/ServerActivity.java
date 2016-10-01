package jeein2.androidsocket1;


import android.app.Activity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Button;
import android.widget.EditText;

import android.app.Activity;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import android.view.View;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Created by HANSUNG on 2016-09-30.
 */

public class ServerActivity extends Activity {

    static final int PORT=10001;
    ServerSocket serversocket;
    Socket socket;
    DataInputStream is;
    DataOutputStream os;

    TextView text_msg; //클라이언트로부터 받을 메세지를 표시하는 TextView
    EditText edit_msg; //클라이언트로 전송할 메세지를 작성하는 EditText

    String msg="";

    boolean isConnected=true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_server);

        text_msg= (TextView)findViewById(R.id.text_massage_from_client);
        edit_msg= (EditText)findViewById(R.id.edit_message_to_client);
    }

    //Button 클릭시 자동으로 호출되는 callback 메소드
    public void mOnClick(View v){
        switch(v.getId()){
            case R.id.btn_start_server: //채팅 서버 구축 및 클라이언트로 부터 메세지 받기

                //Android API14버전이상 부터 네트워크 작업은 무조건 별도의 Thread에서 실행 해야함.
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub
                        try {
                            //서버소켓 생성.
                            serversocket=new ServerSocket(PORT);
                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }

                        try {
                            //서버에 접속하는 클라이언트 소켓 얻어오기(클라이언트가 접속하면 클라이언트 소켓 리턴)
                            socket= serversocket.accept(); //서버는 클라이언트가 접속할 때까지 여기서 대기...

                            //여기 까지 왔다는 것은 클라이언트가 접속했다는 것을 의미하므로
                            //클라이언트와 데이터를 주고 받기 위한 통로구축..
                            is= new DataInputStream(socket.getInputStream()); //클라이언트로 부터 메세지를 받기 위한 통로
                            os= new DataOutputStream(socket.getOutputStream()); //클라이언트로 메세지를 보내기 위한 통로

                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }


                        //클라이언트가 접속을 끊을 때까지 무한반복하면서 클라이언트의 메세지 수신
                        while(isConnected){
                            try {        msg=is.readUTF();
                            } catch (IOException e) {
                                // TODO Auto-generated catch block
                                e.printStackTrace();
                            }

                            //클라이언트로부터 읽어들인 메시지msg를 TextView에 출력..
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
                            /////////////////////////////////////////////////////////////////////////////

                        }//while..

                    }//run method...

                }).start(); //Thread 실행..

                break;

            case R.id.btn_send_server: // 클라이언트로 메세지 전송하기.

                if(os==null) return; //클라이언트와 연결되어 있지 않다면 전송불가..

                //네트워크 작업이므로 Thread 생성
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub

                        //클라이언트로 보낼 메세지 EditText로 부터 얻어오기
                        String msg= edit_msg.getText().toString();

                        try {
                            os.writeUTF(msg); //클라이언트로 메세지 보내기.UTF 방식으로(한글 전송가능...)
                            os.flush();   //다음 메세지 전송을 위해 연결통로의 버퍼를 지워주는 메소드..
                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }
                }).start(); //Thread 실행..

                break;
        }
    }
}