



#ifdef __cplusplus
extern "C" {
#endif


//一定要加 export 在函数名前，才会导出这个函数
//extern GoInt Sum(GoInt a, GoInt b);
//extern char* run_golang_json(const char* src_json);
//extern void free_golang_cstr(const char* golang_c_str);

//下面是函数指针的形式
extern char* (*_run_golang_json)(const char* src_json);
extern void (*_free_golang_cstr)(const char* golang_c_str);

//创建一个 tcp 连接
extern int  (*create_tcp_connect)();
extern void (*free_tcp_connect)(int i_client);

extern int (*tcp_connect)(int i_client, const char * host, int port, int use_ssl);

extern void (*tcp_send_string)(int i_client, const char *  line);

//使用终止分隔符的 //返回值要用 _free_golang_cstr 释放
//另外参数 sp 是终止符字符串，但其实只有第一个字符起使用,因为 golang 原始版本即是如此，可以自己再封装
extern char* (*tcp_recv_string)(int i_client, const char *  sp);

#ifdef __cplusplus
}
#endif

//--------
//qt 的 dll 调用方式还是比较危险的，如果 dll 正常的话，那很方便，只要在这里写上函数就可以了。
//但是如果 dll 有问题，或者是找不到的话，则会崩溃。并且没有任何的提示，而且还没运行到这个函数
//就崩溃了！！非常的危险。

//--------------------------------------------------------
//golang 调用 dll 的方式一般说法是 3 种
//1.传统操作系统的函数，不同平台不通用；
//2.就是现在用这个，直接写函数原型，然后 pro 项目文件写上 LIBS += 'D:\gopath1.10.8\src\_src_http_dll_mini\libgolang.dll'
//就能用的方式。但这种虽然最方便，但当 dll 文件或者是其中的函数不存在时则会直接崩溃而没有任何的提示。
//3.qt 自己的类操作系统函数方式。

//--------------------------------------------------------

//qt 特有的加载方式
int LoadGolangDll_QTCPP();


