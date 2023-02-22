
package main

//windows 下的 gui dll 的直接使用，不需要 cgo

/*

难点除了一开始对 golang 如何实现 c 接口的 dll 函数（函数原型写法，字符串如何传递等等）比较头痛外。
后面的难点其实是查找到各个 gtk 函数是在哪个 h 文件中的，原始的使用方法又是什么。

这里要特别说明的是 gtk 使用的函数并不只局限于 gtk/gdk 这些本身，实际上还有大量的 gnu 库也用到了。
比如 D:\new\msys64\mingw64\include\glib-2.0\gobject\gsignal.h 中的信号函数。

另外还要知道 gtk 中使用的可能是宏，所以有时候要从一个 dll 中加载一个函数无论如何是不行的，这时只能去 h 头文件
中找宏实现，自己实现这个宏，例如 "g_signal_connect" 函数就是宏。所以在写 gtk 接口时必须有一份 gtk 的 c 语言头文件
接口在手上。所以我建议开发时将以下两个目录加入工程，以便查找函数原型的实现位置。
D:\new\msys64\mingw64\include
D:\new\msys64\mingw64\include\gtk-3.0

另外项目中还附带了 \3rd\GtkSharp-develop
这是 gtk 在 c# 中的实现，也是用的 dll 所以可参考性很高。
另外一个接口实在不知道在 golang 中如何实现的话建议阅读 govcl 的源码，它也是在 golang 中通过 dll 实现 ui 库的做法。
可参考性非常高，比如对于 c 语言回调函数在 golang 中的实现，我们一直找不到办法，后面是参考了它的实现，才知道 golang
有直接的函数用于转换函数成为 c 语言的回调函数，也算是对 golang 加深了理解。


*/

import (
	"fmt"
	"os"
	"path/filepath"
	"io/ioutil"
	//"net/http"
	//"html/template"
	"log"
	//"strings"
	//"reflect"
	//"runtime"
	//"time"
	//"mime"
	//"runtime/debug"
	//--------
	// "os/exec"
	//"math/rand"
	// "math"

	//--------
	//参考的库
	// github.com/lxn/win

	//--------

	"syscall"

	"unsafe"
)


//这是直接加载 dll ，如果 dll 不存在会有问题
// var kernel32, _        = syscall.LoadLibrary("kernel32.dll");
// var getModuleHandle, _ = syscall.GetProcAddress(kernel32, "GetModuleHandleW");

// var user32, _          = syscall.LoadLibrary("user32.dll");
// var messageBox, _      = syscall.GetProcAddress(user32, "MessageBoxW");
// //createWindowEx = libuser32.NewProc("CreateWindowExW")
// var createWindowEx, _  = syscall.GetProcAddress(user32, "CreateWindowExW");

//var libgtk, _          = syscall.LoadLibrary("libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll 

var libgtk syscall.Handle;

//--------------------------------------------------------
//下面是目前正式使用的 gtk 库
var libglib * syscall.DLL;
// libglib, err = syscall.LoadDLL("libglib-2.0-0.dll");

var libgtk2 * syscall.DLL;
// libgtk2, err = syscall.LoadDLL("libgtk-3-0.dll");

var libgio * syscall.DLL;
//libgio, err = syscall.LoadDLL("libgio-2.0-0.dll");

var libgobject * syscall.DLL;
// libgobject, err = syscall.LoadDLL("libgobject-2.0-0.dll");

//--------------------------------------------------------

//gtk 库所在的路径
//var gtk_path = "F:/new/msys64/mingw64/bin";
var gtk_path = "d:/new/msys64/mingw64/bin";

