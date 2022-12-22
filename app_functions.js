
/// <reference path="jquery-1.11.3.min.js" />
/// <reference path="my_ajax.js" />

//一些还没决定怎么处理的函数暂时放这里

function f1()			
{
	document.writeln("f1...");
	//document.getElementById("tt").innerHTML = "test1";
			
}//


//UI_HttpPost_SetParam("memo", "bbb中文");
//UI_HttpPost_v2("http://www.net/ms/json_index.php", self, CarLoginForm_Http_OnEvent);

//g_UI_HttpPost_SetParam = {};  //全局变量
g_UI_HttpPost_SetParam = "";  //全局变量 //其实是字符串就行了

//UI_HttpPost 为避免内存泄漏，实现上是单实例的，实现太重了，可能再实现一个 UI_HttpGet ，不过实际上是可以用的
function UI_HttpPost_SetParam(key, value)
{
	g_UI_HttpPost_SetParam = g_UI_HttpPost_SetParam + 
		function_httpEncode(key) + "=" +
		function_httpEncode(value) + "&";

}//

//兼容 lua 语法
function UI_HttpPost(url, param, funcHttp_OnEvent)
{
	var post_data = g_UI_HttpPost_SetParam;
	//url_post_async(url, post_data, funcHttp_OnEvent);

    //--------------------------------------------------------
    //为了兼容自动生成的 lua 代码中第一个参数是 self 的情况，如果是带有 self 的，则忽略第 1 个参数
    var f = function(){};
    f.length;   //length 是函数对象的一个属性值，指明该函数期望多少个参数，意即形参的个数
    f.arguments; //arguments.length 是函数被调用时实际传参的个数

    //alert(funcHttp_OnEvent.length);

    var _lua_self_ = false;
    if (funcHttp_OnEvent.length >= 3) _lua_self_ = true;

	//--------------------------------------------------------
	function func(s){

        if (_lua_self_) { funcHttp_OnEvent(null, s, "ok"); return ;}
		funcHttp_OnEvent(s, "ok");

	}//成功
	
	function func_error(s){

        if (_lua_self_) { funcHttp_OnEvent(null, s, "error"); return ;}

		funcHttp_OnEvent(s, "error");

	}//失败

	//--------------------------------------------------------

	//为和 lua 通用，多写一点
	url_post_async(url, post_data, func, func_error);

	g_UI_HttpPost_SetParam = "";  //要清空

}//


