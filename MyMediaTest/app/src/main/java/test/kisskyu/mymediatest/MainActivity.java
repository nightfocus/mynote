package test.kisskyu.mymediatest;

import android.Manifest;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.hardware.Camera;
import android.view.ViewDebug;
import android.widget.Button;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class MainActivity extends AppCompatActivity
{

    public SurfaceView mSurfaceView = null;
    Camera mCamera = null;
    Button stButton;
    TextView text1;
    Camera.Parameters camParams = null;
    private Queue<byte[]> _YUVQueue = new LinkedList<byte[]>();
    private Lock _queueLock = new ReentrantLock();
    Condition _queueCond = _queueLock.newCondition(); // Condition实例
    Boolean bStartFlag = true;

    int vWidth = 720;
    int vHeight = 540;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        stButton = (Button) this.findViewById(R.id.StartButton);
        text1 = (TextView) this.findViewById(R.id.textView);
        mSurfaceView = (SurfaceView) this.findViewById(R.id.surfaceViewEx);
    }

    public void onStartVideoTest(View v)
    {
        try
        {
            // 申请camera权限.
            if(ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED)
            {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, 123);
                return;
            }
            // 申请文件写权限.
            if(ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
            {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 124);
                return;
            }

            bStartFlag = true;
            Thread handleYUVThread = new Thread(handleYUVRunnable);
            handleYUVThread.setPriority(Thread.MAX_PRIORITY);
            handleYUVThread.start();

            mSurfaceView.getHolder().setFixedSize(720, 1080);
            mSurfaceView.getHolder().addCallback(new SurfaceCallBack());
            try
            {
                if(mCamera == null)
                {
                    mCamera = Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK);
                    initCamera();
                }
                mCamera.setPreviewDisplay(mSurfaceView.getHolder());
                mCamera.startPreview();
                System.out.println("startPreview");
            } catch (Exception e)
            {
                e.printStackTrace();
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public void onStopVideoTest(View v)
    {
        releaseCameraAndResource();
    }

    /**
     * 开始camera的预览
     */
    private void initCamera()
    {
        camParams = mCamera.getParameters();
        Camera.Size prevewSize = camParams.getPreviewSize();
        List<Camera.Size> PreviewSizeList = camParams.getSupportedPreviewSizes();
        List<Integer> PreviewFormats = camParams.getSupportedPreviewFormats();
        List<int[]> FpsRange = camParams.getSupportedPreviewFpsRange();
        setOrientation();

        /**设置一堆属性并给camera设置这些属性*/
        // camParams.setWhiteBalance(Camera.Parameters.WHITE_BALANCE_AUTO);
        // camParams.setSceneMode(Camera.Parameters.SCENE_MODE_AUTO);
        // camParams.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        camParams.setPreviewFpsRange(15000, 15000); // 固定15帧. 必须是 getSupportedPreviewFpsRange() 返回列表中的值
        camParams.setPreviewFormat(ImageFormat.NV21); // 必须是 getSupportedPreviewFormats() 返回列表中的值
        // camParams.setPictureSize(1920, 1080);
        camParams.setPreviewSize(vWidth, vHeight); // 必须是 getSupportedPreviewSizes() 返回列表中的值
        camParams.setFlashMode("off"); // 无闪光灯

        // 上述几个属性设置的和真实手机的不一样时，就会报错
        mCamera.setParameters(camParams);


        // 如果在这使用addCallbackBuffer，那么表示需要自己来维护每次每调的内存，
        // 将setPreviewCallback改为setPreviewCallbackWithBuffer.
        // 接下来，在 onPreviewFrame() 中，就必须再次进行addCallbackBuffer,
        // 而且，要先调用 addCallbackBuffer，然后再处理帧数据，否则会降低帧率.
        // 回收缓存，下次仍然会使用，所以不需要再开辟新的缓存，达到优化的目的
        // byte[] buf = new byte[480 * 320 * 3 / 2]; // 230400
        // mCamera.addCallbackBuffer(buf);

        mCamera.setPreviewCallback(new Camera.PreviewCallback()
        {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera)
            {
                int len = data.length;

                // text1.append(String.valueOf(len));
                _queueLock.lock();
                if (_YUVQueue.size() < 20) // 最多缓存20帧.
                {
                    // 如果是横屏拍摄，则不需要做 90度的转换.
                    byte[] tmpyuv420 = rotateYUV420Degree90(data, vWidth, vHeight);
                     _YUVQueue.offer(nv21ToI420(tmpyuv420, vWidth, vHeight));
                }
                else
                {
                    System.out.println("Drop a frame.");
                }
                _queueCond.signal();
                _queueLock.unlock();
            }
        });

        return;
    }

    private void releaseCameraAndResource()
    {
        if(mCamera != null)
        {
            mCamera.setPreviewCallback(null);
            mCamera.stopPreview();
            mCamera.release();

            // 让线程 handleYUVRunnable 退出.
            _queueLock.lock();
            _queueCond.signal();
            _queueLock.unlock();
            bStartFlag = false;
            // _queueCond.signal();

            mCamera = null;
        }
    }

    /**
     * YUV数据处理线程.
     */
    private Runnable handleYUVRunnable = new Runnable() {
        @Override
        public void run()
        {
            FileOutputStream fileOut = null;

            while (bStartFlag)
            {
                try
                {
                    _queueLock.lock();
                    _queueCond.await();
                    while (bStartFlag)
                    {
                        byte[] yuvData = _YUVQueue.poll();
                        if (yuvData == null)
                        {
                            break;
                        }
                        Date d = new Date(System.currentTimeMillis());
                        System.out.print(d.toString() + ": ");
                        System.out.println(yuvData.length);
                        try
                        {
                            if (fileOut == null)
                            {
                                fileOut = new FileOutputStream("/sdcard/mymediatestdata.yuv", false);
                            }
                            fileOut.write(yuvData);
                        }
                        catch (IOException e)
                        {
                            e.printStackTrace();
                        }
                    }
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
                finally
                {
                    _queueLock.unlock();
                }
            }

            try
            {
                if (fileOut != null)
                    fileOut.close();
            }
            catch (Exception e)
            {
            }

            return;
        }

    };

    /**
     * 假定有一张图片的大小为width*height, YUV420图片数据的大小就是width*height*3/2
       NV21
        前面width*height个字节存的是每个像素的Y分量，后面的width*height/2字节是VUVUVUVU...这样存的。
       I420
        前面的width*height个字节还是每个像素的Y分量，接下来的width*height/4字节是U分量，最后的width*height/4字节是V分量。
    */
    private byte[] nv21ToI420(byte[] data, int width, int height)
    {
        byte[] ret = new byte[data.length];
        int total = width * height;

        ByteBuffer bufferY = ByteBuffer.wrap(ret, 0, total);
        ByteBuffer bufferU = ByteBuffer.wrap(ret, total, total / 4);
        ByteBuffer bufferV = ByteBuffer.wrap(ret, total + total / 4, total / 4);

        bufferY.put(data, 0, total);
        for (int i=total; i<data.length; i+=2)
        {
            bufferV.put(data[i]);
            bufferU.put(data[i+1]);
        }

        return ret;
    }

    private byte[] rotateYUV420Degree90(byte[] data, int imageWidth, int imageHeight)
    {
        byte [] yuv = new byte[imageWidth*imageHeight*3/2];
        // Rotate the Y luma
        int i = 0;
        for(int x = 0;x < imageWidth;x++)
        {
            for(int y = imageHeight-1;y >= 0;y--)
            {
                yuv[i] = data[y*imageWidth+x];
                i++;
            }
        }
        // Rotate the U and V color components
        i = imageWidth*imageHeight*3/2-1;
        for(int x = imageWidth-1;x > 0;x=x-2)
        {
            for(int y = 0;y < imageHeight/2;y++)
            {
                yuv[i] = data[(imageWidth*imageHeight)+(y*imageWidth)+x];
                i--;
                yuv[i] = data[(imageWidth*imageHeight)+(y*imageWidth)+(x-1)];
                i--;
            }
        }
        return yuv;
    }


    private void setOrientation()
    {
        // 横竖屏镜头自动调整
        if (this.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE)
        {
            // camParams.set("orientation", "portrait"); //
            // camParams.set("rotation", 90); // 镜头角度转90度（默认摄像头是横拍）
            mCamera.setDisplayOrientation(90); // 在2.2以上可以使用
        }
        else
        {
            // 如果是横屏
            // camParams.set("orientation", "landscape"); //
            mCamera.setDisplayOrientation(0); // 在2.2以上可以使用
        }
    }


    private final class SurfaceCallBack implements SurfaceHolder.Callback
    {
        /*
        public SurfaceCallBack()
        {
            mSurfaceView.getHolder().addCallback(this);
        }
        */

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
        {

        }

        // 这个不会被调用...
        // 资料：使用动态加载SurfaceView可以调用，就是new一个SurfaceView对象，然后addView(mSurfaceView),
        // 具体没做过。
        @Override
        public void surfaceCreated(SurfaceHolder holder)
        {
        }

        // 在APP被切换置后台时，会被触发. 比如按了Home键
        @Override
        public void surfaceDestroyed(SurfaceHolder holder)
        {
            releaseCameraAndResource();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        if(requestCode != 123 && requestCode != 124)
            return;

        for(int res : grantResults)
        {
            if(getPackageManager().PERMISSION_GRANTED == res)
            {
                onStartVideoTest(stButton);
            }
            else
            {
                // Do nothing.
            }
        }
    }

}
