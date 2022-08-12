<%@page import="com.google.gson.Gson"%>
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>

<%@ include file="include.jsp"%> 

<%-- 
<%@ include file="functions.jsp"%> 
<%@ include file="app_functions.jsp"%> 
<%@ include file="include.jsp"%> 
<%@ include file="sms.jsp"%> 
--%>



<%
    //发送注册或者其他验证码


    response.setHeader("Content-Type", "application/json;charset=UTF-8");//注意加上这一句
    String path = request.getContextPath();
    String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";

    //JSONObject json = (JSONObject)request.getAttribute("json");

    //---------------------------------------------------------------
    
    try {  
        
        String phone = request.getParameter("phone");
        
        phone = v(phone);
        
        if (phone.length()<10)
        {
            //throw new Exception("格式错误");
            error("手机号码不正确", out, "格式错误"); //有个多语言的问题
            return;
        }
        
        String key = CreateGuid(); //app_func.GetSmsCode();  //这个返回给客户端
        String value = app_func.GetSmsCode();
        app_func.DBSetValue("smscode_reg_user", "cookie_id", key, "smscode", value);
        
        //---------------------------------------------------------------
        smssender t = new smssender();
        //t.sendsms(value.toString(), "13507734567");
        t.sendsms(value.toString(), phone);
        
        //---------------------------------------------------------------
        
        Gson json = new Gson();

        Map<String, String> kv = new HashMap<String, String>();

        kv.put("id",key);
        kv.put("phone", v(phone)); //返回给客户端校验一下 //有个问题，如果没有数据的话会不输出

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

