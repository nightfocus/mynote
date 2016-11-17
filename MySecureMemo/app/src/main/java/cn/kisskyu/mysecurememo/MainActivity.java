package cn.kisskyu.mysecurememo;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.util.Base64;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.text.TextWatcher;
import android.widget.TextView;

import java.io.UnsupportedEncodingException;
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

    TextWatcher myTextWatcher = new TextWatcher()
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
    }

    public void OnPage1(View v)
    {
        if(currPage == 1)
            return;
        currPage = 1;
        initialData();
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
        return (newPasswdMd5.equals(passwdMd5));
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


