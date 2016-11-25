package cn.kisskyu.mysecurememo;

import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class FirstCoverActivity extends AppCompatActivity
{
    TextView text1;
    Handler timeHandler;
    Intent goMain;

    Runnable runnable=new Runnable()
    {
        @Override
        public void run() {
            // TODO Auto-generated method stub
            //要做的事情
            // text1.setText("a");
            // text1.setTextColor(0xFF0000FF);
            // timeHandler.postDelayed(this, 2000);

            // 显式调用方式
            // Intent goMain = new Intent(this, MainActivity.class);
            // 隐式调用方式
            goMain = new Intent("ActualMainPage");
            startActivity(goMain);

            // 移除定时器
            timeHandler.removeCallbacks(this);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_cover);

        text1 = (TextView)findViewById(R.id.text1);

        goMain = new Intent("ActualMainPage");

        // 创建一个定时器，2秒后触发.
        timeHandler = new Handler();
        timeHandler.postDelayed(runnable, 5000); // 5秒后执行一次runnable
    }

    public void onEntryMain(View v)
    {
        timeHandler.removeCallbacks(runnable);
        startActivity(goMain);
    }
}
