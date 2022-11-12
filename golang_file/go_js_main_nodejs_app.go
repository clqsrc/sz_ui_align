package main

//import lua "github.com/yuin/gopher-lua"

import "github.com/robertkrimen/otto"
//otto 用到了 https://github.com/go-sourcemap/sourcemap/tree/v1.0.5

import (
	
	"fmt"
//	"github.com/robertkrimen/otto/parser"
)

//尝试模拟 nodejs 环境  //本程序的， go_js_main_nodejs 中的则是公用的 

//参考 Motto提供了类似于Nodejs的模块环境，可以在golang中运行javascript文件。基于 otto
//https://github.com/ddliu/motto
//https://github.com/ddliu/nodego


//解码一段字符串的邮件  //传递变量的方式
func main_js_nodejs_DecodeMail_forString_old(mail_src string) string {
	
	defer PrintError("main_js_nodejs_DecodeMail_forString");

	r := "";
	//--------

	//var src1 = LoadFromFile_String("t1.js");
	var src1 = LoadFromFile_String("mail_decode.js");
	
	
	//--------------------------------------------------------
	//下面是 js 脚本
	fmt.Println("-----------------------------------------\r\n js ...");
	
	vm := otto.New()
	vm.Run(`
		
		//不支持 typescript //也不支持 es6  //不过可以先用 typescript 的 tsc 先转换即可
		//const hello : string = "Hello World!"
		//console.log(hello)

	    abc = 2 + 2;
	    console.log("The value of abc is " + abc); // 4
	`);
	
	//https://github.com/go-git/go-git
	//--------
	//设置1个函数
	vm.Set( "sayHello" , func(call otto.FunctionCall) otto.Value {      
		fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())      
		return otto.Value{} 
	});

	//设置1个带有返回值的函数
	vm.Set( "twoPlus" , func(call otto.FunctionCall) otto.Value {      
		right, _ := call.Argument( 0 ).ToInteger()      
		result, _ := vm.ToValue( 2 + right)      
		return result 
	});
	
	//----
	//实现 require 函数的话就不用直接运行了
	// var src2 = LoadFromFile_String("t2.js");
	// v, err := vm.Run(src2); 
	
	// if (nil != err) {
	// 	fmt.Println(err);
	// }else{
	// 	fmt.Println(v);
	// }//
	
	//----
	//设置1个函数
	vm.Set( "ShowMessage" , func(call otto.FunctionCall) otto.Value {  
	    
		//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
		js_ShowMessage(call.Argument(0).String());
		    
		return otto.Value{} 
	});
	
	vm.Set( "require" , func(call otto.FunctionCall) otto.Value {  
	    
		//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
		return js_require(vm, call.Argument(0).String());
		    
		return otto.Value{} 
	});

	//----
	vm.Set("mail_src", mail_src);

	//----
	
	
	v, err := vm.Run(src1); 
	
	if (nil != err) {
		
		fmt.Println(err);
		
	}else{
		fmt.Println(v);
		
	}//

	//--------
	//如何取一个 js 脚本中的变量值呢
	v_js, err_js := vm.Get("result"); 
	
	if (nil != err_js) {
		
		fmt.Println(err_js);
		
	}else{

		//if v_js.IsString();
		r,_ = v_js.ToString();

		fmt.Println(v_js);
		
	}//

	//--------
	
	return r;

	
}//

//解码一段字符串的邮件 //直接调用函数的方式，参考 gs 的 src_map
func main_js_nodejs_DecodeMail_forString(mail_src string) string {
	
	defer PrintError("main_js_nodejs_DecodeMail_forString");

	r := "";
	//--------

	//var src1 = LoadFromFile_String("t1.js");
	////var src1 = LoadFromFile_String("mail_decode.js");
	
	//----
	var js_request = make(map[string]string);
	//设置参数  //对 js 来说可以简单理解为 http 的 request ，本质上也相当于 webservice 本地化
	//vm.Set( "js_request" , golang_param);
	
	js_request["file_name"] = "ttt.txt";

	vm := main_js_nodejs_RunJS("mail_decode_v2.js", js_request); 
	
	r = main_js_nodejs_RunJS_CallFunction(vm, "DecodeMail_forString", mail_src);  //运行 js 中的 DecodeMail_forString() 函数；
	
	return r;
	
	//----
	
	
}//


