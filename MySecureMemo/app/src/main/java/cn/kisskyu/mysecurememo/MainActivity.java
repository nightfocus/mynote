package cn.kisskyu.mysecurememo;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Vibrator;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.util.Base64;
import android.view.GestureDetector;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.text.TextWatcher;
import android.widget.PopupMenu;
import android.widget.TextView;
import java.io.FileInputStream;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;


public class MainActivity extends AppCompatActivity
{
    EditText memoText, passDecipher;
    TextView tipText;
    Button page1, page2, page3, page4;
    Button buttonDecipher, buttonSave;
    String passwdMd5; // 保存好的密码签名，用于验证密码是否正确
    int currPage;
    boolean has_READ_EXTERNAL_STORAGE = false;
    private GestureDetector gestureDetector;

    // 定义一个匿名类，实现TextWatcher这个接口，并创建该类的对象myTextWatcher，
    // 这样在这个类中，可以直接使用 MainActivity 类的成员变量。
    private TextWatcher myTextWatcher = new TextWatcher()
    {
        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after)
        {
        }

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count)
        {
        }

        @Override
        public void afterTextChanged(Editable s)
        {
        }
    };

    // 定义一个广播接收器，用于接收Android发送的广播消息.
    private BroadcastReceiver myReceiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent) {
            // TODO Auto-generated method stub
            if (Intent.ACTION_SCREEN_ON.equals(intent.getAction()) )
            {
                //当按下电源键，屏幕亮起的时候
            }

            if (Intent.ACTION_SCREEN_OFF.equals(intent.getAction()) )
            {
                //当按下电源键，屏幕变黑的时候. 重新初始化数据
                initialData();
            }
            if (Intent.ACTION_USER_PRESENT.equals(intent.getAction()) )
            {
                //当解除锁屏的时候
            }
        }
    };

    private View.OnTouchListener memoTextTouchListener = new View.OnTouchListener()
    {
        @Override
        public boolean onTouch(View v, MotionEvent event)
        {
            return gestureDetector.onTouchEvent(event);
        }
    };

    static
    {
        System.loadLibrary("cencryptlib");
    }
    /*
    *  在这个目录下：
    *  D:\github.private\mynote\MySecureMemo\app\src\main
    *  执行：
    *  javah -d jni -classpath D:\android-sdk\platforms\android-23\android.jar;D:\android-sdk\extras\android\support\v7\appcompat\libs\android-support-v7-appcompat.jar;D:\android-sdk\extras\android\support\v4\android-support-v4.jar;D:\github.private\mynote\MySecureMemo\app\build\intermediates\classes\debug cn.kisskyu.mysecurememo.MainActivity
    *  会生成jni目录和.h文件.
    * */
    // 根据用户的原始密码，生成加解密所需的密钥.
    public synchronized static native String cEncryptPwd(String pwd);

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        memoText = (EditText)findViewById(R.id.memoText);
        passDecipher = (EditText)findViewById(R.id.passDecipher);
        buttonDecipher = (Button)findViewById(R.id.buttonDecipher);
        buttonSave = (Button)findViewById(R.id.buttonSave);
        tipText = (TextView)findViewById(R.id.tipText);
        page1 = (Button)findViewById(R.id.imageView1);
        page2 = (Button)findViewById(R.id.imageView2);
        page3 = (Button)findViewById(R.id.imageView3);
        page4 = (Button)findViewById(R.id.imageView4);
        currPage = 1; // 默认为第一页.


        initialData();
        // 注册文本框的事件
        // memoText.addTextChangedListener(myTextWatcher);

        // 捕获电源按键，当黑屏后，需恢复到密文状态
        registerReceiver(myReceiver, new IntentFilter(Intent.ACTION_SCREEN_OFF));

        // 注册4个按钮的长按事件.
        // 这也可以使用匿名类的语法，而不是每个注册时都new一个对象
        page1.setOnLongClickListener(new OnLongClickPage());
        page2.setOnLongClickListener(new OnLongClickPage());
        page3.setOnLongClickListener(new OnLongClickPage());
        page4.setOnLongClickListener(new OnLongClickPage());

        // 捕获memoText控件中的手指事件. 仅在控件状态为 Enabled 时可触发。
        // 没啥用，纯属测试.
        memoText.setOnTouchListener(memoTextTouchListener);

        gestureDetector = new GestureDetector(MainActivity.this, onGestureListener);

    }

    /*
        back键
        Android的程序无需刻意的去退出,当你一按下手机的back键的时候，系统会默认调用程序栈中最上层Activity的Destroy()方法来，销毁当前Activity。
        当此Activity又被其它Activity启动起来的时候,会重新调用OnCreate()方法进行创建,
        当栈中所有Activity都弹出结束后,应用也就随之结束了.如果说程序中存在service之类的,则可以在恰当的位置监听处理下也就可以了.

        home键
        Android程序的隐藏,当你按下手机的Home键的时候,系统会默认调用程序栈中最上层Activity的stop()方法,
        然后整个应用程序都会被隐藏起来,当你再次点击手机桌面上应用程序图标时,
        系统会调用最上层Activity的OnResume()方法,此时不会重新打开程序,而是直接进入,会直接显示程序栈中最上层的Activity.
    * */
    @Override
    public void onBackPressed()
    {
        // 显式的打开activity
        // 当FirstCoverActivity.class 的 launchMode 是默认时，会重新创建这个活动;
        // 但是，当 FirstCoverActivity.class 的 launchMode 是 singleTask时，则当前这个活动的 onDestroy 会被触发。
        // 即便当前这个活动也是 signleTask.
        startActivity(new Intent(this, FirstCoverActivity.class));
    }

    private GestureDetector.OnGestureListener onGestureListener = new GestureDetector.SimpleOnGestureListener()
    {
        // 用户按下触摸屏、快速移动后松开，由1个MotionEvent ACTION_DOWN, 多个ACTION_MOVE, 1个ACTION_UP触发
        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
                               float velocityY)
        {
            float x = e2.getX() - e1.getX();
            float y = e2.getY() - e1.getY();

            if (x > 0)
            {
                // 右滑了
            }
            else if (x < 0)
            {
                // 左滑了
            }
            return true;
        }

        // 这里仅重载了onFling，还有：
        // onDown 用户轻触触摸屏，由1个MotionEvent ACTION_DOWN触发
        // onShowPress 用户轻触触摸屏，尚未松开或拖动，由一个1个MotionEvent ACTION_DOWN触发
        // onSingleTapUp 用户（轻触触摸屏后）松开，由一个1个MotionEvent ACTION_UP触发
        // onScroll 用户按下触摸屏，并拖动，由1个MotionEvent ACTION_DOWN, 多个ACTION_MOVE触发
        // onLongPress 用户长按触摸屏，由多个MotionEvent ACTION_DOWN触发
    };

    // 在当前Activity中响应手指事件.
    // 测试中如果在当前Activity的某个控件中touch，不会触发该方法. 必须在空白处touch.
    public boolean onTouchEvent(MotionEvent event)
    {
        return gestureDetector.onTouchEvent(event);
    }
    // */

    // Activity关闭时会被触发
    @Override
    protected void onDestroy()
    {
        // System.out.println("onDestroy.");
        super.onDestroy();
        unregisterReceiver(myReceiver);
    }

    public void OnPage1(View v)
    {
        if(currPage == 1)
            return;
        currPage = 1;
        initialData();
        // 开启时长为200毫秒的振动
        Vibrator vibrator=(Vibrator)getSystemService(Service.VIBRATOR_SERVICE);
        vibrator.vibrate(new long[]{0, 200}, -1);
    }
    public void OnPage2(View v)
    {
        if(currPage == 2)
            return;
        currPage = 2;
        initialData();
        Vibrator vibrator=(Vibrator)getSystemService(Service.VIBRATOR_SERVICE);
        vibrator.vibrate(new long[]{0, 200}, -1);
    }
    public void OnPage3(View v)
    {
        if(currPage == 3)
            return;
        currPage = 3;
        initialData();
        Vibrator vibrator=(Vibrator)getSystemService(Service.VIBRATOR_SERVICE);
        vibrator.vibrate(new long[]{0, 200}, -1);
    }
    public void OnPage4(View v)
    {
        if(currPage == 4)
            return;
        currPage = 4;
        initialData();
        Vibrator vibrator=(Vibrator)getSystemService(Service.VIBRATOR_SERVICE);
        vibrator.vibrate(new long[]{0, 200}, -1);
    }

    public void OnDecipher(View v)
    {
        String cipherText = memoText.getText().toString();
        String password = passDecipher.getText().toString();
        if(cipherText.isEmpty() || password.isEmpty() || !checkPasswd(password))
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("没有读取到数据，或者输入的密码不对。");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return;
        }

        String plainMemo = personalDecrypt(cipherText, password);
        if(plainMemo.isEmpty())
            return;
        memoText.setText(plainMemo);
        memoText.setSelection(plainMemo.length());

        setViewStatus();
    }

    public void OnMemoSave(View v)
    {
        String cipherText="";

        String plainMemo = memoText.getText().toString();
        String password = passDecipher.getText().toString();
        if(plainMemo.isEmpty())
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("该页数据将恢复到初始状态，原有密码将被清除。");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();

            passwdMd5 = "";
        }
        else
        {
            passwdMd5 = generatePasswdMd5(password);
            cipherText = personalEncrypt(plainMemo, password);
            if (cipherText.isEmpty())
                return;
        }

        SharedPreferences setting = getPreferences(Activity.MODE_PRIVATE);
        SharedPreferences.Editor editor = setting.edit();
        editor.putString(getSharedDataKey(), cipherText);
        editor.putString(getSharedPwdKey(), passwdMd5);
        editor.commit();

        // 从SharedPreferences 中读取出来回显密文
        String mbase64text = setting.getString(getSharedDataKey(), "");
        setWaitDesStatus();
        memoText.setText(mbase64text);
    }

    // 密码校验，根据用户的原始密码passwd计算MD5，再和保存好的密码对比.
    private  boolean checkPasswd(String passwd)
    {
        String newPasswdMd5 = generatePasswdMd5(passwd);
        return (newPasswdMd5.equals(passwdMd5) || passwdMd5.isEmpty());
    }
    // 生成密码的md5签名
    private String generatePasswdMd5(String passwd)
    {
        try
        {
            // 使用用户输入的密码+"20161116"，生成md5，再转成base64
            String passwd1 = passwd + "20161116";
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            return new String(Base64.encode(md5.digest(passwd1.getBytes()), Base64.DEFAULT)).trim();
        }
        catch (NoSuchAlgorithmException e)
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("缺少必要的md5算法库，搞不定了。");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return new String();
        }

    }

    // 加密,使用pwssword对plainText进行3DES加密，返回密文串
    private String personalEncrypt(String plainText, String password)
    {
        try
        {
            String actualPwd = cEncryptPwd(password);
            String cipherText = AndroidDes3Util.encode(plainText, actualPwd);
            return cipherText;
        }
        catch (Exception e)
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("缺少必要的加密算法库，搞不定了。");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return new String();
        }
    }

    // 解密，同上.
    private String personalDecrypt(String cipherText, String password)
    {
        try
        {
            String actualPwd = cEncryptPwd(password);
            String plainText = AndroidDes3Util.decode(cipherText, actualPwd);
            return plainText;
        }
        catch (Exception e)
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("缺少必要的加密算法库，搞不定了。");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return new String();
        }
    }

    // 根据当前是第几页，做获取对应的数据进行初始化.
    private void initialData()
    {
        memoText.setText("");
        passDecipher.setText("");
        tipText.setText("Page: " + String.valueOf(currPage));

        setInitialStatus();
        SharedPreferences setting = getPreferences(Activity.MODE_PRIVATE);
        String mbase64text = setting.getString(getSharedDataKey(), "");
        passwdMd5 = setting.getString(getSharedPwdKey(), "").trim();
        if(!mbase64text.isEmpty())
        {
            setWaitDesStatus();
            memoText.setText(mbase64text);
        }
    }

    private class OnLongClickPage implements View.OnLongClickListener
    {

        @Override
        public boolean onLongClick(View v)
        {
            // 检查是哪一个按钮触发的
            // 仅仅是当前currPage对应的按钮触发时才响应
            int whichPage = 0;
            int vhid = v.getId();
            switch (vhid)
            {
                case R.id.imageView1:
                    whichPage = 1;
                    break;
                case R.id.imageView2:
                    whichPage = 2;
                    break;
                case R.id.imageView3:
                    whichPage = 3;
                    break;
                case R.id.imageView4:
                    whichPage = 4;
                    break;
                default:
                    break;
            }
            if(whichPage != currPage)
                return false;

            // 创建PopupMenu对象
            final PopupMenu popup = new PopupMenu(MainActivity.this, v);
            // 通过代码添加菜单项
            // Menu menu = popup.getMenu();
            // menu.add(Menu.NONE, Menu.FIRST + 0, 0, "复制");
            // menu.add(Menu.NONE, Menu.FIRST + 1, 1, "粘贴");

            // 将R.menu.menuix 菜单资源加载到popup菜单中
            getMenuInflater().inflate(R.menu.menuix, popup.getMenu());

            // 为popup菜单的菜单项单击事件绑定事件监听器
            // 仍然是匿名对象，实现 OnMenuItemClickListener 这个接口
            popup.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener()
            {
                @Override
                public boolean onMenuItemClick(MenuItem item)
                {
                    switch (item.getItemId())
                    {
                        case R.id.exportmemo:
                            exportmemo();
                            popup.dismiss();
                            break;
                        case R.id.importmemo:
                            importmemo();
                            popup.dismiss();
                            break;
                        default:
                            break;
                        //使用Toast显示用户单击的菜单项
                        // Toast.makeText(PopupMenuTest.this, "您单击了【"+item.getTitle()+"】菜单项",Toast.LENGTH_SHORT).show();
                    }
                    return true;
                }

            });

            // 在 PopupMenu 中显示出图标
            // PopupMenu的样式文件在 res/menu/menuix.xml，虽然定义了icon，但默认仍不会显示，
            // 这里通过反射的机制让图标能显示出来：
            try
            {
                // 通过popup这个对象，反射出它的class，再获取内部成员 mPopup。mPopup为：private final MenuPopupHelper mPopup;
                Field field = popup.getClass().getDeclaredField("mPopup");
                // 设置该成员为可访问的
                field.setAccessible(true);
                // 获取popup这个对象的该成员。
                // 在调用这个方法之前，field仅仅表示 PopupMenu.mPopup这个成员；调用之后就表示具体的popup对象的mPopup成员了。
                Object menuPopupHelper = field.get(popup);
                // 这个比较绕:
                // menuPopupHelper.getClass(): 获得menuPopupHelper对象的Java.lang.Class，在这里为 class MenuPopupHelper
                // getName(): 以String形式返回此Class的名称，在这里为 "MenuPopupHelper"
                // Class.forName(): 定义该类的一个具体对象
                Class<?> classPopupHelper = Class.forName(menuPopupHelper.getClass().getName());
                // 反射出classPopupHelper的方法"setForceShowIcon", boolean.class表示需要反射的方法的参数类型.
                Method setForceIcons = classPopupHelper.getMethod("setForceShowIcon", boolean.class);
                // 最后，invoke 对象menuPopupHelper的setForceShowIcon方法，参数为true.
                // 其实这些代码所做的核心事件就是，调用： popup.mPopup.setForceShowIcon(true);
                setForceIcons.invoke(menuPopupHelper, true);
            } catch (Exception e)
            {
                e.printStackTrace();
            }

            popup.show();
            return true;
        }
    }

    private void exportmemo()
    {
        // AsyncHttpClient asyncHttpClient = new AsyncHttpClient();

        /*
        File[] files = new File("/").listFiles();
        for (File file : files)
        {
            String uri = file.getName();
            String pa = file.getPath();
            String x = uri + pa;
        }
        */
        String content = memoText.getText().toString();
        if (content == null || "".equals(content))
        {
            return;
        }

        // 用Intent分享文字内容
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("text/plain");
        // EXTRA_SUBJECT 貌似一般用在邮件中
        intent.putExtra(Intent.EXTRA_SUBJECT, "MySecureMemo数据" + String.valueOf(currPage));
        // 这里填充需要分享的文本内容
        intent.putExtra(Intent.EXTRA_TEXT, content);
        // 弹出候选的应用程序列表
        startActivity(Intent.createChooser(intent, "导出数据" + String.valueOf(currPage) + "到..."));
        // startActivity(intent);
    }


    private void importmemo()
    {

        // 这是临时申请权限.需参考Android6的新权限机制：
        /*
        * 对于6.0以下的权限及在安装的时候，根据权限声明产生一个权限列表，用户只有在同意之后才能完成app的安装，
        * 造成了我们想要使用某个app，就要默默忍受其一些不必要的权限（比如是个app都要访问通讯录、短信等）。
        * 而在6.0以后，我们可以直接安装，当app需要我们授予不恰当的权限的时候，我们可以予以拒绝（比如：单机的象棋对战，请求访问任何权限，我都是不同意的）。
        * 当然你也可以在设置界面对每个app的权限进行查看，以及对单个权限进行授权或者解除授权。
        * */
        if(ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        {
            // 请求读取文件的权限, 123无意义，仅用来传递给onRequestPermissionsResult，
            // 在用户点击拒绝或授权后，会触发 onRequestPermissionsResult。
            // 虽然可以在运行过程中申请权限，但测试发现仍然需要将该权限加在 AndroidManifest.xml 中
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 123);
            // 在这直接返回。如果用户确认了权限，则由onRequestPermissionsResult 函数再次调用 importmemo
            return;
        }

        Intent selectedFile = new Intent(Intent.ACTION_GET_CONTENT);
        selectedFile.setType("text/plain");
        // selectedFile.addCategory(Intent.CATEGORY_OPENABLE);// 用来指示一个GET_CONTENT意图只希望ContentResolver.openInputStream能够打开URI
        // 使用startActivityForResult，表示当用户从新的Activity完成选择后，能再回到当前Activity，是跳到onActivityResult处。
        startActivityForResult(selectedFile, 456); // 此处的456一定要>=0，用来传递给onActivityResult(requestCode)
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        // 是否选择了确认
        if (resultCode == Activity.RESULT_OK)
        {
            Uri uri = data.getData();//得到uri，后面就是将uri转化成file的过程。
            String fileName = uri.getPath();

            try
            {
                // FileInputStream fin = this.openFileInput(fileName);
                FileInputStream fin = new FileInputStream(fileName);
                int length = fin.available();
                byte[] buffer = new byte[length];
                fin.read(buffer);
                fin.close();

                if (memoText.getText().toString().isEmpty())
                {
                    memoText.setText(new String(buffer));
                    setWaitDesStatus();
                    AlertDialog.Builder AlertPlan = new AlertDialog.Builder(MainActivity.this);
                    AlertPlan.setTitle("已导入，请输入密码，解密后完成保存。");
                    AlertPlan.setPositiveButton(" 返回 ", null);
                    AlertPlan.show();
                }
            } catch (Exception e)
            {
                // e.printStackTrace();
                AlertDialog.Builder AlertPlan = new AlertDialog.Builder(MainActivity.this);
                AlertPlan.setTitle("导入文件失败：" + e.toString());
                AlertPlan.setPositiveButton(" 返回 ", null);
                AlertPlan.show();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        if(requestCode != 123)
            return;

        for(int res : grantResults)
        {
            if(getPackageManager().PERMISSION_GRANTED == res)
            {
                importmemo();
            }
            else
            {
                // Do nothing.
            }
        }
    }

    // 初始状态，无数据.
    private void setInitialStatus()
    {
        memoText.setEnabled(true);
        buttonDecipher.setEnabled(false);
        passDecipher.setEnabled(true);
        buttonSave.setEnabled(true);
    }

    // 设置相关控件为等待解密的状态.
    private void setWaitDesStatus()
    {
        passDecipher.setText(""); // 清空密码

        memoText.setEnabled(false);
        buttonDecipher.setEnabled(true);
        passDecipher.setEnabled(true);
        buttonSave.setEnabled(false);
    }

    private void setViewStatus()
    {
        memoText.setEnabled(true);
        buttonDecipher.setEnabled(false);
        passDecipher.setEnabled(true);
        buttonSave.setEnabled(true);

        // 取消密码框的焦点
        passDecipher.clearFocus();
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(passDecipher.getWindowToken(), 0); //强制隐藏键盘
        //buttonSave.setFocusable(true);
        //buttonSave.setFocusableInTouchMode(true);
        //buttonSave.requestFocus();
    }

    private String getSharedDataKey()
    {
        return "memTextBase64_" + String.valueOf(currPage);
    }
    private String getSharedPwdKey()
    {
        return "passwdMd5_" + String.valueOf(currPage);
    }
    /*
    private void setWaitSaveStatus()
    {
        memoText.setEnabled(true);
        buttonDecipher.setEnabled(false);
        passDecipher.setEnabled(true);
        buttonSave.setEnabled(true);
    }
    */
}


