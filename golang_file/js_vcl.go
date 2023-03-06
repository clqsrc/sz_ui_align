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


	"github.com/ying32/govcl/vcl"
	// "github.com/ying32/govcl/vcl/types"



	// "io/ioutil"
	"fmt"
	// "path/filepath"
	// "os"
	// "os/exec"
	// "encoding/json"

)

import "github.com/robertkrimen/otto"
//otto 用到了 https://github.com/go-sourcemap/sourcemap/tree/v1.0.5

func js_main111(form * vcl.TForm){
	
}//

//有错误提示的运行 js 脚本
func run_js_src(vm *otto.Otto, js_src string){
	

	//_, err := vm.Run("vcl_NewPanel(0); console.log('vcl');");  //直接运行脚本
	_, err := vm.Run(js_src);  //直接运行脚本
	
	if (nil != err) {
		
		fmt.Println("run_js_src():", err);
		
	}//	
	
	
	//return;
	
	
}//


//有错误提示的运行 js 脚本
func run_js_src_file(vm *otto.Otto, fn string){
	
	defer PrintError("run_js_src_file()");
	
	js_src := LoadFromFile_String(fn);

	//_, err := vm.Run("vcl_NewPanel(0); console.log('vcl');");  //直接运行脚本
	_, err := vm.Run(js_src);  //直接运行脚本
	
	if (nil != err) {
		
		fmt.Println("run_js_src():", err);
		
	}//	
	
	
	//return;
	
	
}//


//有错误提示的运行 js 脚本 //设置一个变量
func js_set_var(vm *otto.Otto, name string, value interface{}){
	
	//err := vm.Set( "vcl" , js_vcl); 
	err := vm.Set(name, value); 
	if (nil != err) {
		
		fmt.Println("js_set_var():", err);
		
	}//

}//


//注册全部的 vcl 函数 //正式的功能函数
func js_vcl_reg_functions(vm * otto.Otto){

	//参考 func JS_SetDocument //设置一个名为 “vcl” 的全局变量
	//var g_jsCanvas * otto.Object, _  = vm.Object(`document = {}`);
	////var g_jsCanvas * otto.Object;
	//g_jsCanvas, _  = vm.Object(`vcl = {}`);
	_, _  = vm.Object(`vcl = {}`);


	js_set_var(vm, "nil", nil);
	run_js_src(vm, "vcl.nil = nil;"); //倒是可以用 js 来设置属性。

	js_set_var(vm, "tmp_func", vcl.NewPanel);
	run_js_src(vm, "vcl.NewPanel = tmp_func;"); //倒是可以用 js 来设置属性。

	js_set_var(vm, "tmp_func", vcl.ShowMessage);
	run_js_src(vm, "vcl.ShowMessage = tmp_func;"); //倒是可以用 js 来设置属性。
	
	run_js_src(vm, "vcl.vcl_NewPanel();"); //倒是可以用 js 来设置属性。//ok 这时候成功传递进来了
	
	run_js_src(vm, "console.vcl = vcl_NewPanel;"); //倒是可以用 js 来设置属性。
	
	run_js_src(vm, "console.vcl(nil); console.log('console.vcl'); console.log(nil);")
	run_js_src(vm, "console.log(console.vcl);")
	run_js_src(vm, "vcl; console.log('vcl');");


}//



