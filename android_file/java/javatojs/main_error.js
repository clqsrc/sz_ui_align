

try{

//--------------------------------------------------------

alert("aaa");

function RunJavaFunc(func_name, json_param, bin_param)
{
    // //主要使用 json_param，而 bin_param 是用在传输大的二进制或者非常大的字符串时
    //     //据说一定要用 final 函数
    //     // sdk17版本以上加上注解
    //     @JavascriptInterface
    //     public String RunJavaFunc(final String func_name, final String json_param, final String bin_param)

    return RunJava.RunJavaFunc(func_name, json_param, bin_param);

}//


RunJavaFunc("ShowMessage", "", "aaa");



//--------------------------------------------------------

} catch (e) {

    e.toString().aaa();  //确实成功到达了这里。这个语句会故意在 asd 中生成可见的字符串

    console.log("发生异常:" + e)
    //log("发生异常:" + e);
}//



