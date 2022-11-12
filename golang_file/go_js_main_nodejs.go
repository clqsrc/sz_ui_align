package main

import lua "github.com/yuin/gopher-lua"

import "github.com/robertkrimen/otto"
//otto 用到了 https://github.com/go-sourcemap/sourcemap/tree/v1.0.5

import (
	
	"fmt"
	"github.com/robertkrimen/otto/parser"
)

//尝试模拟 nodejs 环境

//参考 Motto提供了类似于Nodejs的模块环境，可以在golang中运行javascript文件。基于 otto
//https://github.com/ddliu/motto
//https://github.com/ddliu/nodego

func main_js_nodejs() {
	
	L := lua.NewState(); // 创建一个lua解释器实例
	
	defer L.Close();
	
    // 执行字符串语句
	if err := L.DoString(`print("hello")`); err != nil {
		panic(err);
		
	}//
	
	//--------------------------------------------------------
	//下面是 js 脚本
	fmt.Println("-----------------------------------------\r\n js ...");
	
	vm := otto.New()
	vm.Run(`
		
		//不支持 typescript
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
	
	//--------
	//https://www.qedev.com/dev/118210.html
	// 如果你只是对AST（抽象语法树）感兴趣，你可以获得一个分离的语法解析器
	//http://godoc.org/github.com/robertkrimen/otto/parser

	//解析并返回一个AST
	filename :=  ""
	// A filename is optional
	src := `     
	// Sample xyzzy example
	     ( function (){          if ( 3.14159 >  0 ) {              console.log( "Hello, World." );              return ;          }            var xyzzy =  NaN ;          console.log( "Nothing happens." );          return xyzzy;      })(); ` 
	     
	// Parse some JavaScript, yielding a *ast.Program and/or an ErrorList
	program, err := parser.ParseFile(nil, filename, src,  0 );
	
	fmt.Println(err, program);
	
}//


func js_ShowMessage(s string){
	
	fmt.Println("js_ShowMessage: ", s);
	
}//

//js 中 require 的简化实现 //目前只是运行，不返回 export //ok 已经可以返回值
func js_require(vm * otto.Otto, fn string) (otto.Value) {
	
	fn = fn + ".js";
	
	r := otto.Value{};
	
	//--------------------------------------------------------
	//otto 居然是可以支持 module.exports 的，不过要先执行一次 module 初始化
	//var module={};
	//module.exports = { mail_error: mail_error, MailMime_errorMail_forTest: MailMime_errorMail_forTest }; //{hello,hello2}
	
	v, err := vm.Run("var module={};"); 
	
	if (nil != err) {
		fmt.Println(err);
	}else{
		fmt.Println(v);
	}//
	
	//--------------------------------------------------------
	
	//----
	//var src2 = LoadFromFile_String("t2.js");
	var src2 = LoadFromFile_String(fn);
	v, err = vm.Run(src2); 
	
	if (nil != err) {
		fmt.Println(err);
	}else{
		fmt.Println(v);
		
		return v;
	}//
	
	//----
	
	return r;
	
}//

//js 中的 nodejs 的文件操作默认是异步的，所以要给一个同步的版本 SaveToFile_Sync
//declare function SaveToFile_Sync(fn:any, s:any, msg_show:any):any;
func js_SaveToFile_Sync(vm * otto.Otto, fn string, s string, msg_show int) (otto.Value) {
	
	//fn = fn + ".js";
	
	r := otto.Value{};
	
	//--------------------------------------------------------

	//--------------------------------------------------------
	SaveToFile(s, fn);

	
	//----
	
	return r;
	
}//

func js_LoadFromFile(vm * otto.Otto, fn string) (otto.Value) {
	
	//fn = fn + ".js";
	
	r := otto.Value{};
	
	//--------------------------------------------------------

	//--------------------------------------------------------

	var src2 = LoadFromFile_String(fn);
	// v, err = vm.Run(src2); 
	
	// if (nil != err) {
	// 	fmt.Println(err);
	// }else{
	// 	fmt.Println(v);
		
	// 	return v;
	// }//
	
	//----
	//result, _ := vm.ToValue( 2 + right)      
	result, _ := vm.ToValue(src2);
	
	r = result; 
	
	return r;
	
}//


func main_js_nodejs_t1() {
	
	var src1 = LoadFromFile_String("t1.js");
	
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
	
	
	v, err := vm.Run(src1); 
	
	if (nil != err) {
		
		fmt.Println(err);
		
	}else{
		fmt.Println(v);
		
	}//
	
	return;
	
	
	//--------------------------------------------------------
	
	//--------
	//https://www.qedev.com/dev/118210.html
	// 如果你只是对AST（抽象语法树）感兴趣，你可以获得一个分离的语法解析器
	//http://godoc.org/github.com/robertkrimen/otto/parser

	//解析并返回一个AST
	filename :=  ""
	// A filename is optional
	src := `     
	// Sample xyzzy example
	     ( function (){          if ( 3.14159 >  0 ) {              console.log( "Hello, World." );              return ;          }            var xyzzy =  NaN ;          console.log( "Nothing happens." );          return xyzzy;      })(); ` 
	     
	// Parse some JavaScript, yielding a *ast.Program and/or an ErrorList
	program, err := parser.ParseFile(nil, filename, src,  0 );
	
	fmt.Println(err, program);
	
}//


//正式的运行一个脚本，每个脚本是独立的一个运行时，如果需要其他 js ，通过 require 来引用
//golang_param 是传递给脚本的参数，一般情况下是个 string map
//返回值是一个 json 格式的字符串
func main_js_nodejs_RunJS(fn string, golang_param interface{}) (*otto.Otto) {
	
	defer PrintError("main_js_nodejs_RunJS");
	
	//var src1 = LoadFromFile_String("t1.js");
	var src1 = LoadFromFile_String(fn);
	
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
	
	vm.Set( "SaveToFile_Sync" , func(call otto.FunctionCall) otto.Value {  
	    
		//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
		var msg_show int64 = 0;
		msg_show, _ = call.Argument(2).ToInteger();
		return js_SaveToFile_Sync(vm, call.Argument(0).String(), call.Argument(1).String(), int(msg_show));
		    
		return otto.Value{} 
	});
	
	vm.Set( "LoadFromFile" , func(call otto.FunctionCall) otto.Value {  
	    
		//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
		return js_LoadFromFile(vm, call.Argument(0).String());
		    
		return otto.Value{} 
	});
	
	//--------------------------------------------------------
	//otto 居然是可以支持 module.exports 的，不过要先执行一次 module 初始化
	//var module={};
	//module.exports = { mail_error: mail_error, MailMime_errorMail_forTest: MailMime_errorMail_forTest }; //{hello,hello2}
	
	v, err := vm.Run("var module={};"); 
	
	if (nil != err) {
		fmt.Println(err);
	}else{
		fmt.Println(v);
	}//
	
	//--------------------------------------------------------
	
	//----
	//设置参数  //对 js 来说可以简单理解为 http 的 request ，本质上也相当于 webservice 本地化
	vm.Set( "js_request" , golang_param);
	
	//----
	
	
	//v, err := vm.Run(src1); 
	v, err = vm.Run(src1); 
	
	if (nil != err) {
		
		fmt.Println(err);
		
	}else{
		fmt.Println("js vm.Run() result=", v.String());  //这里得到的是 js 里的什么值？ //从 https://github.com/robertkrimen/otto 看
		//应该是脚本的返回值?
		//不对，实际上是最后一个表达式的值。所以最后 "return 1+1" 这样是不对的，应该直接是 "1+1"
		
	}//
	
	//vm.
	
	return vm;
	
	
	//--------------------------------------------------------

	
}//


//参考 https://blog.csdn.net/ALakers/article/details/111620603
//如果运行的脚本是一个库的话，还可以在 golang 里调用它的函数
// 使用vm.Call(函数名,nil,传递的参数,如果后面有多个参数，用逗号隔开就可以了)
// function withParamsFunc(x, K) {
// 	return 1;
// }
func main_js_nodejs_RunJS_CallFunction(vm * otto.Otto, func_name string, argumentList ...interface{}) string {
	
	defer PrintError("main_js_nodejs_RunJS_CallFunction");
	
	// 函数调用
	//value, _ := vm.Call("withParamsFunc", nil, "1313432", "32135")
	//fmt.Println("withParamsFunc value is:",value)
	
	//value, err := vm.Call(func_name, argumentList);
	value, err := vm.Call(func_name, nil, argumentList);  //上面的调用不对，第 2 个参数应该是 this //2022.11.12
	//fmt.Println("withParamsFunc value is:",value)
	
	if (nil != err) {
		
		fmt.Println(err);
		
		return "";		
	}else{
		fmt.Println("js vm.Run() result=", value.String());  //这里得到的是 js 里的什么值？ //从 https://github.com/robertkrimen/otto 看
		//应该是脚本的返回值?
		//不对，实际上是最后一个表达式的值。所以最后 "return 1+1" 这样是不对的，应该直接是 "1+1"
		
		return value.String();
	}//
	

}//
