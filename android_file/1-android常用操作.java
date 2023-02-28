

//取一个 view

try {

    EditText txtID  = findViewById(R.id.txtID);

}
catch (Exception e)
{
    e.printStackTrace();
    Functions.ShowMessage("有异常。" + e.getMessage()); //这句话也可能会异常，所以没有装载成功时一定要调试一下
}



//获取e.printStackTrace() 的具体信息，赋值给String 变量，并返回
//位于 java_file.functions.java
public static String GetStackTraceInfo(Exception e) {}






