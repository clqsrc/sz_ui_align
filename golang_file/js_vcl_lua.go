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
	"encoding/json"

)

import "github.com/robertkrimen/otto"
//otto 用到了 https://github.com/go-sourcemap/sourcemap/tree/v1.0.5

//--------------------------------------------------------
//js_vcl.go>>js_vcl_lua.go

//原 lua 的接口函数

//对应 c_functions.lua/c_functions.js

//--------------------------------------------------------

// var g_cur_vm * otto.Otto; //按理说尽量只用一个 vm

//设置当前脚本的根 root_view //其实只是一个示例
func js_set_root_view_form(vm *otto.Otto, form * vcl.TForm){

	// vm.Set("root_view_form", form);

	js_set_var(vm, "root_view_form", form);
	
}//

func js_set_root_view(vm *otto.Otto, div * vcl.TPanel){

	// vm.Set("root_view", div);
	js_set_var(vm, "root_view", div);
	
}//

//没撤，golang otto 中是无法生成 key=object, value=string 的 map 的，只好还是 golang 中实现
var g_div_id_list = make(map[interface{}]string);
var g_div_list = make(map[string]interface{});
var g_div_list_cur_index int64 = 0;

//只能当做字符串的虚拟指针
// func NativeToVPoint(view vcl.IWinControl) string {
func NativeToVPoint(view interface{}) string {

	g_div_list_cur_index++;
	
	var id = IntToStr(g_div_list_cur_index);

	g_div_list[id] = view;
	g_div_id_list[view] = id;


	return id;

}//

// func VPointToNative(id string) (vcl.IWinControl) {
func VPointToNative(id string) (interface{}) {

	// g_div_list_cur_index++;
	
	// var id = IntToStr(g_div_list_cur_index);

	var view = g_div_list[id];
	//g_div_id_list[view] = id;


	return view;

}//

//func create_view(parentView * vcl.TPanel)(* vcl.TPanel){
//换成 IWinControl 就可以直接创建在 form 上了。注意 IWinControl 本身已经是指针了，不能再加 * 操作符
func create_view(parentView vcl.IWinControl)(* vcl.TPanel){

	div := vcl.NewPanel(parentView);
	div.SetParent(parentView);

	// div := vcl.NewPanel(mainForm);
	// div.SetParent(mainForm);


	div.SetBounds(0, 0, 100, 50);

	div.Show();

	return div;
}//

func UI_CreateViewClass(parentView * vcl.TPanel, ui_class string) (interface{}) {

	if (ui_class == "image") {

		div := vcl.NewImage(parentView);
		div.SetParent(parentView);
	
		div.SetBounds(0, 0, 100, 50);
	
		div.Show();
		return div;
	}//

	if (ui_class == "edit") {

		div := vcl.NewEdit(parentView);
		div.SetParent(parentView);
	
		div.SetBounds(0, 0, 100, 50);
	
		div.Show();
		return div;
	}//


	return create_view(parentView);

}//

func UI_Div_SetImage_forObj(uijson interface{}, view_obj * vcl.TImage, fn_image string){

	//要判断是否是程序下的目录
	view_obj.Picture().LoadFromFile(fn_image)

}//

func view_hide(div * vcl.TPanel){
	div.Hide();
}//

func view_show(div * vcl.TPanel){
	div.Show();
}//

//简化的解码
func DecodeJson_toKV(s string) (map[string]string) {

	// defer PrintError("DecodeJson_toKV"); //应该不用

	//line := make(map[string]interface{}) //居然是可以解码到 map 中的
	//var line interface{};// :=  make(interface{}); //居然是可以解码到 interface{} 中!

	line_list := make(map[string]string); //这是一个 map 数组

	var buf []byte = []byte(s);

	err := json.Unmarshal(buf, &line_list) //居然是可以解码到 map 中的
	if err != nil {
		fmt.Println("error: DecodeJson_toKV() ", err)
	}
	////fmt.Println(line);

	return line_list;
} //

//简化的解码 //rect 的字段是整数时 golang 不能自动转换成字符串，所以单独用一个临时用好了
func DecodeJson_toKV_rect(s string) (map[string]float64) {

	// defer PrintError("DecodeJson_toKV"); //应该不用

	//line := make(map[string]interface{}) //居然是可以解码到 map 中的
	//var line interface{};// :=  make(interface{}); //居然是可以解码到 interface{} 中!

	line_list := make(map[string]float64); //这是一个 map 数组

	var buf []byte = []byte(s);

	err := json.Unmarshal(buf, &line_list) //居然是可以解码到 map 中的
	if err != nil {
		fmt.Println("error: DecodeJson_toKV_rect() ", err)
	}
	////fmt.Println(line);

	return line_list;
} //

func view_setrect(view * vcl.TPanel, rect string){

	kv := DecodeJson_toKV(rect);

	s_left := kv["left"];
	s_height := kv["height"];
	s_top := kv["top"];
	s_width := kv["width"];

    left := StrToIntDef_(s_left, 0);
    top := StrToIntDef_(s_top, 0);
    width := StrToIntDef_(s_width, 0);
    height := StrToIntDef_(s_height, 0);
	

	view.SetBounds(int32(left), int32(top), int32(width), int32(height));

}//


