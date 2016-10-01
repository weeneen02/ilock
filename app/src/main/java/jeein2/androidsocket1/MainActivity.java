package jeein2.androidsocket1;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

/**
 * Created by HANSUNG on 2016-09-30.
 */
///////////////
public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    //Button을 클릭했을 때 호출되는 callback 메소드
    public void mOnClick(View v){

        Intent i;
        switch(v.getId()){
            case R.id.btn_server: //서버 화면
                i= new Intent(this, ServerActivity.class);
                startActivity(i);
                break;
            case R.id.btn_client: //클라이언트 화면
                i= new Intent(this, ClientActivity.class);
                startActivity(i);
                break;
        }
    }
}
