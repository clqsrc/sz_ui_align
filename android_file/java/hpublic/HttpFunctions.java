package hpublic;

import android.content.Context;
//import android.support.v7.app.AlertDialog;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.nio.charset.Charset;
import java.util.Map;
import java.lang.reflect.Method;
import java.util.Hashtable; //线程安全

//参考：https://www.jianshu.com/p/8b5c6bc686fd


//import net.newbt.Functions;

//import java.lang.reflect.Method;
//
//import cz.msebera.android.httpclient.Header;
//
//import static net.newbt.Functions.*;
//import static net.newbt.xmppminiapplication.App.*;


//class  MyHttpHandler extends AsyncHttpResponseHandler {
//
//    public Context _this;
//    public String funcName;
//
//    //请求成功时回调，相应码为200
//    @Override
//    public void onSuccess(int statusCode, Header[] headers,
//                          byte[] responseBody) {
//        ////Toast.makeText(MainActivity.this, new String(responseBody), 0).show();
//        //Functions.ShowMessage("请求成功" + new String(responseBody), MainActivity.this);
//        ////Functions.ShowMessage("请求成功" + new String(responseBody), _this);
//
//        //AppFunctions.invokePrivateMothod(funcName, new String(responseBody), 0,  _this.getClass(), _this);
//        //AppFunctions.invokePrivateMothod_2(funcName, new String(responseBody), 0,  _this.getClass(), _this);
//        AppFunctions.invokePrivateMothod_2_s_i(funcName, new String(responseBody), 0,  _this.getClass(), _this);
//    }
//
//    //请求失败时回调，相应码不为200
//    @Override
//    public void onFailure(int statusCode, Header[] headers,
//                          byte[] responseBody, Throwable error) {
//        ////Toast.makeText(MainActivity.this, "请求失败", 0).show();
//        //异常//Functions.ShowMessage("请求失败" + new String(responseBody), MainActivity.this);
//        ////Functions.ShowMessage("请求失败" + error.getMessage(), MainActivity.this);
//        //Functions.ShowMessage("请求失败" + error.getMessage(), _this);
//
//        //AppFunctions.invokePrivateMothod(funcName, new String(responseBody), _this.getClass(), _this);
//        //AppFunctions.invokePrivateMothod_2(funcName,error.getMessage(), 1,  _this.getClass(), _this);
//        HttpFunctions.invokePrivateMothod_2_s_i(funcName,error.getMessage(), 1,  _this.getClass(), _this);
//    }
//
//    @Override
//    public void onRetry(int retryNo) {}
//
//    @Override
//    public void onStart() {}
//
//}//

//来自 sister_xmpp_an\app\src\main\java\net\newbt\xmppminiapplication
//public class AppFunctions
public class HttpFunctions
{


    //如今的 http 很混乱，所以写一个公用的好了

    //----------------------------------
    //因为没有创建实例，所以其实是单实例的，所以只能在 ui 主线程中调用这些函数。所以发送的 post 参数也得是单实例的

    public static Hashtable<String, String> postParam = new Hashtable<String, String>();  //线程安全