func gtk_test1() {

	//defer syscall.FreeLibrary(kernel32);
	//defer syscall.FreeLibrary(user32);

	//----

	fmt.Println("ok gtk_test1.");

	var err error;

	libgtk, err = syscall.LoadLibrary("libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll
	
	fmt.Println("1:", libgtk, err);

	//libgtk, err = syscall.LoadLibrary("F:/new/msys64/mingw64/bin/libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll
	libgtk, err = syscall.LoadLibrary(gtk_path + "/bin/libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll

	fmt.Println("2:", libgtk, err);

	//绝对路径也不行，估计要修改工作路径
	pwd, _ := os.Getwd();
	fmt.Println("开始工作目录", pwd);
	// 程序所在目录
	execDir, err := filepath.Abs(filepath.Dir(os.Args[0]));
	if err != nil {
		log.Fatal(err)
	}

	if pwd == execDir {
		fmt.Println("不需要切换工作目录")
		//return
	}

	fmt.Println("切换工作目录到", execDir);

	if err := os.Chdir(execDir); err != nil {
		log.Fatal(err)
	}

	//dll_path := "F:/new/msys64/mingw64/bin";
	dll_path := gtk_path;
	if err := os.Chdir(dll_path); err != nil {
		log.Fatal(err)
	}

	pwd, _ = os.Getwd()
	fmt.Println("切换后工作目录:", pwd)
	
	//----------------------------------------------------------------
	//切换目录后确实可以了
	//MessageBox_gtk("c", "t中文", 0);
	//libgtk, err = syscall.LoadLibrary("F:/new/msys64/mingw64/bin/libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll
	libgtk, err = syscall.LoadLibrary(gtk_path + "/bin/libgtk-3-0.dll");  //F:\new\msys64\mingw64\bin\ 中的 dll

	fmt.Println("3:", libgtk, err);

	var gtk_application_new uintptr;
	gtk_application_new, err  = syscall.GetProcAddress(libgtk, "gtk_application_new");
	fmt.Println("4:", gtk_application_new, err);

	//--------

	//MessageBoxW, _ := libgtk.
    //MessageBoxW.Call(IntPtr(0), StrPtr(text), StrPtr(title), IntPtr(0))

	//var libgtk2 * syscall.DLL;

	libgtk2, err = syscall.LoadDLL("libgtk-3-0.dll");

	fmt.Println("5:", libgtk2, err);

    var gtk_application_new2 * syscall.Proc;

    gtk_application_new2, err = libgtk2.FindProc("gtk_application_new");

	fmt.Println("6:", gtk_application_new2, err);

	//"org.gtk.example", 
	//G_APPLICATION_FLAGS_NONE := 0;

    //gtk_application_new2.Call(StrPtr("org.gtk.example"), IntPtr(G_APPLICATION_FLAGS_NONE));
    //gtk_application_new2.Call(C.CString("org.gtk.example"), IntPtr(G_APPLICATION_FLAGS_NONE));
	////gtk_application_new2.Call(StrPtr_c("org.gtk.example\x00"), IntPtr(0));

	/*
	appid := "org.gtk.example\x00";
	buf := []byte(appid);
	p := &buf[0];
	//p := &buf;

	//&StringToUTF16(s)[0] 

	r := unsafe.Pointer(p);
	r2 := uintptr(r);
	*/
	////app,_,_ := gtk_application_new2.Call(r2, IntPtr(0));
	app,_,_ := gtk_application_new2.Call(g_id.c_str, IntPtr(0));

	fmt.Println("7:", app);

	//C.CString("go")


	//----
	//var libglib * syscall.DLL;

	libglib, err = syscall.LoadDLL("libglib-2.0-0.dll");

	fmt.Println("7.5:", libglib, err);

	//var libgio * syscall.DLL;
	libgio, err = syscall.LoadDLL("libgio-2.0-0.dll");

	fmt.Println("7.6:", libgio, err);

	// libgobject-2.0-0.dll
	//var libgobject * syscall.DLL;
	libgobject, err = syscall.LoadDLL("libgobject-2.0-0.dll");

	fmt.Println("7.7:", libgobject, err);


	//--------------------------------------------------------
	///实现 // 3.将信号activate与其处理函数进行绑定 //一定要在 run 之前绑定
	//g_signal_connect(app,"activate",G_CALLBACK(activateMe),NULL);

	var g_signal_connect * syscall.Proc;
    g_signal_connect, err = libgtk2.FindProc("g_signal_connect");
	fmt.Println("6.1:", g_signal_connect, err);

    g_signal_connect, err = libglib.FindProc("g_signal_connect");
	fmt.Println("6.1:", g_signal_connect, err);

    g_signal_connect, err = libgio.FindProc("g_signal_connect");
	fmt.Println("6.1:", g_signal_connect, err);

    g_signal_connect, err = libgobject.FindProc("g_signal_connect");
	fmt.Println("6.1:", g_signal_connect, err);


	find_gtk_func("g_signal_connect"); //没有这个函数，实际上是 g_signal_connect_closure

	//不过在 gsignal.h 中的宏有如下的定义 //D:\new\msys64\mingw64\include\glib-2.0\gobject\gsignal.h
	//#define g_signal_connect(instance, detailed_signal, c_handler, data) \
    // g_signal_connect_data ((instance), (detailed_signal), (c_handler), (data), NULL, (GConnectFlags) 0)


	var g_signal_connect_closure * syscall.Proc;
	g_signal_connect_closure, err = libgobject.FindProc("g_signal_connect_closure");
	fmt.Println("6.1:", g_signal_connect_closure, err);

	var g_signal_connect_data * syscall.Proc;
	g_signal_connect_data, err = libgobject.FindProc("g_signal_connect_data");
	fmt.Println("6.1:", g_signal_connect_data, err);

	//typedef void* gpointer;

	//gulong
	// g_signal_connect_data (gpointer       instance,//发送信号的对象
	// 	const gchar   *detailed_signal,//信号名称
	// 	GCallback      c_handler,//回调函数
	// 	gpointer       data,//传递给回调函数的参数
	// 	GClosureNotify destroy_data,//销毁数据的函数
	// 	GConnectFlags  connect_flags)//信号连接的选项

	g_signal_connect_data.Call(
		app,
		to_c_str("activate\x00").c_str, //"activate", //g_id.c_str, 
		syscall.NewCallbackCDecl(gtk_on_app_activate), //1, //0, //G_CALLBACK(activateMe), //这个是回调函数，如果给 0 的话会提示错误，如果给 1 会崩溃，因为函数地址错误
		123, //IntPtr(0), //NULL  // 	gpointer       data,//传递给回调函数的参数
	
		IntPtr(0), //NULL  //g_signal_connect 时传入的默认值
		IntPtr(0), //0     //g_signal_connect 时传入的默认值
	);

	fmt.Println("7:", app);

	//--------------------------------------------------------	

	var g_application_run * syscall.Proc;
	// g_application_run, err = libgtk2.FindProc("g_application_run");
	// g_application_run, err = libglib.FindProc("g_application_run");
	g_application_run, err = libgio.FindProc("g_application_run");

	fmt.Println("8:", g_application_run, err);

	g_application_run.Call(app, 0, 0);

	//--------
	//user32 := syscall.NewLazyDLL("libgtk-3-0.dll");
    //MessageBoxW := user32.NewProc("MessageBoxW")
    //MessageBoxW.Call(IntPtr(0), StrPtr(text), StrPtr(title), IntPtr(0))


}//

// func StrPtr(s string) uintptr {
//     return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)));
// }//

func StrPtr_c(s string) uintptr {
    //return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)));

	buf := []byte(s);

	p := &buf[0];
	//p := &buf;

	//&StringToUTF16(s)[0] 

	//return p;

	r := unsafe.Pointer(p);

	return uintptr(r);  //失败，大概是 buf 会被回收
}//

//避免 gc 回收
type cstring struct {
	s string;
    buf []byte;
    
	c_str uintptr; //最后的 c 语言 dll 导出函数中需要的 char * 指针
    
}//

var g_id = to_c_str("org.gtk.example\x00");

func to_c_str(s string) cstring {
    //return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)));

	var r cstring;

	r.buf = []byte(s);

	p := &r.buf[0];
	//p := &buf;

	//&StringToUTF16(s)[0] 

	//return p;

	_r := unsafe.Pointer(p);

	r.c_str = uintptr(_r);   //这个只是把指针值强制转换成整数而已，并不涉及数据的转换

	return r;  
}//

func MessageBox_gtk(caption, text string, style uintptr) (result int) {

	gtk_application_new, _ := syscall.GetProcAddress(libgtk, "MessageBoxW");



	//----------------------------------------------------------------

	var nargs uintptr = 4;
	
	ret, _, callErr := syscall.Syscall9(uintptr(gtk_application_new),
	
	nargs, //第二个参数, nargs 即参数的个数,一旦传错, 轻则调用失败,重者直接APPCARSH //多余的参数, 用0代替
	
	//后面的才是 messageBox 自身的参数。一共是 4 个
	0,
	
	uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(text))),
	
	uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(caption))),
	
	style,
	
	0,
	
	0,
	
	0,
	
	0,
	
	0);
	
	if callErr != 0 {
	
	abort("Call MessageBox", callErr)
	
	}
	
	result = int(ret)
	
	return;  //这样返回也是奇怪
	//return result;
	
}//


