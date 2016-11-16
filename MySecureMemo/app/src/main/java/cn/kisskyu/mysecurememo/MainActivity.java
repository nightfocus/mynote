package cn.kisskyu.mysecurememo;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.util.Base64;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.text.TextWatcher;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;


public class MainActivity extends AppCompatActivity
{
    EditText memoText, passDecipher;
    Button buttonDecipher, buttonSave;
    String passwdMd5; // 保存好的密码签名，用于验证密码是否正确

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

    /*
    *  在这个目录下：
    *  D:\github.private\mynote\MySecureMemo\app\src\main
    *  执行：
    *  javah -d jni -classpath D:\android-sdk\platforms\android-23\android.jar;D:\android-sdk\extras\android\support\v7\appcompat\libs\android-support-v7-appcompat.jar;D:\android-sdk\extras\android\support\v4\android-support-v4.jar;D:\github.private\mynote\MySecureMemo\app\build\intermediates\classes\debug cn.kisskyu.mysecurememo.MainActivity
    *  会生成jni目录和.h文件.
    * */
    // public synchronized static native String cEncrypt(String plainText, String pwd);
    // public synchronized static native String cDecrypt(String cipherText, String pwd);

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        memoText = (EditText)findViewById(R.id.memoText);
        passDecipher = (EditText)findViewById(R.id.passDecipher);
        buttonDecipher = (Button)findViewById(R.id.buttonDecipher);
        buttonSave = (Button)findViewById(R.id.buttonSave);

        initialData();
        // 注册文本框的事件
        // memoText.addTextChangedListener(myTextWatcher);

        registerReceiver(myReceiver, new IntentFilter(Intent.ACTION_SCREEN_OFF));
    }

    public void OnDecipher(View v)
    {
        String cipherText = memoText.getText().toString();
        String password = passDecipher.getText().toString();
        if(cipherText.isEmpty() || password.isEmpty() || !checkPasswd(password))
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("没有读取到数据，或者输入的密码不对.");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return;
        }

        String plainMemo = personalDecrypt(cipherText, password);
        memoText.setText(plainMemo);
        memoText.setSelection(plainMemo.length());

        setViewStatus();
    }

    public void OnMemoSave(View v)
    {
        String plainMemo = memoText.getText().toString();
        String password = passDecipher.getText().toString();
        if(plainMemo.isEmpty() || password.isEmpty())
        {
            AlertDialog.Builder AlertPlan  = new AlertDialog.Builder(MainActivity.this);
            AlertPlan.setTitle("没有需要保存的数据，或者没有设定密码.");
            AlertPlan.setPositiveButton(" 返回 ", null);
            AlertPlan.show();
            return;
        }

        passwdMd5 = generatePasswdMd5(password);
        String cipherText = personalEncrypt(plainMemo, password);

        SharedPreferences setting = getPreferences(Activity.MODE_PRIVATE);
        SharedPreferences.Editor editor = setting.edit();
        editor.putString("memTextBase64", cipherText);
        editor.putString("passwdMd5", passwdMd5);
        editor.commit();

        String mbase64text = setting.getString("memTextBase64", "");
        setWaitDesStatus();
        memoText.setText(mbase64text);
        memoText.setSelection(mbase64text.length());
    }

    // 密码校验
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

    // 加密
    private String personalEncrypt(String plainText, String password)
    {
        String strBase64 = new String(Base64.encode(plainText.getBytes(), Base64.DEFAULT));
        return strBase64;
    }

    // 解密
    private String personalDecrypt(String cipherText, String password)
    {
        String plainText = new String(Base64.decode(cipherText.getBytes(), Base64.DEFAULT));
        return plainText;
    }

    private void initialData()
    {
        setInitialStatus();
        SharedPreferences setting = getPreferences(Activity.MODE_PRIVATE);
        String mbase64text = setting.getString("memTextBase64", "");
        passwdMd5 = setting.getString("passwdMd5", "").trim();
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


