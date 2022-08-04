<%@page import="com.google.gson.Gson"%>
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>

<%@ include file="include.jsp"%> 

<%-- 
<%@ include file="functions.jsp"%> 
<%@ include file="app_functions.jsp"%> 
<%@ include file="include.jsp"%> 
--%>



<%
    //登录

    response.setHeader("Content-Type", "application/json;charset=UTF-8");//注意加上这一句
    String path = request.getContextPath();
    String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";

    //JSONObject json = (JSONObject)request.getAttribute("json");

    //---------------------------------------------------------------
    
    try {  
        
        //String phone = v(request.getParameter("phone")); //phone 接收验证码的手机号
        //String smscode = v(request.getParameter("smscode")); //手机收到的验证码
        //String id = v(request.getParameter("id")); //本次对话的 session id 。
        String user_name = v_safe(request.getParameter("user_name")); //用户名
        String user_pass = v_safe(request.getParameter("user_pass")); //密码
        
        //user_name
        //用户名（在目前的 ui 设计中，直接传递界面上的手机号就可以了。即与 phone 字段相同。）
        
        
        //String session_id = id;// CreateGuid(); //app_func.GetSmsCode();  //这个返回给客户端
        
        //phone = v(phone);
        
        //---------------------------------------------------------------
        //参数验证

        
        if (user_name.length()<1)
        {
            error("用户名不能为空", out, "格式错误"); //有个多语言的问题
            return;
        }
        
//        if (user_pass.length()<1)
//        {
//            error("密码不能为空", out, "格式错误"); //有个多语言的问题
//            return;
//        }
        

        //---------------------------------------------------------------
        //用户信息
        ArrayList<Map<String, String>> kv_user_list = app_func.DBGetLineList("users_files", "user_name", user_name);
        
        //app_func.DBSetValue("smscode_reg_user", "cookie_id", session_id, "is_use", "1"); //用过了
        
        //方便以后修改为 nosql
        //app_func.DBSetValue("users", "user_name", user_name, "user_pass", user_pass); //其实是更新
        //app_func.DBSetValue("users", "user_name", user_name, "phone", phone); //用过了
        
        Gson json = new Gson();

        //Map<String, String> kv = new HashMap<String, String>();
        Map<String, Object> kv = new HashMap<String, Object>();

        //kv.put("id", session_id);
        //kv.put("phone", v(phone)); //返回给客户端校验一下 //有个问题，如果没有数据的话会不输出
        kv.put("result", "1");
        kv.put("list", kv_user_list);

        String s = json.toJson(kv);
        
        out.clear();    //清空一下比较好
        out.println(s);

        //throw new Exception("ttt");

        //return;
    }catch (Exception e) {        
        //out.print("数据库连接异常2！");  
        
        error("未能完成", out, e.getMessage());
        
        e.printStackTrace();
    }    
  
    //error("未能完成", out);
    //return;
    //---------------------------------------------------------------
    

%>