    //清空参数
    public static void Http_ParamClear()
    {
        try {

            postParam.clear();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }//

    public static void Http_SetParam(String key, String value)
    {
        try {

            postParam.put(key, value);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }//

    //----------------------------------
    //现在的 http 很啰嗦
    public static int DEF_CONN_TIMEOUT = 15 * 1000; //默认的超时值，毫秒
    public static Charset DEF_CHATSET = Charset.defaultCharset(); //默认的字符集 //java.nio.charset.StandardCharsets.UTF_8
    public static String HttpGet(String _url, Context _this, String funcName) //get 还是不行,太复杂的 sql 过不了
    {
        HttpURLConnection conn = null;
        BufferedReader reader = null;
        String rs = null;
        try {
            StringBuffer sb = new StringBuffer();
            //strUrl = strUrl + "?" + urlencode(params);
            URL url = new URL(_url); //new URL(strUrl);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            //conn.setRequestProperty("User-agent", userAgent);  //应该不用
            conn.setUseCaches(false);
            conn.setConnectTimeout(DEF_CONN_TIMEOUT);  //Sets a specified timeout value, in milliseconds //从源码看这是毫秒
            conn.setInstanceFollowRedirects(false);
            conn.connect();
            InputStream is = conn.getInputStream();
            //reader = new BufferedReader(new InputStreamReader(is, DEF_CHATSET));
            reader = new BufferedReader(new InputStreamReader(is, DEF_CHATSET));
            String strRead = null;
            while ((strRead = reader.readLine()) != null) {
                sb.append(strRead);
            }
            rs = sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {

                try{ reader.close(); }catch(Exception e2) {}

            }
            if (conn != null) {
                conn.disconnect();
            }
        }
        return rs;


    }//


    static public String GetPath_write(String fn)
    {
        //这是内部路径
        String path = Functions.Functions_cur_this.getFilesDir().getAbsolutePath();

        return path + "/" + fn;
    }

    public static int HttpGetToFile(String _url, String fn)
    {
        HttpURLConnection conn = null;
        BufferedReader reader = null;
        String rs = null;

        File file = null;
        FileOutputStream fileos = null;

        int r = 0; //是否成功
        try {

            //--------
            //可写的目录路径
            //参考 https://www.cnblogs.com/2010wuhao/p/4681899.html
            //这是内部路径
            //String path = Functions.Functions_cur_this.getFilesDir().getAbsolutePath();

            //String fn_full = path + "/" + fn;  //全路径
            String fn_full = fn;  //全路径

            //File file = new File(context.getFilesDir(), filename);
            file = new File(fn_full);

            fileos = new FileOutputStream(file);

            //--------

            StringBuffer sb = new StringBuffer();
            //strUrl = strUrl + "?" + urlencode(params);
            URL url = new URL(_url); //new URL(strUrl);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            //conn.setRequestProperty("User-agent", userAgent);  //应该不用
            conn.setUseCaches(false);
            conn.setConnectTimeout(DEF_CONN_TIMEOUT);  //Sets a specified timeout value, in milliseconds //从源码看这是毫秒
            conn.setInstanceFollowRedirects(false);
            conn.connect();
            InputStream is = conn.getInputStream();

            //reader = new BufferedReader(new InputStreamReader(is, DEF_CHATSET));
            //String strRead = null;

            //while ((strRead = reader.readLine()) != null) {
            //    sb.append(strRead);
            //}//while

            byte [] buf = new byte[4*1024];  //4k 缓冲区
            int read = 0;

            while (true) {

                read = is.read(buf);

                if (read < 1) break;

                fileos.write(buf, 0, read);  //读取多少写入多少

            }//while

            //rs = sb.toString();

            r = 1;
        } catch (Exception e) {
            e.printStackTrace();
            r = 0;
        } finally {
            if (reader != null) {

                try{ reader.close(); }catch(Exception e2) {}
                try{ fileos.close(); }catch(Exception e3) {}

            }

            if (conn != null) {
                conn.disconnect();
            }

        }//try
        return r;


    }//

    public static String HttpGet(String _url)
    {
        HttpURLConnection conn = null;
        BufferedReader reader = null;
        String rs = "";  //null;

        //File file = null;
        //FileOutputStream fileos = null;

        int r = 0; //是否成功
        //String r = "";
        try {


            //--------

            StringBuffer sb = new StringBuffer();
            //strUrl = strUrl + "?" + urlencode(params);
            URL url = new URL(_url); //new URL(strUrl);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            //conn.setRequestProperty("User-agent", userAgent);  //应该不用
            conn.setUseCaches(false);
            conn.setConnectTimeout(DEF_CONN_TIMEOUT);  //Sets a specified timeout value, in milliseconds //从源码看这是毫秒
            conn.setInstanceFollowRedirects(false);
            conn.connect();
            InputStream is = conn.getInputStream();

            reader = new BufferedReader(new InputStreamReader(is, DEF_CHATSET));
            String strRead = null;

            while ((strRead = reader.readLine()) != null) {
                sb.append(strRead);
            }//while


            rs = sb.toString();

            r = 1;
        } catch (Exception e) {
            e.printStackTrace();
            r = 0;
        } finally {
            if (reader != null) {

                try{ reader.close(); }catch(Exception e2) {}
                //try{ fileos.close(); }catch(Exception e3) {}

            }

            if (conn != null) {
                conn.disconnect();
            }

        }//try

        return rs;
        //return r;


    }//

    //将map型转为请求参数型 //这个函数的实现其实有点问题，因为不是文件式的 mime 。不过以后再改进了。
    private static String urlencode(Map<String, Object> params) {

        StringBuilder sb = new StringBuilder();
        for (Map.Entry i : params.entrySet()) {
            try {
                sb.append(i.getKey()).append("=")
                        .append(URLEncoder.encode(i.getValue() + "", "UTF-8"))
                        .append("&");
            } catch (Exception e) {
                // TODO: handle exception
                e.printStackTrace();
            }
        }
        return sb.toString();
    }//

    //现在的 http 很啰嗦
    public static String HttpPost(String _url,  Map params){ //get 还是不行,太复杂的 sql 过不了

        HttpURLConnection conn = null;
        BufferedReader reader = null;
        String rs = null;
        try {
            StringBuffer sb = new StringBuffer();
            URL url = new URL(_url);  //URL(strUrl);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setDoInput(true);
            //conn.setRequestProperty("User-agent", userAgent);
            conn.setUseCaches(false);
            conn.setConnectTimeout(DEF_CONN_TIMEOUT);
            conn.setInstanceFollowRedirects(false);
            conn.connect();
            DataOutputStream out = new DataOutputStream(conn.getOutputStream());
            out.writeBytes(urlencode(params));
            InputStream is = conn.getInputStream();
            reader = new BufferedReader(new InputStreamReader(is, DEF_CHATSET));
            String strRead = null;
            while ((strRead = reader.readLine()) != null) {
                sb.append(strRead);
            }
            rs = sb.toString();
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        } finally {
            if (reader != null) {
                //reader.close();

                try{ reader.close(); }catch(Exception e2) {}
            }
            if (conn != null) {
                conn.disconnect();
            }
        }
        return rs;


    }//


    //异步事件版本
    //c_func  C 语言的函数
    //c_param C 语言函数的参数
    //public static void HttpPost_UI(String _url,  Map params, long c_func,  long c_param) //get 还是不行,太复杂的 sql 过不了
    public static void HttpPost_UI(String _url,  long c_func,  long c_param) //get 还是不行,太复杂的 sql 过不了
    {
        Map params = postParam;  //参数单实例比较简单点

        try {

            Functions.go(new Runnable() {
                @Override
                public void run() {

                    String _rs = "";  //HttpPost(_url, params);

                    if (params.size() > 0) {
                        _rs = HttpPost(_url, params);
                    }else{
                        _rs = HttpGet(_url);
                    }

                    String rs = _rs;

                    Functions.go_main(Functions.Functions_cur_this, new Runnable() {
                        @Override
                        public void run() {

                            //这里不能直接用 this 传给 c 接口，而是应该用转换过的全局引用指针,因为这样传过去的就是临时指针了
                            if (null != rs)
                                NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, rs, "ok");
                            else
                                NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, "http post error", "error");


                            //清空 post/get 参数
                            Http_ParamClear();

                        }

                    });//go main


                }
            });//go


        } catch (Exception e) {

        } finally {

        }//try



    }//


