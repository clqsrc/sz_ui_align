package com.javasrc.http;


//自定义一个简单的 http

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

//--------------------------------------------------------
//下面是普通 socket

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

//--------------------------------------------------------

public class http {

    //----------------
    //全部页面
    static public ArrayList<http_page> page_list = new ArrayList<http_page>();  //实体中的数据

    //----------------

    //nio 太啰嗦了一点，还是来个简单的线程模式的好了

    public static void CreateServer(int port) 
    {
        try {
           //ServerSocket ss = new ServerSocket(8888);
           ServerSocket ss = new ServerSocket(port);
           System.out.println("启动服务器....");

           ThreadAccept.StartAccept(ss);

        //    Socket s = ss.accept();
        //    System.out.println("客户端:"+s.getInetAddress().getLocalHost()+"已连接到服务器");
           
        //    BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
        //    //读取客户端发送来的消息
        //    String mess = br.readLine();
        //    System.out.println("客户端："+mess);
        //    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
        //    bw.write(mess+"\n");
        //    bw.flush();

        //} catch (IOException e) {
        } catch (Exception e) {
    
           e.printStackTrace();
        }
     }//

     
    
}//

//子类
class ThreadAccept implements Runnable{

    public ServerSocket ss = null;

    @Override
    public void run() {
        System.out.println("Thread State is："+Thread.currentThread().getState());

        while(true){
            try {
            //ServerSocket ss = new ServerSocket(8888);
            //System.out.println("启动服务器....");


            Socket s = ss.accept();
            System.out.println("客户端:"+s.getInetAddress().getLocalHost()+"已连接到服务器");

            ThreadSocket.StartSocket(s);

            //} catch (IOException e) {
            } catch (Exception e) {

                e.printStackTrace();

            }//try

        }//while


    }//

    public static void StartAccept(ServerSocket ss) throws Exception{

        ThreadAccept t = new ThreadAccept();
        
        t.ss = ss;

        Thread thread = new Thread(t);
        thread.start();

    }//

}//


//子类
class ThreadSocket implements Runnable{

    //public ServerSocket ss = null;
    public Socket s = null;

    public static void StartSocket(Socket s) throws Exception{

        ThreadSocket t = new ThreadSocket();
        
        t.s = s;

        Thread thread = new Thread(t);
        thread.start();

    }//

    //--------------------------------------------------------

    //是否已处理 http 请求，没有的话就返回 404
    int Handled = 0; //Handler/Handled

    //----

    BufferedReader _br = null;
    BufferedWriter _bw = null;

    @Override
    public void run() {
        System.out.println("Thread State is:"+Thread.currentThread().getState());

        try {
            //ServerSocket ss = new ServerSocket(8888);
            //System.out.println("启动服务器....");
            //Socket s = ss.accept();
            //System.out.println("客户端:"+s.getInetAddress().getLocalHost()+"已连接到服务器");
            
            BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
            _br = br;


            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
            _bw = bw;


            this.ReadHeads();

            //--------
            //分配给哪个页面处理类
            for (int i = 0; i < http.page_list.size(); i++) {

                http_page page = http.page_list.get(i);

                //如果路径对上了就用它处理  //按道理换用 map 更快，不过无所谓了，本项目访问压力不大
                if (page.http_path.equals(this.http_url_path)) {

                    //注意这是每次请求都新建的实例
                    http_page.cur_page _cur_page = new http_page.cur_page();

                    _cur_page.get_params = this.get_params;
                    _cur_page.params = this.params;

                    String response = page.hander(_cur_page);

                    SendHttpResponse(response);

                    return;
                }//if
                
            }//for

            //--------

            //没有处理的话就是 404 .其实标准不是这样的，不过目前的浏览器也可以支持
            if (0 == Handled){
                SendString("HTTP/1.0 200 OK\r\n");
                SendString("server: java server http\r\n");
                SendString("Access-Control-Allow-Origin: *\r\n");   //跨域
                SendString("\r\n");
                SendString("404. java server http.");
            }//if

            if (true) return;
            //--------------------------------------------------------

            //读取客户端发送来的消息
            String mess = br.readLine();
            System.out.println("客户端："+mess);
            
            bw.write("hi you say:" + mess + "\n");
            bw.flush();


        } catch (Exception e) { // (IOException e) 

            e.printStackTrace();
        }
        finally{
            //最后关闭了 socket 所以是符合 http 协议的
            try { s.close(); }catch(Exception e2) { e2.printStackTrace(); }
        }


    }//


    //--------------------------------------------------------
    //发送 http 回应
    public void SendHttpResponse(String response) throws Exception
    {
        //if (0 == Handled){
            SendString("HTTP/1.0 200 OK\r\n");
            SendString("server: java server http\r\n");
            SendString("Access-Control-Allow-Origin: *\r\n");   //跨域
            SendString("\r\n");
            //SendString("404. java server http.");
        //}//if

        SendString(response);

    }//


    //--------------------------------------------------------

    //读取一行
    public String ReadLine() throws Exception{
        
        //读取客户端发送来的消息
        String mess = _br.readLine();

        return mess;
    }//

    public void SendString(String s) throws Exception
    {
        //_bw.write("hi you say:" + mess + "\n");
        _bw.write(s);
        _bw.flush();

    }//

    //读取完整的 http 头信息
    Hashtable<String, String> heads = new Hashtable<String, String>();
    ArrayList<String> head_lines = new ArrayList<String>();  //头信息的原始数据

    ArrayList<String> body_lines = new ArrayList<String>();  //实体中的数据

    //----
    Hashtable<String, String> get_params = new Hashtable<String, String>();  //get 请求的参数
    Hashtable<String, String> params = new Hashtable<String, String>();  //get 和 pos 请求的参数

    public String http_url_path = "";  //当前的访问路径，如 "index.jsp"
     
    public void ReadHeads() throws Exception
    {
        String first_line = ReadLine(); //http 的首行要单独读取

        System.out.println(first_line);

        for (int i=0; i<1000; i++) //只读取 1000 行的保护
        {
            String line = ReadLine();

            head_lines.add(line);
            

            if (line.length() == 0) break;  //空行就表示头信息结束了。如果只处理 get 请求的话就不用处理后面的了

        }//for

        //--------------------------------------------------------
        //简单一点，处理一下首行好了
        //GET /index.jsp HTTP/1.1
        String[] _first_line = first_line.split(" ");
        if (_first_line.length>2){
            String url = _first_line[1];  //第二行

            String[] _url = ThreadSocket.split_two(url, "?");

            url = _url[0];  //"?"问号前面的地是地址
            this.http_url_path = url;
            String[] request = _url[1].split("&"); //问号后面的是 get 参数
            
            for (int i = 0; i < request.length; i++) {

                String param = request[i];

                String[] _kv = ThreadSocket.split_two(param, "=");

                String key = _kv[0];
                String value = _kv[1];

                String _value  = java.net.URLDecoder.decode(value, "UTF-8");  

                //get_params.put(key, value);
                get_params.put(key, _value);  //值要解码 http
                params.put(key, _value);

            }//

        }//if

        //--------------------------------------------------------

    }//func

    //分隔成两行
    public static String[] split_two(String s, String ch)
    {
        String[] lines = new String[]{"", ""};

        int pos = s.indexOf(ch, 0);

        lines[0] = s;  //默认放在第一个位置中

        if (pos>-1){ //找到了才分割

            lines[0] = s.substring(0, pos);
            lines[1] = s.substring(pos + ch.length());
        }//if

        return lines;
    }//

}//class



