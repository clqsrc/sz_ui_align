package jsp;



import java.util.UUID;
import java.io.*;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.text.SimpleDateFormat;




//来自 jsp 注意保持一致
public class functions {
    


    //final javax.servlet.jsp.JspWriter _out = out;
    
    //生成一个 guid 字符串
    static public String CreateGuid()
    {
        UUID uuid = UUID.randomUUID(); 
        //System.out.println (uuid);

        //if (null == _v) _v = "";
        //return _v;

        return uuid.toString();
    }//


  
    //复制文件 //文件已存在的话，要先删除
    //jetty 下似乎有问题
    public void CopyFile_v1(String fn, String fn_new)
    //throws IOException 
    throws Exception 
    {   
    
        //Files.copy(source.toPath(), dest.toPath()); 
        //要确认拷贝的路径存在
//        File destDir = new File("E:/xx");
//        if(!(destDir.exists()&& destDir.isDirectory())) {
//            destDir.mkdirs();
//        }

        //File source = new File("D:/a.txt");
        //File dest = new File("E:/xx/b.txt");
        File source = new File(fn);
        File dest = new File(fn_new);
        try{
            Files.copy(source.toPath(), dest.toPath());
            //Files.copy(fn, fn_new);
        } catch (IOException e){
            // TODO Auto-generated catch block
             e.printStackTrace();

             throw(e);
        }

    
    }//

    public void CopyFile(String fn, String fn_new)
    //throws IOException 
    throws Exception 
    {   
    
        //Files.copy(source.toPath(), dest.toPath()); 
        //要确认拷贝的路径存在
//        File destDir = new File("E:/xx");
//        if(!(destDir.exists()&& destDir.isDirectory())) {
//            destDir.mkdirs();
//        }

        //File source = new File("D:/a.txt");
        //File dest = new File("E:/xx/b.txt");
        File source = new File(fn);
        File dest = new File(fn_new);
//        try{
//            //Files.copy(source.toPath(), dest.toPath());
//            //Files.copy(fn, fn_new);
//
//
//
//        } catch (IOException e){
//            // TODO Auto-generated catch block
//             e.printStackTrace();
//
//             throw(e);
//        }

        //--------------------------------------------------------
        //private static void copyFileUsingStream(File source, File dest) throws IOException {

        InputStream is = null;
        OutputStream os = null;
        try {

            is = new FileInputStream(source);
            os = new FileOutputStream(dest);

            byte[] buffer = new byte[4 * 1024];

            int length;

            while ((length = is.read(buffer)) > 0) {

                os.write(buffer, 0, length);

            }
        } catch (IOException e){
            // TODO Auto-generated catch block
             e.printStackTrace();

             throw(e);

        } finally {
            is.close();
            os.close();

        }//

    
    }//


    //字符串存入文件 //utf8
    public void SaveStringToFile(String filePath, String s) throws Exception 
    {
        try {
            FileOutputStream fos = new FileOutputStream(filePath);
            
            //fos.write(s.getBytes());
            fos.write(s.getBytes("UTF-8"));

            fos.close();

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();

            throw(e);
        }

    }//
        
    
    //创建目录，单级目录
    public void mkdir(String path)
    {
        //File destDir = new File("E:/xx");
        File destDir = new File(path);
        if(!(destDir.exists()&& destDir.isDirectory())) {
            destDir.mkdirs();
        }
    }//
        
    //删除文件
    public void deletefile(String fn)
    {
        //File file = new File("test.txt");
        File file = new File(fn);

        if(file.exists()){
                file.delete();
                System.out.println("文件已删除。");
        }else{
                System.out.println("文件不存在！");
        }


    }//
    

    //可作文件名的时间 //到秒,还是到毫秒吧
    public String FormatDatetime_forFileName()
    {
        //import java.util.Date;
        //import java.text.SimpleDateFormat;

        //SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
        //SimpleDateFormat sdf = new SimpleDateFormat("yyyy MMM dd  HH:mm:ss.SSS",Locale.ENGLISH);
        SimpleDateFormat df = new SimpleDateFormat("yyyyMMdd_HHmmss_SSS");//设置日期格式
        String s = df.format(new Date());// new Date()为获取当前系统时间

        return s;
    }//

