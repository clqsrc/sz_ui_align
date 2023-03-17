

//这个其实是 golang 语法，不过道理一样的
//设置当前脚本的根 root_view //其实只是一个示例
func js_set_root_view_form(vm *otto.Otto, form * vcl.TForm){

	// vm.Set("root_view_form", form);

	js_set_var(vm, "root_view_form", form);
	
}//