    //异步事件版本 //下载一张图片到指定路径
    //c_func  C 语言的函数
    //c_param C 语言函数的参数
    //fn 是相对路径
    public static void HttpGetToFile_UI(String _url, String fn,  long c_func,  long c_param) //get 还是不行,太复杂的 sql 过不了
    {
        Map params = postParam;  //参数单实例比较简单点

        try {

            Functions.go(new Runnable() {
                @Override
                public void run() {

                    String full_fn = GetPath_write(fn); //取全路径

                    //int r = HttpGetToFile(_url, fn);
                    int r = HttpGetToFile(_url, full_fn);

                    Functions.go_main(Functions.Functions_cur_this, new Runnable() {
                        @Override
                        public void run() {

                            //这里不能直接用 this 传给 c 接口，而是应该用转换过的全局引用指针,因为这样传过去的就是临时指针了
                            if (0 != r) {
                                //NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, "http get success", "ok");
                                //NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, fn, "ok");
                                NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, full_fn, "ok");
                            }else
                                NDK_c_interface.RunFunc_Event_1o2s(c_func, c_param, "http get error", "error");


                            //清空 post/get 参数
                            Http_ParamClear();

                        }

                    });//go main


                }
            });//go


        } catch (Exception e) {

        } finally {

        }//try



    }//

    //--------------------
    //获取并调用私有方法
    public static void invokePrivateMothod(String funcName, String param1, Class<?> cls, Context _this) {
        try {
            // 获取方法名为showName，參数为String类型的方法
            //Method method = cls.getDeclaredMethod("showName", String.class);
            Method method = _this.getClass().getDeclaredMethod(funcName, String.class);
            // 若调用私有方法。必须抑制java对权限的检查
            method.setAccessible(true);
            // 使用invoke调用方法。而且获取方法的返回值。须要传入一个方法所在类的对象，new Object[]
            // {"Kai"}是须要传入的參数，与上面的String.class相相应
            //String string = (String) method.invoke(person, new Object[] { "Kai" } );
            method.invoke(_this, new Object[] { param1 } );

            //System.out.println(string);

//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        } catch (SecurityException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (IllegalArgumentException e) {
//            e.printStackTrace();
            //} catch (InvocationTargetException e) {
        } catch (Exception e) {
            //e.printStackTrace();
            Functions.ShowMessage(e.getMessage(), _this);
        }
    }//

    //两个参数的//注意参数1是字段串，参数2是整数
    public static void invokePrivateMothod_2_s_i(String funcName, String param1, int param2, Class<?> cls, Context _this) {
        try {
            // 获取方法名为showName，參数为String类型的方法
            //Method method = cls.getDeclaredMethod("showName", String.class);
            //Method method = _this.getClass().getDeclaredMethod(funcName, String.class);
            Method method = _this.getClass().getDeclaredMethod(funcName, String.class, int.class);
            // 若调用私有方法。必须抑制java对权限的检查
            method.setAccessible(true);
            // 使用invoke调用方法。而且获取方法的返回值。须要传入一个方法所在类的对象，new Object[]
            // {"Kai"}是须要传入的參数，与上面的String.class相相应
            //String string = (String) method.invoke(person, new Object[] { "Kai" } );
            method.invoke(_this, new Object[] { param1, param2 } );

            //System.out.println(string);

//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        } catch (SecurityException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (IllegalArgumentException e) {
//            e.printStackTrace();
            //} catch (InvocationTargetException e) {
        } catch (Exception e) {
            //e.printStackTrace();
            //Functions.ShowMessage(e.getMessage(), _this);
            Functions.ShowMessage(e.getMessage(), Functions.Functions_cur_this);
            Functions.ShowMessage("Error at invokePrivateMothod_2_s_i():" + e.toString(), Functions.Functions_cur_this);
        }
    }//

    //----------------------------------------
    public static void ShowHtml(Context _this,String fn){
        //ActivityWebBrowser.LocalFileName = fn;
        //Functions.ShowWindow(_this, ActivityWebBrowser.class);
    }//



    //显示一个本程序的顶层窗口