//在所有 dll 中查找函数
func find_gtk_func(func_name string)  {

	fn_list := File_ListAllFile_ext(gtk_path, "dll");

	for i, v := range fn_list {
		

		//fmt.Println(i, "=", v.Name());
		fmt.Println(i, "=", v);

		//----

		var err error;

		var libgobject * syscall.DLL;
		// libgobject, err = syscall.LoadDLL("libgobject-2.0-0.dll");
		libgobject, err = syscall.LoadDLL(v);
	
		fmt.Println("7.7:", libgobject, err);

		var g_signal_connect * syscall.Proc;

		if (nil == libgobject) { continue; }

		g_signal_connect, err = libgobject.FindProc("g_signal_connect");     //dll 中没有这个函数？
		//实际上是 g_signal_connect_closure 函数？ 从  SignalClosure.cs 看
		//D:\gopath1.18.3\src\_src_golang_dll_gtk\3rd\GtkSharp-develop\Source\Libs\GLibSharp

		g_signal_connect, err = libgobject.FindProc("g_signal_connect_closure"); 
		//g_signal_connect, err = libgobject.FindProc("gtk_application_new");
		fmt.Println("6.1:", g_signal_connect, err);

		if (nil == err) { 
			fmt.Println("7.7: find ok!", libgobject, err);
		 }//if 

	}//for

	// g_signal_connect, err = libgio.FindProc("g_signal_connect");
	// fmt.Println("6.1:", g_signal_connect, err);

    // g_signal_connect, err = libgobject.FindProc("g_signal_connect");
	// fmt.Println("6.1:", g_signal_connect, err);
}//