func view_setrect_float(view * vcl.TPanel, rect string){

	kv := DecodeJson_toKV_rect(rect);

	left := kv["left"];
	height := kv["height"];
	top := kv["top"];
	width := kv["width"];

    // left := StrToIntDef_(s_left, 0);
    // top := StrToIntDef_(s_top, 0);
    // width := StrToIntDef_(s_width, 0);
    // height := StrToIntDef_(s_height, 0);
	

	view.SetBounds(int32(left), int32(top), int32(width), int32(height));

}//



//取 edit 内容
func View_Edit_GetText(edit * vcl.TEdit) string {

	return edit.Text();

}//

func View_Edit_SetText(edit * vcl.TEdit, text string){

	edit.SetText(text);

}//



//注册全部的 vcl 函数 //正式的功能函数
func js_vcl_lua_reg_functions(vm * otto.Otto){

	//_, _  = vm.Object(`vcl = {}`);
	_, _  = vm.Object(`lua = {}`);


	// js_set_var(vm, "nil", nil);
	// run_js_src(vm, "vcl.nil = nil;"); //倒是可以用 js 来设置属性。

	// js_set_var(vm, "tmp_func", vcl.NewPanel);
	// run_js_src(vm, "vcl.NewPanel = tmp_func;"); //倒是可以用 js 来设置属性。

	// js_set_var(vm, "tmp_func", vcl.ShowMessage);
	// run_js_src(vm, "vcl.ShowMessage = tmp_func;"); //倒是可以用 js 来设置属性。
	
	// run_js_src(vm, "vcl.vcl_NewPanel();"); //倒是可以用 js 来设置属性。//ok 这时候成功传递进来了
	
	// run_js_src(vm, "console.vcl = vcl_NewPanel;"); //倒是可以用 js 来设置属性。
	
	// run_js_src(vm, "console.vcl(nil); console.log('console.vcl'); console.log(nil);")
	// run_js_src(vm, "console.log(console.vcl);")
	// run_js_src(vm, "vcl; console.log('vcl');");

	//--------------------------------------------------------
	js_set_var(vm, "create_view", create_view);
	js_set_var(vm, "UI_CreateViewClass", UI_CreateViewClass);
	js_set_var(vm, "UI_Div_SetImage_forObj", UI_Div_SetImage_forObj);
	js_set_var(vm, "view_hide", view_hide);
	js_set_var(vm, "view_show", view_show);
	js_set_var(vm, "view_setrect", view_setrect);
	js_set_var(vm, "view_setrect_float", view_setrect_float);
	js_set_var(vm, "View_Edit_GetText", View_Edit_GetText);
	js_set_var(vm, "View_Edit_SetText", View_Edit_SetText);

	js_set_var(vm, "UI_SetOnClick", UI_SetOnClick);


	//虚拟指针的函数
	//只能当做字符串的虚拟指针
	js_set_var(vm, "NativeToVPoint", NativeToVPoint);
	js_set_var(vm, "VPointToNative", VPointToNative);
	
	js_set_var(vm, "vcl_ShowMessage", vcl.ShowMessage);


}//


//一定要将 vm 的虚拟指针告诉脚本引擎，因为和 lua 版本不同，我们这个环境是支持同时运行多个相互隔离的虚拟 app 的
//每个脚本只能运行一次
func RunScriptStartVM(vm * otto.Otto){


	var vm_vpoint string = NativeToVPoint(vm);

	run_js_src(vm, "on_start_vm(" + vm_vpoint + ");");

}//

//原 lua 接口下的单击事件都过这一个函数  //不行，直接用 view.SetOnClick 设置到这个函数的话是无法通过 sender 知道是哪个控件触发的
// func OnClick_for_lua(sender vcl.IObject){

// 	//run_js_src(g_cur_vm, "vcl; console.log('vcl');");
// 	js_set_var(g_cur_vm, "tmp_cur_sender", sender);
// 	run_js_src(g_cur_vm, "UI_OnClick(tmp_cur_sender);");

	

// }//

//这会在事件发生时调用 lua_functions_event.lua 中的 UI_OnClick 函数，在里面会再转换出 obj ，所以不要直接调用这个函数，而是要调用 ui_json:SetOnClick 来间接使用
func UI_SetOnClick(view * vcl.TPanel, vm_vpoint string, _func string) {

	//view.SetOnClick(OnClick_for_lua); //不行，直接用 view.SetOnClick 设置到这个函数的话是无法通过 sender 知道是哪个控件触发的

	//不过因为这里已经是 native 函数体，所以多写几个事件函数实例是没关系的,而且一个 app 中的事件也是有限的


	var vm * otto.Otto = VPointToNative(vm_vpoint).(* otto.Otto);

	view.SetOnClick(func(sender vcl.IObject) {

		vcl.ShowMessage("UI_SetOnClick() _func: " + _func);

		run_js_src(vm, _func);  //直接执行一段 js 脚本最简单 //lua 里反而没有直接执行脚本的方便方法

	});

    //return c2l_UI_SetOnClick(view, func);
	//return "0";

}//