    //去除路径，只要文件名
    public String ExtractFileName(String fullpath)
    {
        //import java.io.File;  

        File tempFile =new File(fullpath.trim());  
        String fileName = tempFile.getName();  

        return fileName; 

    }//

    //字符串存入文件 //utf8
    public byte[] LoadBytesFromFile(String filePath) throws Exception 
    {
        try {
            FileInputStream fis = new FileInputStream(filePath);
            
            //byte[] b=new byte[(int) file.length()];
            byte[] buf = new byte[4 * 1024 * 1024]; //最多 4m

            int len = fis.read(buf);

            if (len<1)return null;

            //byte[] rbuf = new byte[Integer.min(len, buf.length)]; //最多 4m

            //String s = new String(buf, 0, len, "UTF-8");

            byte[] rbuf = Arrays.copyOfRange(buf, 0, len); //要再截取一下，否则是不对的

            fis.close();

            //return buf;
            return rbuf;

        } catch (Exception e) {
            
            e.printStackTrace();

            throw(e);
        }

        //return ""; //java 下不会过这里.其他语言的话，还是要加这个的。

    }//

    public String bytes2Hex(byte[] buf) 
    {

        StringBuilder sb = new StringBuilder();
        try {
 
            for (byte b : buf) {  
                sb.append(String.format("%02X", b)); // 10进制转16进制，X 表示以十六进制形式输出，02 表示不足两位前面补0输出  
            }  
            return sb.toString();
         } catch (Exception e) {  
            e.printStackTrace();

            //throw
        }//

        return "";
    }//

    //计算一个文件的 sha256 //SHA256Bin
    public String SHA256File(String fn) 
    {

        //import java.security.MessageDigest;  
        //import java.security.NoSuchAlgorithmException; 
        
        MessageDigest md = null;
        String strDes = ""; //null;
        String encName="SHA-256";
        try {

            byte[] buf = LoadBytesFromFile(fn);
            md = MessageDigest.getInstance(encName);
            md.update(buf);
            strDes = bytes2Hex(md.digest()); // to HexString
        //} catch (NoSuchAlgorithmException e) {
        } catch (Exception e) {
            return null;
        }
        return strDes;
    }//

    //2022
    //超时值的单位是毫秒
    public static String HttpGet(String url, int timeout_millisecond)
    {
        try{
        //http_get(url);
        //--------------------------------------------------------
        //String addParam=java.net.URLEncoder.encode("username", "UTF-8")
        
        java.net.URL http = new java.net.URL(url);

        java.net.HttpURLConnection connect = (java.net.HttpURLConnection) http.openConnection();
        //connect.setDoOutput(true);
        //setDoOutput()默认是false，需要手动设置为true，完了就可以调用getOutputStream()方法从服务器端获得字节输出流。
        connect.setRequestMethod("GET");
        
        //超时值的单位是毫秒
        //int timeout_millisecond = 60 * 1000;
        
        connect.setConnectTimeout(timeout_millisecond);//connectTimeout);
        connect.setReadTimeout(timeout_millisecond);//readTimeout);
        
        String res = connect.getResponseMessage();  //这个其实不是结果，它的值一般是 "OK"
        
        //out.clear();    //清空一下比较好
        //out.println(res);
        
        //--------------------------------------------------------
        // 获取请求返回的数据流
        InputStream is = connect.getInputStream();
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        // 封装输入流is，并指定字符集
        int i;
        while ((i = is.read()) != -1) {
            baos.write(i);
        }
        //out.println(baos.toString());
        
        return baos.toString();
        //--------------------------------------------------------
        } catch (Exception e) {
            return "";
        }

    }//

    //获取e.printStackTrace() 的具体信息，赋值给String 变量，并返回
    public static String GetStackTraceInfo(Exception e) 
    {

        StringWriter sw = null;
        PrintWriter pw = null;

        try {
            sw = new StringWriter();
            pw = new PrintWriter(sw);
            //将出错的栈信息输出到printWriter中
            e.printStackTrace(pw);
            pw.flush();
            sw.flush();

            return sw.toString();
        } catch (Exception ex) {

            return "发生错误";
        } finally {
            if (sw != null) {
                try {
                    sw.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
            if (pw != null) {
                pw.close();
            }
        }
    }//

}//class