//列出全部文件 //指定后缀名的
//func XmppminiGroup_ListAllUser(gname string) (interface{}) {
func File_ListAllFile_ext(path string, ext string) ([]string) {
	

	//userlist := make(map[string]interface{});
	//userlist := make([]interface{}, 0, 1);

	
	fileList, e := ioutil.ReadDir(path); //这个的结果是按文件名排序的,所以生成文件名时最好有序
	if (e != nil) { return nil; }
	
	//userlist := make([]interface{}, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的
	userlist := make([]string, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的

	//userlist := make(map[string]interface{});
	
	
	for i, v := range fileList {
		
		//if (i < curPos) { continue; };//分页的跳过机制
		//if (i >= endPos) { continue; };//分页的跳过机制
		
		fmt.Println(i, "=", v.Name())
		
		// uname := v.Name();	
		//uname = MakeUserName(uname);

		//fn := path + "\\" + uname + "\\user_info.txt";
		fn := v.Name();
		
		// kv := make(map[string]string);
		
		//kv[fn] = ToString(v.IsDir());
		
		//stop := StrToIntDef_(kv["stop"], 0);

		
		//----
		
		//userlist[i] = kv;
		//fi = append(fi, f)
		//userlist = append(userlist, kv);
		userlist = append(userlist, fn);
	
	}//for
	
	// kv := make(map[string]string);
	// kv["aaa"] = "bbb";
	
	// userlist = append(userlist, kv);
	
	return userlist;
	
}//

//--------------------------------------------------------
// static void
// activate (GtkApplication* app,
//           gpointer        user_data)
// {
//   GtkWidget *window;

//   window = gtk_application_window_new (app);
//   gtk_window_set_title (GTK_WINDOW (window), "Window");
//   gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
//   gtk_widget_show_all (window);
// }
	

//golang 似乎没有 static 静态函数的概念 
//gtk_on_app_activate (app uintptr, user_data uintptr)
//从 govcl 的注释来看 syscall.NewCallbackCDecl 的个数是有限制的，所以回调函数在程序中应该尽量共用
//不过参数的类型又要和原型是一致的，所以不同类型的还是得至少定义一个。所以就不可能程序中只有一个回调函数，
//至少也得是每种类型一个。所以就接口的设计也就是有要求，要尽量统一，并且能区分发起者。
//参考 https://github.com/golang/go/blob/master/src/runtime/syscall_windows.go 
//中的 func compileCallback(fn eface, cdecl bool) (code uintptr) 函数实现前的注释。
func gtk_on_app_activate (app uintptr, user_data uintptr) (uintptr) {

//   GtkWidget *window;

//   window = gtk_application_window_new (app);
//   gtk_window_set_title (GTK_WINDOW (window), "Window");
//   gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
//   gtk_widget_show_all (window);

	var err error;

	var gtk_application_window_new * syscall.Proc;
	var gtk_widget_show_all * syscall.Proc;
	var gtk_window_set_title * syscall.Proc;

	//--
	gtk_application_window_new, err = libgtk2.FindProc("gtk_application_window_new");
	fmt.Println("8:", gtk_application_window_new, err);

	window,_,_ := gtk_application_window_new.Call(app);

	fmt.Println("8:", window);

	//-- //这个是设置标题，不有也是可以的
	gtk_window_set_title, err = libgtk2.FindProc("gtk_window_set_title");
	fmt.Println("8:", gtk_window_set_title, err);

	//gtk_window_set_title.Call(window, to_c_str("12345").c_str);
	gtk_window_set_title.Call(window, to_c_str("12345").c_str); //"\x00" 放这个在最后更安全一点

	fmt.Println("8:", window);

	//--
	gtk_widget_show_all, err = libgtk2.FindProc("gtk_widget_show_all");
	fmt.Println("8:", gtk_widget_show_all, err);

	gtk_widget_show_all.Call(window);

	fmt.Println("8:", window);


    //确实成功了， user_data 中的整数也正确
	fmt.Println("gtk_on_app_activate() event!!!", user_data);

	return 0;  // 如果本函数没有  (uintptr) 的返回类型的话，会报 compileCallback: expected function with one uintptr-sized result

}//


//--------------------------------------------------------