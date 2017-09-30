package cn.kisskyu.myservicedemo;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class MyService extends Service
{
    /**
     * 进度条的最大值
     */
    public static final int MAX_PROGRESS = 100;
    /**
     * 进度条的进度值
     */
    private int progressPercent = 0;

    /**
     * 增加get()方法，供Activity调用
     * @return 下载进度
     */
    public int getProgress()
    {
        return progressPercent;
    }

    public MyService()
    {
    }

    @Override
    public void onCreate()
    {
        super.onCreate();
        Log.d("MyService", "MyService start.");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        Log.d("MyService", "Entry onStartCommand"); // 这个貌似不会被触发
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy()
    {
        stop1();
        super.onDestroy();
        Log.d("MyService", "Entry onDestroy");
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return new MyWorkBind();
    }


    private int cState = 0; //  0: stop; 1:start; 2: pause
    public void start1()
    {
        if(cState != 0)
            return;

        new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                cState = 1;
                try
                {
                    /* 模拟每1秒完成 1%
                    * */
                    while (cState > 0)
                    {
                        if(cState == 1)
                        {
                            Log.d("MyService", String.valueOf(progressPercent));
                            progressPercent++;
                            if(progressPercent == 100)
                                break;
                        }
                        Thread.sleep(1000);
                    }
                }
                catch(Exception e)
                {

                }
                finally
                {
                    cState = 0;
                    progressPercent = 0;
                }
            }
        }).start();
    }

    // 停止
    public void stop1()
    {
        cState = 0;
    }

    // 暂停
    public void pause1()
    {
        if(cState == 0)
            return;
        else if(cState == 1)
            cState = 2;
        else
            cState = 1;
    }

    class MyWorkBind extends Binder
    {
        public MyService returnMyService()
        {
            return MyService.this;
        }
    }
}
