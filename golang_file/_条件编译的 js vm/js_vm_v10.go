// +build !go1.18

// 在源码文件顶部添加注释，来决定文件是否参与编译
//golang版本号： 比如Go Version 1.1为go1.1,Go Version 1.12版本为go1.12，以此类推。
//注意：// +build的下一行必须是空行。
// 以空格分开表示AND
// 以逗号分开表示OR
// !表示NOT

//本文件的条件编译表示低于 go1.18 版本下使用本文件 //因为只能用 Otto 不能用 goja

package main

import (
	
	// 如果你使用自定义的syso文件则不要引用此包
	//_ "github.com/ying32/govcl/pkgs/winappres"

	//clq 说明。上面的文件参考 https://gitee.com/ying32/govcl/wikis/pages?doc_id=102420&sort_id=410058
	//应该只是用来加入 syso 资源文件的，参考 ..\vendor\github.com\ying32\govcl\pkgs\winappres 下的文件
	//如果对 ui 要求不高，不要其实也是可以的。
	//syso 文件是 google 的标准，其实只是原来修改 windows 下的程序的资源的。所以作者在注释中说明了只对 windows 程序生效。
	//----
	//应该也可以使用 ".manifest" 文件。
	//--------------------------------------------------------
	//高分屏的官方设置方法看两个地址
	//https://learn.microsoft.com/zh-cn/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process
	//https://learn.microsoft.com/zh-cn/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows?source=recommendations

	// "unsafe"

	//"github.com/ying32/govcl/vcl"
	// "github.com/ying32/govcl/vcl/types"
	//"github.com/dop251/goja"
	"github.com/robertkrimen/otto"

	// "encoding/json"
	"fmt"
	// "io/ioutil"
	// "os"
	// "os/exec"
	// "path/filepath"
)

//因为 goja 不能在 go 1.10 下使用，而 otto 因为不支持完整 js 正则表达式的原因不能使用 tsc 的 js 源码，因此有必要抽象出一个统一的接口
//以便在两者间切换，并且应该做成条件编译。

//或者做成象数据库那样，操作 api 是一样的，内部实现不同


type JS_VM struct {
	
	
	vm_otto * otto.Otto;
	
	//vm_goja * goja.Runtime;
	
	is_goja int;  //如果为 1 ，表示为 goja 引擎
	is_otto int;  //如果为 1 ，表示为 otto 引擎
	
	//按道理说两者应该只存在一个，不过如果是单元测试可以两个引擎同时运行
	
	
}//


// type JS_VM_ *JS_VM;


//方便性函数
//如果 use_goja 是 -1 表示在高版本中使用 goja ，低版本使用 otto
func CreateJsVM(use_goja int) (* JS_VM) {
	//_vm := (&JS_VM{}).CreateVM(0);
	_vm := (&JS_VM{}).CreateVM(use_goja);
	
	return _vm;
}



//相当于 vm := otto.New(); //golang 似乎没有静态函数的说法
//func (vm *JS_VM) CreateVM(use_goja int) (* JS_VM) {
//如果声明为结构体的指针的话会很神奇 //可以直接调用 
//可以写出这样的代码 _vm := JS_VM.CreateVM(JS_VM{}, 0); //很像 lua ，还要把自己再传递一次的参数形式
//golang 的术语称之为 “值方法和指针方法（Value Methods vs Pointer Methods）”
func (vm JS_VM) CreateVM(use_goja int) (* JS_VM) {

	_vm := &JS_VM{};

	_vm.is_goja = 0;
	_vm.is_otto = 1;
	
	_vm.is_goja = use_goja;
	if (1 == use_goja)  { _vm.is_otto = 0; }
	
	if (2 == use_goja)  { _vm.is_otto = 1; _vm.is_goja = 1; }  //测试的情况下可以两个引擎同时启动
	
	//if (-1 == use_goja) { _vm.is_otto = 0; _vm.is_goja = 1; }  //自动选择
	if (-1 == use_goja) { _vm.is_otto = 1; _vm.is_goja = 0; }  //自动选择
	
	if (1 == _vm.is_otto) {
		_vm.vm_otto = otto.New();
	}
	
	if (1 == _vm.is_goja) {
		//_vm.vm_goja = goja.New();
	}
	
	if (1 == use_goja) {
		fmt.Println("goja 在 golang 1.18 之后才支持!!!");
	}


	return _vm;

}//

//测试函数，不要用
// func (vm *JS_VM) CreateVM_(use_goja int) (* JS_VM) {

// 	_vm := &JS_VM{};

// 	_vm.is_goja = 0;
// 	_vm.is_goja = use_goja;
	
// 	_vm.vm_otto = otto.New();
	
// 	_vm.vm_goja = goja.New();


// 	return _vm;

// }//


//运行一个脚本（字符串，不是文件名，可多次重复调用）
//原意中本来有一个返回值，比较啰嗦，暂时不要了，以后可以统一为一个字符串，因为返回一个对象的话还要涉及到各种拆箱
func (vm *JS_VM) RunString(str string)(error) {
	
	
	var err error = nil;
	
	if (1 == vm.is_otto) {
		_, err = vm.vm_otto.Run(str);
	}//
	
	if (1 == vm.is_goja) {
		//_, err = vm.vm_goja.RunString(str);
	}//
	
	
	return err;
	
}//

//兼容 otto 的接口别名而已
func (vm *JS_VM) Run(str string)(error) {

	return vm.RunString(str);
	
}//

