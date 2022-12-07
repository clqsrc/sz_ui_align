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


//单个的页面都从这个类继承

//--------------------------------------------------------


//----

public class http_page {

    //自己要处理的路径
    public String http_path = "";  //只读，由调用者传入

    public Hashtable<String, String> get_params;  //get 请求的参数 //只读，由调用者传入
    public Hashtable<String, String> params;  //get 和 pos 请求的参数 //只读，由调用者传入

    //指明自己要处理的路径 //加入到 http.page_list 中
    public static void AddToPageList(http_page page, String http_path)
    {
        //http.page_list.add(this);

        page.http_path = http_path;

        http.page_list.add(page);

    }//

    //处理函数 //返回值为 0 的话就是没处理
    public String hander() 
    {
        try {

            
        } catch (Exception e) {
    
           e.printStackTrace();
        }

        return "";

     }//

     
    
}//

//--------------------------------------------------------

