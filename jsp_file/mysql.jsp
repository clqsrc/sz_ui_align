<%@ page contentType="text/html" pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="java.util.*"%>
<%

    final javax.servlet.jsp.JspWriter _out = out;
    //final javax.servlet.jsp.JspWriter _out = out; //要在内部类使用，一定要加 final

    final class mysql_t
    {
        //---------------------------------------------------------------
        //这是内部类，在目前的 jsp 版本中不能直接访问 jsp 的全局变量。变通方法是给这个类虚拟一个
        //否则会报 "从内部类中访问本地变量out; 需要被声明为最终类型"
        javax.servlet.jsp.JspWriter out = _out;
        
        
        //---------------------------------------------------------------
        //驱动的配置
        //final String driver_name = "com.mysql.jdbc.Driver";  //驱动程序名 //这是 mysql 5 的，据说 mysql 8 后改名称了
        final String driver_name = "com.mysql.cj.jdbc.Driver";  //驱动程序名
        //String url = "jdbc:mysql://localhost:3306/mysql"; //数据库名
        //final String url = "jdbc:mysql://localhost:3306/f4"; //数据库名
        //final String url = "jdbc:mysql://localhost:3306/ad_map"; //数据库名
        //final String url = "jdbc:mysql://localhost:3306/ad_map?useUnicode=true&characterEncoding=utf-8&serverTimezone=GMT%2B8";
        //final String url = "jdbc:mysql://localhost:3306/ad_map?useUnicode=true&characterEncoding=utf-8&serverTimezone=UTC";
        
        //如果报 Keystore was tampered with, or password was incorrect 
        //在 mysql 8 下要用 useSSL=false
        //final String url = "jdbc:mysql://localhost:3306/ad_map?trustServerCertificate=false&useUnicode=true&characterEncoding=utf-8&serverTimezone=UTC";
        final String url = "jdbc:mysql://localhost:3306/ad_map?useSSL=false&useUnicode=true&characterEncoding=utf-8&serverTimezone=UTC";
        

        
        final String username = "root";  //数据库用户名
        final String password = "";  //数据库用户密码
        
        //---------------------------------------------------------------


        //查询语句
        public ResultSet Query(String s)
        {
            //javax.servlet.jsp.JspWriter out = _out;

            //---------------------------------------------------------------
            try {  
                //Class.forName("com.mysql.jdbc.Driver");  //驱动程序名 //这是 mysql 5 的，据说 mysql 8 后改名称了
                //Class.forName("com.mysql.jdbc.Driver");  //驱动程序名
                Class.forName(driver_name);  //驱动程序名
                //String url = "jdbc:mysql://localhost:3306/mysql"; //数据库名
                //String url = "jdbc:mysql://localhost:3306/f"; //数据库名
                //String username = "root";  //数据库用户名
                //String password = "F";  //数据库用户密码
                Connection conn = DriverManager.getConnection(url, username, password);  //连接状态

                if(conn != null)
                {  
//                    out.print("数据库连接成功！");  
//                    out.print("<br />");            
                    Statement stmt = null;  
                    ResultSet rs = null;  
                    //String sql = "SELECT *FROM users;";  //查询语句
                    String sql = s; //"SELECT *FROM users;";  //查询语句
                    stmt = conn.createStatement();  
                    rs = stmt.executeQuery(sql);  

    //                out.print("查询结果：");  
    //                out.print("<br />");  
    //                out.println("姓名"+"  "+"性别 "+"  "+"年龄");  
    //                out.print("<br />");
    //
    //                while (rs.next()) {  
    //                 out.println(rs.getString("id")+"   &nbsp  "+rs.getString("user_name")+"  &nbsp "+rs.getInt("sms_code")); //将查询结果输出  
    //                }

                    return rs;
                }
                else{  
                    out.print("连接失败！");  
                }  
            }catch (Exception e) {     
                try{
                out.print("数据库连接异常！");  
                }catch (Exception e2) {}  

                e.printStackTrace();
            } 

            return null;
            
        }//func
        
        //Map<String, String> kv2 = new HashMap<String, String>();
        //ArrayList<Integer> list = new ArrayList<Integer>();
        
        
        //ArrayList<Map<String, String>> list = new ArrayList<Map<String, String>>();
        
        //查询语句 //用 list 接收，因为数据库可能要关闭
        public ArrayList<Map<String, String>> QueryToList(String s) throws Exception
        {
            //javax.servlet.jsp.JspWriter out = _out;
            
            //---------------------------------------------------------------
            Connection conn = null;
            Statement stmt = null;  
            ResultSet rs = null;  
            try {  
                //Class.forName("com.mysql.jdbc.Driver");  //驱动程序名
                Class.forName(driver_name);  //驱动程序名
                //String url = "jdbc:mysql://localhost:3306/mysql"; //数据库名
//                String url = "jdbc:mysql://localhost:3306/f"; //数据库名
//                String username = "root";  //数据库用户名
//                String password = "F2017";  //数据库用户密码
//                Connection conn = DriverManager.getConnection(url, username, password);  //连接状态
                conn = DriverManager.getConnection(url, username, password);  //连接状态

                if(conn != null)
                {  
//                    out.print("数据库连接成功！");  
//                    out.print("<br />");            
//                    Statement stmt = null;  
//                    ResultSet rs = null;  
                    //String sql = "SELECT *FROM users;";  //查询语句
                    String sql = s; //"SELECT *FROM users;";  //查询语句
                    stmt = conn.createStatement();  
                    rs = stmt.executeQuery(sql);  

    //                out.print("查询结果：");  
    //                out.print("<br />");  
    //                out.println("姓名"+"  "+"性别 "+"  "+"年龄");  
    //                out.print("<br />");
    //
    //                while (rs.next()) {  
    //                 out.println(rs.getString("id")+"   &nbsp  "+rs.getString("user_name")+"  &nbsp "+rs.getInt("sms_code")); //将查询结果输出  
    //                }
    

                    //取字段名
                    ResultSetMetaData rsm = rs.getMetaData(); 
                    int colNum = 0;
                    colNum = rsm.getColumnCount();
                    
                    
                    ArrayList<String> field_name_list = new ArrayList<String>();
                    
                    for(int i = 1;i <= colNum;i++) //jdbc 的取值居然是从 1 开始的
                    {
                        String name = rsm.getColumnName(i);
                        field_name_list.add(name);
                    }//for

                    //----
                    
                    ArrayList<Map<String, String>> list = new ArrayList<Map<String, String>>();
                    
                    while (rs.next()) 
                    {  
                        //out.println(rs.getString("id")+"   &nbsp  "+rs.getString("user_name")+"  &nbsp "+rs.getInt("sms_code")); //将查询结果输出  

                        Map<String, String> line = new HashMap<String, String>(); //这是一行
                        
                        for(int i=0; i<colNum; i++)
                        {
                            String field_name = field_name_list.get(i);

                            String value = rs.getString(i+1); //jdbc 的取值居然是从 1 开始的

                            line.put(field_name, value);
                        }            
                        
                        list.add(line);
                    }//while
                    
                    
                    return list;
                    //return rs;
                }
                else{  
                    out.print("连接失败！ QueryToList()");  
                }  
            }catch (Exception e) {     
                try{
                out.print("数据库连接异常！QueryToList()" + e.getMessage());  
                }catch (Exception e2) {}  

                e.printStackTrace();
                
                //throw e;
                throw new Exception("数据库连接异常！QueryToList()" + e.getMessage()); 
                
            }finally{ //还是关闭好点
                try{ conn.close(); }catch (Exception e2) {}  
                try{ stmt.close(); }catch (Exception e2) {}  
                try{ rs.close(); }catch (Exception e2) {}  
            }

            return null;
            
        }//func
        
        //执行一条语句
        public int Execute(String s) throws Exception
        {
            //javax.servlet.jsp.JspWriter out = _out;
            
            //---------------------------------------------------------------
            Connection conn = null;
            Statement stmt = null;  
            //ResultSet rs = null;  
            try {  
                //Class.forName("com.mysql.jdbc.Driver");  //驱动程序名
                Class.forName(driver_name);  //驱动程序名
                //String url = "jdbc:mysql://localhost:3306/mysql"; //数据库名
//                String url = "jdbc:mysql://localhost:3306/f"; //数据库名
//                String username = "root";  //数据库用户名
//                String password = "F2017";  //数据库用户密码
//                Connection conn = DriverManager.getConnection(url, username, password);  //连接状态
                conn = DriverManager.getConnection(url, username, password);  //连接状态

                if(conn != null)
                {  
//                    out.print("数据库连接成功！");  
//                    out.print("<br />");            
//                    Statement stmt = null;  
//                    ResultSet rs = null;  
                    //String sql = "SELECT *FROM users;";  //查询语句
                    String sql = s; //"SELECT *FROM users;";  //查询语句
                    stmt = conn.createStatement();  
                    //rs = stmt.executeQuery(sql);  
                    int r = stmt.executeUpdate(sql);


    


                    //----
                    
                    return r;
                    //return rs;
                }
                else{  
                    out.print("连接失败！ Execute()");  
                }  
            }catch (Exception e) {     
                try{
                out.print("数据库连接异常！Execute()" + e.getMessage());  
                }catch (Exception e2) {}  

                e.printStackTrace();
                
                //throw e;
                //throw new Exception("数据库连接异常！Execute()" + s + e.getMessage()); 
                throw new Exception("数据库连接异常！Execute()" + e.getMessage()); 
                
            }finally{ //还是关闭好点
                try{ conn.close(); }catch (Exception e2) {}  
                try{ stmt.close(); }catch (Exception e2) {}  
                //try{ rs.close(); }catch (Exception e2) {}  
            }

            return 0;
            
        }//func
        
       

    }//class
    
    //----------------

    final mysql_t mysql = new mysql_t(); //虚拟类

%>