//设置一个 js 变量(其实也可以是函数)
func (vm *JS_VM) SetValue(name string, value interface{})(error) {
	
	
	var err error = nil;
	
	if (1 == vm.is_otto) {
		err = vm.vm_otto.Set(name, value);
	}//
	
	if (1 == vm.is_goja) {
		//err = vm.vm_goja.Set(name, value);
	}//
	
	
	return err;
	
}//

//兼容 otto 的接口别名而已
func (vm *JS_VM) Set(name string, value interface{})(error) {

	
	return vm.SetValue(name, value);
	
}//

//返回值难以兼容，尽量只在 require 中使用
func (vm *JS_VM) RunString_AndGetResult(str string)(interface{}, error) {
	
	
	var err error = nil;
	
	var result interface{} = nil;
	
	if (1 == vm.is_otto) {
		result, err = vm.vm_otto.Run(str);
	}//
	
	if (1 == vm.is_goja) {
		//result, err = vm.vm_goja.RunString(str);
	}//
	
	
	return result, err;
	
}//

//nodejs 中的 require 函数实现
func (vm *JS_VM)SetNodejsFunc_require() {
	
	if (1 == vm.is_otto) {
		
		//vcl.ShowMessage("vm.is_otto");
		fmt.Println("vm.is_otto");
	
		vm.vm_otto.Set( "require" , func(call otto.FunctionCall) otto.Value {  
		    
			//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
			//return js_require(vm, call.Argument(0).String());
			return js_nodejs_require_OTTO(vm.vm_otto, call.Argument(0).String());
			
			    
			return otto.Value{}; 
		});
		
	}//
	
	if (1 == vm.is_goja) {
		
		//vcl.ShowMessage("vm.is_goja");
		fmt.Println("vm.is_goja");
	
		// vm.vm_goja.Set( "require" , func(call goja.FunctionCall) goja.Value {  
		    
		// 	//fmt.Printf( "Hello, %s.\n" , call.Argument( 0 ). String ())  
		// 	//return js_require(vm, call.Argument(0).String());
		// 	return js_nodejs_require_GOJA(vm.vm_goja, call.Argument(0).String());
			    
		// 	// return struct{};
		// 	//return goja.Value{}; //不行，因为 goja.Value 是 interface
		// 	return goja.Undefined();
		// });
		
	}//
	
}//

//js 中 require 的简化实现 //目前只是运行，不返回 export //ok 已经可以返回值
//otto 版本
func js_nodejs_require_OTTO(vm * otto.Otto, fn string) (otto.Value) {
	
	fn = fn + ".js";
	
	r := otto.Value{};
	
	//--------------------------------------------------------
	//otto 居然是可以支持 module.exports 的，不过要先执行一次 module 初始化
	//var module={};
	//module.exports = { mail_error: mail_error, MailMime_errorMail_forTest: MailMime_errorMail_forTest }; //{hello,hello2}
	
	v, err := vm.Run("var module={};"); 
	
	if (nil != err) {
		fmt.Println("js_nodejs_require_OTTO()", err);
	}else{
		fmt.Println("js_nodejs_require_OTTO()", v);
	}//
	
	//--------------------------------------------------------
	
	//----
	//var src2 = LoadFromFile_String("t2.js");
	var src2 = LoadFromFile_String(fn);
	v, err = vm.Run(src2); 
	
	if (nil != err) {
		fmt.Println("js_nodejs_require_OTTO()", err);
	}else{
		fmt.Println("js_nodejs_require_OTTO()", v);
		
		return v;
	}//
	
	//----
	
	return r;
	
}//


//goja 版本
// func js_nodejs_require_GOJA(vm * goja.Runtime, fn string) (goja.Value) {
	
// 	fn = fn + ".js";
	
// 	//r := goja.Value{};//不行，因为 goja.Value 是 interface
// 	r := goja.Undefined();
	
// 	//--------------------------------------------------------
// 	//otto 居然是可以支持 module.exports 的，不过要先执行一次 module 初始化
// 	//var module={};
// 	//module.exports = { mail_error: mail_error, MailMime_errorMail_forTest: MailMime_errorMail_forTest }; //{hello,hello2}
	
// 	v, err := vm.RunString("var module={};"); 
	
// 	if (nil != err) {
// 		fmt.Println("js_nodejs_require_GOJA()", err);
// 	}else{
// 		fmt.Println("js_nodejs_require_GOJA()", v);
// 	}//
	
// 	//--------------------------------------------------------
	
// 	//----
// 	//var src2 = LoadFromFile_String("t2.js");
// 	var src2 = LoadFromFile_String(fn);
// 	v, err = vm.RunString(src2); 
	
// 	if (nil != err) {
// 		fmt.Println("js_nodejs_require_GOJA()", err);
// 	}else{
// 		fmt.Println("js_nodejs_require_GOJA()", v);
		
// 		return v;
// 	}//
	
// 	//----
	
// 	return r;
	
// }//

func js_nodejs_require(vm * JS_VM, fn string) (interface{}) {
	
	//var err error = nil;
	
	var result interface{} = nil;
	
	if (1 == vm.is_otto) {
		// result, err = vm.vm_otto.Run(str);
		result = js_nodejs_require_OTTO(vm.vm_otto, fn);
	}//
	
	if (1 == vm.is_goja) {
		// result, err = vm.vm_goja.RunString(str);
		//result = js_nodejs_require_GOJA(vm.vm_goja, fn);
	}//
	
	
	return result;	
	
}//



