package main

//import lua "github.com/yuin/gopher-lua"

import (
	"github.com/robertkrimen/otto"
	"github.com/ying32/govcl/vcl"
	//"github.com/dop251/goja"
)
//otto 用到了 https://github.com/go-sourcemap/sourcemap/tree/v1.0.5

import (
	
	"fmt"
	"github.com/robertkrimen/otto/parser"
	"regexp"
	"github.com/dlclark/regexp2"
)

//纯 js 版本的 tsc


//go_js_main_nodejs.go>>go_js_main_tsc.go

//参考 Motto提供了类似于Nodejs的模块环境，可以在golang中运行javascript文件。基于 otto
//https://github.com/ddliu/motto
//https://github.com/ddliu/nodego



func js_main_t1_tsc() {
	
	var src1 = LoadFromFile_String("typescript.js");
	
	//--------------------------------------------------------
	//下面是 js 脚本
	fmt.Println("-----------------------------------------\r\n js ...");
	
	// vm := otto.New();
	// vm.Run(`
		
	// 	//不支持 typescript //也不支持 es6  //不过可以先用 typescript 的 tsc 先转换即可
	// 	//const hello : string = "Hello World!"
	// 	//console.log(hello)

	//     abc = 2 + 2;
	//     console.log("The value of abc is " + abc); // 4
	// `);
	
	//https://github.com/go-git/go-git
	//--------
	//原生的确实不行
	email := "/a/b/c/?/d"; //\a\b\c\?\d
	if email != "" {
		if isOk, err := regexp.MatchString(`^[^*?]*(?=\/[^/]*[*?])`, email); isOk {
			//return true
			vcl.ShowMessage("ok");
		}else{
			vcl.ShowMessage("no:" + err.Error());
		}
	}
	
	//----
	//第三方库似乎可以
	// https://github.com/dlclark/regexp2
	//介绍来自 https://blog.csdn.net/dianxin113/article/details/118769094
	// re := regexp2.MustCompile(`Your pattern`, 0)
	// if isMatch, _ := re.MatchString(`Something to match`); isMatch {
	//     //do something
	// 	vcl.ShowMessage("ok");
	// }
	
	
	re := regexp2.MustCompile(`^[^*?]*(?=\/[^/]*[*?])`, 0)
	if isMatch, _ := re.MatchString(email); isMatch {
	    //do something
		vcl.ShowMessage("ok");
		
	}
	
	//regexp2.MatchString
	
	//----

	//42940 行处的的正则表达式错误
	// var st2 = `
	// var wildcardDirectoryPattern = /^[^*?]*(?=\/[^/]*[*?])/;
	// `;
	// v2, err2 := vm.Run(st2); 
	
	// if (nil != err2) {
		
	// 	fmt.Println(err2);
		
	// }else{
	// 	fmt.Println(v2);
		
	// }//

	//--------
	// v, err := vm.Run(src1); 
	
	// if (nil != err) {
		
	// 	fmt.Println(err);
		
	// }else{
	// 	fmt.Println(v);
		
	// }//
	
	//--------
	//go1.18 可以打开 goja 编译选项	
	ts_src := `
	//使用方法来自 https://www.likecs.com/ask-8906941.html#sc=1144
            //这里有另外一种方法，也是比较简单的，似乎更官方 https://github.com/basarat/typescript-script
            //const tsCode = 'let num: number = 123;';
            const tsCode = 'class crect{ left:string="aaa"; }; let num: number = 123;';
            //const jsCode = window.ts.transpile(tsCode);
            const jsCode = ts.transpile(tsCode);  //相关函数可参考 index_tsc_typescript.d.ts ，其直接来自 https://raw.githubusercontent.com/microsoft/TypeScript/main/lib/typescript.d.ts
            //document.write(jsCode);
            
            //console.log(jsCode); //goja 和 otto 不同，默认没有这个函数
            vcl_ShowMessage(jsCode);
            
	`;
	/*
	vcl.ShowMessage("goja")
	vm2 := goja.New();
	
	// v, err = vm2.Run(src1); 
	v3, err3 := vm2.RunString(src1); 
	
	if (nil != err3) {
		
		fmt.Println("goja err: ",err3);
		
	}else{
		fmt.Println("goja: ", v3);
		
	}//
	
	fmt.Println("--------------------------------------------------------", vm2);
	
	//goja 需要 go1.16 主要是因为没有 "hash/maphash"
	//实测确实直接支持 ts 了。另外据说 babel 也是可以完美支持的，可以通过它来再支持 es6。参考 https://blog.51cto.com/rongfengliang/3121345
	//理论上说 babel 也是支持 ts 编译为 js 的。参考 https://www.51cto.com/article/706106.html
	
	vm2.Set("vcl_ShowMessage", vcl.ShowMessage);
	

	
	v3, err3 = vm2.RunString(ts_src); 
	
	if (nil != err3) {
		
		fmt.Println("goja err: ",err3);
		
	}else{
		fmt.Println("goja: ", v3);
		
	}//
	
	fmt.Println("--------------------------------------------------------", vm);
	*/
	//--------------------------------------------------------
	//用抽象 vm 接口再跑一次
	//_vm := (&JS_VM{}).CreateVM(0);
	//JS_VM_.CreateVM(0);
	_vm := JS_VM.CreateVM(JS_VM{}, 0);
	vm3 := _vm.CreateVM(0);
	// vm3 := (new JS_VM()).CreateVM(0);
	vm3.RunString(src1);
	
	vm3.SetValue("vcl_ShowMessage", vcl.ShowMessage);
	vm3.RunString(ts_src); 
	vm3.RunString("vcl_ShowMessage('aaa')");  //按道理应该弹出两次，正式切换引擎后就一次了
	
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
func main_js_nodejs_RunJSaaa(fn string, golang_param interface{}) (*otto.Otto) {
	
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

