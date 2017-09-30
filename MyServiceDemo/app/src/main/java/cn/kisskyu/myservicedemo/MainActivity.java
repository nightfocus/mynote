package cn.kisskyu.myservicedemo;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;

public class MainActivity extends AppCompatActivity implements View.OnClickListener
{

    private static String TAG = "MainActivity";
    private Button btnStart, btnStop, btnPause;
    private MyService myService;
    private int progress = 0;
    private ProgressBar mProgressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mProgressBar = (ProgressBar) findViewById(R.id.progressBar);
        btnStart = (Button)findViewById(R.id.bstart);
        btnStop = (Button)findViewById(R.id.bstop);
        btnPause = (Button)findViewById(R.id.bpause);

        btnStart.setOnClickListener(this);
        btnStop.setOnClickListener(this);
        btnPause.setOnClickListener(this);

        // 绑定Service
        Intent intent = new Intent(this, MyService.class);
        // 当服务绑定成功时，会先解发 MyService::onBind， 然后调用 onServiceConnected .
        // 解绑时，是调用 unbindService 方法, like:
        // unbindService(conn);
        bindService(intent, conn, Context.BIND_AUTO_CREATE);
    }


    /**
     * 监听进度，每0.1秒获取调用MsgService的getProgress()方法来获取进度，更新UI
     */
    public void listenProgress()
    {
        new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                progress = 0;
                mProgressBar.setVisibility(View.VISIBLE);

                while(progress < MyService.MAX_PROGRESS)
                {
                    progress = myService.getProgress();
                    mProgressBar.setProgress(progress);
                    try
                    {
                        Thread.sleep(100);
                    } catch (InterruptedException e)
                    {
                        e.printStackTrace();
                    }
                }

            }
        }).start();
    }

    @Override
    public void onClick(View v)
    {
        switch (v.getId())
        {
            case R.id.bstart:
                Log.d(TAG, "click bstart button.");
                myService.start1();  // 通知后台服务，开始工作
                listenProgress();
                break;
            case R.id.bstop:
                myService.stop1();
                progress = MyService.MAX_PROGRESS; // 这样可以退出 listenProgress 中的线程.
                break;
            case R.id.bpause:
                myService.pause1(); // 暂停
                break;
        }
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        unbindService(conn); // 解绑这个后台服务，会解发 MyService::onDestroy
    }

    ServiceConnection conn = new ServiceConnection()
    {

        @Override
        public void onServiceDisconnected(ComponentName name)
        {
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service)
        {
            //返回一个MyService对象
            myService = ((MyService.MyWorkBind)service).returnMyService();
        }
    };
}
