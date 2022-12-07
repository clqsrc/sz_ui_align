// package com.javasrc.http;

import com.javasrc.http.*;

// package com.javasrc.http;



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

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.google.gson.reflect.*;
import com.google.gson.*;
import java.lang.*;
import java.lang.reflect.*;


//最终的普通页面
//文件合并  //修改自 http_page_json_t1

//--------------------------------------------------------


//----

public class http_page_file_to_one extends http_page {

    //处理函数 //返回的字符串就是网页中要显示的内容或者是 json
    @Override
    public String hander() 
    {
        try {

            String id = this.params.get("id"); //相当于 jsp 中的 request.getParameter("id")

            //要合并的文件列表
            String file_list = this.params.get("file_list");

            Gson json_file_list = new Gson(); 
            //List<String> e = json_file_list.fromJson(file_list, new TypeToken<List<String>>());
            //JsonElement e; 
            //List<String> ps = json_file_list.fromJson(file_list, new TypeToken<List<String>>(){}.getType());
            //List<String> ps = json_file_list.fromJson(file_list, <List<String>>.class);

            //将json字符串转换为map集合 //https://www.jb51.net/article/228962.htm
            // java.lang.reflect.Type type = new TypeToken<Map<Integer, String>>() {
            // }.getType();
            
            // Map<Integer, String> map1 = json_file_list.fromJson(file_list, type);

            //String[] ps;
            //json_file_list.fromJson(file_list, ps);

            //解码比较啰嗦
            java.lang.reflect.Type type = new TypeToken<String[]>() { }.getType();
            String[] fn_list = json_file_list.fromJson(file_list, type);

            System.out.println(fn_list);

            //这个解码不了数组
            //JsonObject returnData = new JsonParser().parse(file_list).getAsJsonObject();
            //用对象的话更通用一点
            JsonObject returnData = new JsonParser().parse("{\"file_list\":[\"1\", \"2\"]}").getAsJsonObject();

            JsonElement e = returnData.get("file_list");
            JsonArray arr =  e.getAsJsonArray();

            //用这个则是通用的.不过怎么判断是数组呢 e2.isJsonArray()
            JsonElement e2 = new JsonParser().parse(file_list);
            //e2.isJsonArray()

            //----
            //编码

            Gson json = new Gson();

            Map<String, String> kv = new HashMap<String, String>();

            //kv.put("id", session_id);
            //kv.put("phone", v(phone)); //返回给客户端校验一下 //有个问题，如果没有数据的话会不输出
            kv.put("result", "1");
            
            
            
            String s = json.toJson(kv);
            
            //out.clear();    //清空一下比较好
            ////out.println(s);

            return s;

            //return "java server http. index.jsp " + id;

            
        } catch (Exception e) {
    
           e.printStackTrace();
        }

        return "error";

     }//

     
    
}//

//--------------------------------------------------------

