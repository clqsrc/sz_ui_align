//package com.xmppmini;
package hpublic;

import java.util.Arrays;

//专门操作 []bytes 的
public class functions_bytes {
    //https://blog.csdn.net/wolfking0608/article/details/71305360
    //https://blog.csdn.net/along_000/article/details/106061218
    public static byte[] AddBytes(byte[] data1, byte[] data2) {
        byte[] data3 = new byte[data1.length + data2.length];
        System.arraycopy(data1, 0, data3, 0, data1.length);
        System.arraycopy(data2, 0, data3, data1.length, data2.length);
        return data3;

    }//

    //合并多个byte[]数组为一个byte[]数组
    public static byte[] byteConcat(byte[] bt1, byte[] bt2, byte[] bt3){
        byte[] bt4 = new byte[bt1.length+bt2.length+bt3.length];
        int len = 0;
        System.arraycopy(bt1, 0, bt4, 0, bt1.length);
        len += bt1.length;
        System.arraycopy(bt2, 0, bt4, len, bt2.length);
        len += bt2.length;
        System.arraycopy(bt3, 0, bt4, len, bt3.length);
        return bt4;
    }//

    //分割成两个数组
    public static byte[][] SplitBytes(byte[] buf, int count) {

        int size1 = count;
        int size2 = buf.length - size1;

        //声明一一个二维数组，用于存储3个一维数组，每一个一维数据存多少个数组，不知道 ，null
        //int [][]intB=new int[3][];
        //intB[0]=new int[3];  //3
        //intB[1]=new int[]{1,2,3,4};  //3
        //intB[2]=new int[2];  //2

        byte[][] result = new byte[2][];

        //Arrays.copyOfRange() 将一个原始的数组original，从下标from开始复制，复制到上标to，生成一个新的数组。
        //注意这里包括下标from，不包括上标to。
        //result[i] = Arrays.copyOfRange(bytes, from, to);
        result[0] = Arrays.copyOfRange(buf, 0, count);
        result[1] = Arrays.copyOfRange(buf, count, buf.length);

        //Arrays.binarySearch()

        return result;
    }//

    //查找一个字符的位置//从后搜索//没找到就是 -1
    public static int FindChar_last(byte[] buf, char c) {

        byte b = (byte)c;

        //int r = Arrays.binarySearch(buf, b); //不行的，据说是二分法
        int r = -1;

        for (int i=buf.length-1; i>=0; i--) //从最后一个开始
        {
            if (buf[i] == b) { r = i; break;}
        }

        return r;
    }//

    //从 bytes 中安全的分离出一个字符串，因为 utf8 这样的会被意外切割 //分隔标志是一个结束符号，并且是最后一个。比如 xml 中的 ">"
    //不行，有 utf8 时是对不上的
    /*
    public static String SafeToString(byte[] buf, char end_char) {

        int pos = FindChar_last(buf, end_char);
        if (pos<0) return "";


    }//
    */
    //似乎只能整体转换成 单字节的字符串
    public static String SafeToString(byte[] buf) {

        try{
          return new String(buf, "iso8859-1"); //据说异常的字符也能保存在 "iso8859-1" 字符集中不会丢失，以后再测试半个 utf8 的情况了

        }//try
        catch (Exception e)
        {
            e.printStackTrace();
        }//
        return "";
    }//

    //参考 delphi/lazarus 可以虚拟定义一个 RawByteString = type AnsiString(CP_NONE);
    public static String ToRawByteString(byte[] buf) {

        return SafeToString(buf);
    }//

    //xmpp 的一切搜索都使用单字节字符串操作，得到结果后再转换成 utf8
    public static String BytesstringToUtf8(String s)
    {
        try{
            byte [] buf = s.getBytes("iso8859-1"); //还原为原始字节流

            return new String(buf, "UTF-8"); //java.nio.charset.StandardCharsets.UTF_8

            //java.nio.charset.StandardCharsets.UTF_8 要更高的 安卓 sdk 版本

        }//try
        catch (Exception e) //catch (IOException e) //断开的异常抓取不完，还是要判断一下场景
        {
            e.printStackTrace();
        }//

        return "";
    }//

    //从原数组中取多少个元素
    public static byte[] CopyBytes(byte[] bytes, int count) {

        //Arrays.copyOfRange() 将一个原始的数组original，从下标from开始复制，复制到上标to，生成一个新的数组。
        //注意这里包括下标from，不包括上标to。
        byte [] result = Arrays.copyOfRange(bytes, 0, count);


        return result;
    }//

    //将int数值转换为占四个字节的byte数组1

    /**
     * 将int数值转换为占四个字节的byte数组，本方法适用于（低位在前，高位在后）的顺序,和bytesToInt()配套使用
     * @param value
     * @param value：要转换的int值
     * @return byte数组
     */
    public static byte[] intToBytes( int value )
    {
        byte[] src = new byte[4];
        src[3] =  (byte) ((value>>24) & 0xFF);
        src[2] =  (byte) ((value>>16) & 0xFF);
        src[1] =  (byte) ((value>>8)  & 0xFF);
        src[0] =  (byte) (value & 0xFF);
        return src;
    }//

    //将int数值转换为占四个字节的byte数组2
    /**
     * 将int数值转换为占四个字节的byte数组，本方法适用于（低位在后,高位在前）的顺序,和bytesToInt2()配套使用
     *  @param value：要转换的int值
     * @return byte数组
     */
    public static byte[] intToBytes2(int value)
    {
        byte[] src = new byte[4];
        src[0] = (byte) ((value>>24) & 0xFF);
        src[1] = (byte) ((value>>16) & 0xFF);
        src[2] = (byte) ((value>>8)  & 0xFF);
        src[3] = (byte) (value & 0xFF);
        return src;
    }

    //从byte数组中取int数值1

    /**
     * 从byte数组中取int数值，本方法适用于(低位在前，高位在后)的顺序，和和intToBytes()配套使用
     *
     * @param src: byte数组
     * @param offset: 从数组的第offset位开始
     * @return int数值
     */
    public static int bytesToInt(byte[] src, int offset) {
        int value;
        value = (int) ((src[offset] & 0xFF)
                | ((src[offset+1] & 0xFF)<<8)
                | ((src[offset+2] & 0xFF)<<16)
                | ((src[offset+3] & 0xFF)<<24));
        return value;
    }//

    //从byte数组中取int数值2

    /**
     * 从byte数组中取int数值，本方法适用于(低位在后，高位在前)的顺序。和intToBytes2（）配套使用
     */
    public static int bytesToInt2(byte[] src, int offset) {
        int value;
        value = (int) ( ((src[offset] & 0xFF)<<24)
                |((src[offset+1] & 0xFF)<<16)
                |((src[offset+2] & 0xFF)<<8)
                |(src[offset+3]  & 0xFF));
        return value;
    }

}//