//    public static void HideNetSta()
//    {
//        ActivityNetSta.gThis.finish();
//        ActivityNetSta.gThis.overridePendingTransition(0, 0); //据说可以取消动画，但是没成功//放在 finish 后可以
//
//    }//


    //--------
    //xmpp 的日志
    public static void AddLog(String s)
    {

    }//

    //去掉资源标识符等
//    public static String MakeUserName(String s)
//    {
//        s = s.toLowerCase().trim();//Trim(LowerCase(s));
//        String Result = get_value_sp(s, "", "/");
//
//        return Result;
//    }//



}//



//----------------------------------------------------------------

//2022 http 有很多种方式 //这个是使用第三方库的方式，原来用在医院系统中的，也很成功。不过要加入两个库，还是算了，用原生的吧。
/*
* 在Android2.x的版本中使用HttpURLConnection有bug，但是后来高级版本的Android已经将bug修复，并且做了一些进一步的优化工作，
* 所以建议在高版本的Android中(android2.3)以后使用HttpURLConnection，低版本的仍使用HttpClient

作者：空白_c46e
链接：https://www.jianshu.com/p/8b5c6bc686fd
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
* */

/*
import android.content.Context;
//import android.support.v7.app.AlertDialog;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

//import net.newbt.Functions;

//import java.lang.reflect.Method;
//
//import cz.msebera.android.httpclient.Header;
//
//import static net.newbt.Functions.*;
//import static net.newbt.xmppminiapplication.App.*;

//abstract class  MyHandler extends AsyncHttpResponseHandler{
//class  MyHandler extends AsyncHttpResponseHandler{
class  MyHttpHandler extends AsyncHttpResponseHandler {

    public Context _this;
    public String funcName;

    //请求成功时回调，相应码为200
    @Override
    public void onSuccess(int statusCode, Header[] headers,
                          byte[] responseBody) {
        ////Toast.makeText(MainActivity.this, new String(responseBody), 0).show();
        //Functions.ShowMessage("请求成功" + new String(responseBody), MainActivity.this);
        ////Functions.ShowMessage("请求成功" + new String(responseBody), _this);

        //AppFunctions.invokePrivateMothod(funcName, new String(responseBody), 0,  _this.getClass(), _this);
        //AppFunctions.invokePrivateMothod_2(funcName, new String(responseBody), 0,  _this.getClass(), _this);
        AppFunctions.invokePrivateMothod_2_s_i(funcName, new String(responseBody), 0,  _this.getClass(), _this);
    }

    //请求失败时回调，相应码不为200
    @Override
    public void onFailure(int statusCode, Header[] headers,
                          byte[] responseBody, Throwable error) {
        ////Toast.makeText(MainActivity.this, "请求失败", 0).show();
        //异常//Functions.ShowMessage("请求失败" + new String(responseBody), MainActivity.this);
        ////Functions.ShowMessage("请求失败" + error.getMessage(), MainActivity.this);
        //Functions.ShowMessage("请求失败" + error.getMessage(), _this);

        //AppFunctions.invokePrivateMothod(funcName, new String(responseBody), _this.getClass(), _this);
        //AppFunctions.invokePrivateMothod_2(funcName,error.getMessage(), 1,  _this.getClass(), _this);
        HttpFunctions.invokePrivateMothod_2_s_i(funcName,error.getMessage(), 1,  _this.getClass(), _this);
    }

    @Override
    public void onRetry(int retryNo) {}

    @Override
    public void onStart() {}

}//

//来自 sister_xmpp_an\app\src\main\java\net\newbt\xmppminiapplication
//public class AppFunctions
public class HttpFunctions
{


    //如今的 http 很混乱，所以写一个公用的好了

    //----------------------------------
    //现在的 http 很啰嗦
    public static void HttpGet(String url, Context _this, String funcName){ //get 还是不行,太复杂的 sql 过不了

        try {
            String name = ""; //et_name.getText().toString();
            String pass = ""; //et_pass.getText().toString();

            //String path = "http://169.254.244.136/Web2/servlet/Login";
            String path = url; //"http://www.baidu.com";
            //使用异步HttpClient发送get请求
            AsyncHttpClient client = new AsyncHttpClient();
            client.setTimeout(10 * 1000); //从源码看，最少是 10 秒，设置再小也没用

            //把要提交的数据封装在rp对象中
            RequestParams rp = new RequestParams();
            rp.put("name", name);
            rp.put("pass", pass);
            //发送get请求
            MyHttpHandler hander = new MyHttpHandler();
            hander._this = _this;
            hander.funcName = funcName;
            //client.get(path, rp, hander);
            client.post(path, rp, hander);

            //client.get(path, rp, new ResponseHandlerInterface() {
            //    @Override
            //    public void onSuccess(String response) {
            //        System.out.println(response);
            //    }
            //
            //    @Override
            //    Header[] getRequestHeaders() {}
            //});

        }catch (Exception e){
            e.printStackTrace();
            Functions.ShowMessage(e.toString());
        }

    }//

    //现在的 http 很啰嗦
    public static void HttpPost_sql(String url, String sql, Context _this, String funcName){ //get 还是不行,太复杂的 sql 过不了
        try {
            String name = ""; //et_name.getText().toString();
            String pass = ""; //et_pass.getText().toString();

            //String path = "http://169.254.244.136/Web2/servlet/Login";
            String path = url; //"http://www.baidu.com";
            //使用异步HttpClient发送get请求
            AsyncHttpClient client = new AsyncHttpClient();
            client.setTimeout(10 * 1000); //从源码看，最少是 10 秒，设置再小也没用

            //把要提交的数据封装在rp对象中
            RequestParams rp = new RequestParams();
            rp.put("name", name);
            rp.put("pass", pass);
            rp.put("sql", sql);
            //发送get请求
            MyHttpHandler hander = new MyHttpHandler();
            hander._this = _this;
            hander.funcName = funcName;
            //client.get(path, rp, hander);
            client.post(path, rp, hander);

            //client.get(path, rp, new ResponseHandlerInterface() {
            //    @Override
            //    public void onSuccess(String response) {
            //        System.out.println(response);
            //    }
            //
            //    @Override
            //    Header[] getRequestHeaders() {}
            //});
        }catch (Exception e){
            e.printStackTrace();
            Functions.ShowMessage(e.toString());
        }

    }//

    //--------------------
    //获取并调用私有方法
    public static void invokePrivateMothod(String funcName, String param1, Class<?> cls, Context _this) {
        try {
            // 获取方法名为showName，參数为String类型的方法
            //Method method = cls.getDeclaredMethod("showName", String.class);
            Method method = _this.getClass().getDeclaredMethod(funcName, String.class);
            // 若调用私有方法。必须抑制java对权限的检查
            method.setAccessible(true);
            // 使用invoke调用方法。而且获取方法的返回值。须要传入一个方法所在类的对象，new Object[]
            // {"Kai"}是须要传入的參数，与上面的String.class相相应
            //String string = (String) method.invoke(person, new Object[] { "Kai" } );
            method.invoke(_this, new Object[] { param1 } );

            //System.out.println(string);

//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        } catch (SecurityException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (IllegalArgumentException e) {
//            e.printStackTrace();
        //} catch (InvocationTargetException e) {
        } catch (Exception e) {
            //e.printStackTrace();
            Functions.ShowMessage(e.getMessage(), _this);
        }
    }//

    //两个参数的//注意参数1是字段串，参数2是整数
    public static void invokePrivateMothod_2_s_i(String funcName, String param1, int param2, Class<?> cls, Context _this) {
        try {
            // 获取方法名为showName，參数为String类型的方法
            //Method method = cls.getDeclaredMethod("showName", String.class);
            //Method method = _this.getClass().getDeclaredMethod(funcName, String.class);
            Method method = _this.getClass().getDeclaredMethod(funcName, String.class, int.class);
            // 若调用私有方法。必须抑制java对权限的检查
            method.setAccessible(true);
            // 使用invoke调用方法。而且获取方法的返回值。须要传入一个方法所在类的对象，new Object[]
            // {"Kai"}是须要传入的參数，与上面的String.class相相应
            //String string = (String) method.invoke(person, new Object[] { "Kai" } );
            method.invoke(_this, new Object[] { param1, param2 } );

            //System.out.println(string);

//        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
//        } catch (SecurityException e) {
//            e.printStackTrace();
//        } catch (IllegalAccessException e) {
//            e.printStackTrace();
//        } catch (IllegalArgumentException e) {
//            e.printStackTrace();
            //} catch (InvocationTargetException e) {
        } catch (Exception e) {
            //e.printStackTrace();
            //Functions.ShowMessage(e.getMessage(), _this);
            Functions.ShowMessage(e.getMessage());
            Functions.ShowMessage("Error at invokePrivateMothod_2_s_i():" + e.toString());
        }
    }//

    //----------------------------------------
    public static void ShowHtml(Context _this,String fn){
        ActivityWebBrowser.LocalFileName = fn;
        Functions.ShowWindow(_this, ActivityWebBrowser.class);
    }//



    //显示一个本程序的顶层窗口
//    public static void HideNetSta()
//    {
//        ActivityNetSta.gThis.finish();
//        ActivityNetSta.gThis.overridePendingTransition(0, 0); //据说可以取消动画，但是没成功//放在 finish 后可以
//
//    }//


    //--------
    //xmpp 的日志
    public static void AddLog(String s)
    {

    }//

    //去掉资源标识符等
    public static String MakeUserName(String s)
    {
        s = s.toLowerCase().trim();//Trim(LowerCase(s));
        String Result = get_value_sp(s, "", "/");

        return Result;
    }//



}//
*/