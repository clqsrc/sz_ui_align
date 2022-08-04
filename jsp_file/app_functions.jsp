<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@ page import="java.util.*"%>
<%@ page import="java.sql.*"%>

<%@ include file="functions.jsp"%>
<%@ include file="mysql.jsp"%>
<%@ include file="sms.jsp"%>

<%!
    //final javax.servlet.jsp.JspWriter _out = out;
    
    //Gson 会过滤空值，所以在需要输出空的地方要有一个处理
    public String v(String _v)
    {
        if (null == _v) _v = "";
        return _v;
    }//

    //防注入
    public String v_safe(String _v)
    {
        if (null == _v) _v = "";

        //不要用正则
        _v = _v.replace(" ", ""); //空格
        _v = _v.replace(";", ""); //分号
        _v = _v.replace("'", ""); //单引号
        _v = _v.replace("\"", ""); //
        _v = _v.replace("\r", ""); //
        _v = _v.replace("\n", ""); //
        _v = _v.replace("\t", ""); //

        return _v;
    }//


    //默认的错误处理
    public void error(String es, javax.servlet.jsp.JspWriter out, String debug_msg)
    {
        Gson json = new Gson();

        Map<String, String> kv = new HashMap<String, String>();

        //kv.put("id",key);
        kv.put("error", es); //返回给客户端校验一下 //有个问题，如果没有数据的话会不输出
        kv.put("debug_msg", debug_msg);

        String s = json.toJson(kv);
        
        try{
            out.clear();    //清空一下比较好
            out.println(s);
        }catch (Exception e) {        
            //out.print("错误！ error():");  
            e.printStackTrace();
        }  
    }//
%>

<%

//公用函数//本项目
final javax.servlet.jsp.JspWriter __out = out;
class appfunctions_t
{
    //final javax.servlet.jsp.JspWriter _out = out;
    final javax.servlet.jsp.JspWriter _out = __out;
    final javax.servlet.jsp.JspWriter out = __out;

    //取一个字段值
    public String DBGetValue(String table, String field, String key)
    {
        String r = "";
        
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            String sql = "SELECT " + field + " FROM " + table +
                    " where " + field + " = '" + key + "' " + ";";
            ArrayList<Map<String, String>> rs = mysql.QueryToList(sql);

            if (rs.size()<1) return r;
            

            Map<String, String> line = rs.get(0);
            //out.println(line.get("id")+"   &nbsp  "+ line.get("user_name")+"  &nbsp "+line.get("sms_code")); //将查询结果输出          
            return line.get(field); //将查询结果输出          
            

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBGetValue()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
        }          
    
        return r;
    }//
    
    //取一行数据 //取一个记录值
    public Map<String, String> DBGetLine(String table, String field, String key)
    {
        Map<String, String> r = new HashMap<String, String>();
        
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            String sql = "SELECT " + "*" + " FROM " + table +
                    " where " + field + " = '" + key + "' " + ";";
            ArrayList<Map<String, String>> rs = mysql.QueryToList(sql);

            if (rs.size()<1) return r;
            
      
            return rs.get(0);

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBGetValue()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
        }          
    
        return r;
    }//    
    
    //是否存在
    public Boolean DBHave(String table, String field, String key)
    {
        Boolean r = false;
        
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            String sql = "SELECT " + field + " FROM " + table +
                    " where " + field + " = '" + key + "' " + ";";
            ArrayList<Map<String, String>> rs = mysql.QueryToList(sql);

            if (rs.size()<1) return r;
        }catch (Exception e) {        
            try{ _out.print("数据库连接异常！ DBHave()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
            
            return false;
        }  
        
        return true;
    }//
    
    public Boolean DBSetValue(String table, String field_key, String key, String field_value, String value) throws Exception
    {
        Boolean r = false;
        
        try {  
            Boolean have = DBHave(table, field_key, key);
            
            if (have)
            {
                DBUpdateValue(table, field_key, key, field_value, value);
            }else{
                DBInsertValue(table, field_key, key, field_value, value);
            }
            

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBSetValue()" + e.getMessage()); } catch (Exception e2) { }
            //e.printStackTrace();
            
            throw e;
        }         

        return r;
    }//
    
    public Boolean DBInsertValue(String table, String field_key, String key, String field_value, String value) throws Exception
    {
        Boolean r = false;
        
        String sql = "";
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            sql = "insert into " + table + " (" + field_key + 
                    ", " + field_value +
                    ") values('" + key + 
                    "', '" + value +
                    "' ) " +
                    //" where " + field + "=" + key + "" +
                    ";";


            //try{ out.print("DBInsertValue() sql:" + sql); } catch (Exception e2) { }
            //mysql.Execute("insert into users(user_name) values('ggg') ");
            mysql.Execute(sql);

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBInsertValue()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
            
            throw e;
        }         

        return r;
    }//    
    
    
    public Boolean DBUpdateValue(String table, String field_key, String key, String field_value, String value) throws Exception
    {
        Boolean r = false;
        
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            String sql = "UPDATE " + table + " SET " + 
                    field_value + " = '" + value + "' " +

                    " where " + field_key + " = '" + key + "' " +
                    ";";


            //mysql.Execute("insert into users(user_name) values('ggg') ");
            mysql.Execute(sql);

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBUpdateValue()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
            
            throw e;
        }         
    
        return r;
    }//    
    
    
    //生成一个随机数 6 位
    String GetSmsCode()
    {
        Random rand = new Random();
        Integer code = 100000 + rand.nextInt(800000); //保证是 6 位数

        //对应数据库的 sms_code [users 表,f 数据库]

        return code.toString();
    }//
    
    //2022 查询符合条件的记录，返回到一个 map 的数组中
    //取多行数据 //取多个记录值
    public ArrayList<Map<String, String>> DBGetLineList(String table, String field, String key)
    {
        ArrayList<Map<String, String>> r = new ArrayList<Map<String, String>>();
        
        try {  
            //以后再改成更安全的参数形式或者是 nosql
            String sql = "SELECT " + "*" + " FROM " + table +
                    " where " + field + " = '" + key + "' " + ";";
            ArrayList<Map<String, String>> rs = mysql.QueryToList(sql);

    
            return rs;

        }catch (Exception e) {        
            try{ _out.print("数据库连接异常2！ DBGetLineList()" + e.getMessage()); } catch (Exception e2) { }
            e.printStackTrace();
        }          
    
        return r;
    }// 
    
}//

appfunctions_t app_func = new appfunctions_t(); //虚拟类

%>
