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

//--------------------------------------------------------
//将 vcl 的函数都注入到 js 环境中。因为 api 有可能变动，所以要每个版本一个这个函数文件

func js_new_form(form * vcl.TForm){
	
	form_ := vcl.NewForm(nil);
	
	
	form_.ShowModal();
	
}//

//有错误提示的运行 js 脚本
func run_js_src222(vm *otto.Otto, js_src string){
	

	//_, err := vm.Run("vcl_NewPanel(0); console.log('vcl');");  //直接运行脚本
	_, err := vm.Run(js_src);  //直接运行脚本
	
	if (nil != err) {
		
		fmt.Println(err);
		
	}//	
	
	
	//return;
	
	
}//



