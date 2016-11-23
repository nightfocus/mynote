package cn.kisskyu.mysecurememo;

import java.security.Key;

import javax.crypto.Cipher;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESedeKeySpec;
import javax.crypto.spec.IvParameterSpec;
import android.util.Base64;

/**
 * Android 3DES加密工具类
 */
public class AndroidDes3Util {

    // 向量 可有可无 终端后台也要约定
    private final static String iv = "01234567";
    // 加解密统一使用的编码方式
    private final static String encoding = "utf-8";

    /**
     * 3DES加密
     *
     * @param plainText
     *            普通文本
     * @return
     * @throws Exception
     */
    public static String encode(String plainText, String secretKey) throws Exception
    {
        Key deskey = null;
        DESedeKeySpec spec = new DESedeKeySpec(secretKey.getBytes());
        SecretKeyFactory keyfactory = SecretKeyFactory.getInstance("desede");
        deskey = keyfactory.generateSecret(spec);

        Cipher cipher = Cipher.getInstance("desede/CBC/PKCS5Padding"); // [cipher]/[mode]/[padding]
        IvParameterSpec ips = new IvParameterSpec( iv.getBytes());
        cipher.init(Cipher.ENCRYPT_MODE, deskey, ips);
        byte [] encryptData = cipher.doFinal(plainText.getBytes(encoding ));
        return Base64.encodeToString(encryptData,Base64. DEFAULT );
    }

    /**
     * 3DES解密
     *
     * @param encryptText
     *            加密文本
     * @return
     * @throws Exception
     */
    public static String decode(String encryptText, String secretKey) throws Exception
    {
        Key deskey = null ;
        DESedeKeySpec spec = new DESedeKeySpec( secretKey.getBytes());
        SecretKeyFactory keyfactory = SecretKeyFactory.getInstance("desede");
        deskey = keyfactory.generateSecret(spec);
        Cipher cipher = Cipher.getInstance("desede/CBC/PKCS5Padding");  // [cipher]/[mode]/[padding]
        IvParameterSpec ips = new IvParameterSpec(iv.getBytes());
        cipher. init(Cipher.DECRYPT_MODE, deskey, ips);

        byte [] decryptData = cipher.doFinal(Base64.decode(encryptText, Base64. DEFAULT));

        return new String(decryptData, encoding);
    }
}
