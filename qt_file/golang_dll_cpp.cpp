
#include "golang_dll.h"

#include "qlibrary.h"
#include "stdio.h"

//qt 自有的加载 dll 函数的方式似乎是得用类的，所以有 .c 和 .cpp 两个文件才行


//https://blog.csdn.net/zhangbaoqiang1/article/details/82348930/

//--------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif


//一定要加 export 在函数名前，才会导出这个函数
//extern GoInt Sum(GoInt a, GoInt b);
//extern char* run_golang_json(const char* src_json);
//extern void free_golang_cstr(const char* golang_c_str);

//下面是函数指针的形式
char* (*_run_golang_json)(const char* src_json);
void (*_free_golang_cstr)(const char* golang_c_str);

//创建一个 tcp 连接
int  (*create_tcp_connect)();
void (*free_tcp_connect)(int i_client);

int (*tcp_connect)(int i_client, const char * host, int port, int use_ssl);

void (*tcp_send_string)(int i_client, const char *  line);

//使用终止分隔符的 //返回值要用 _free_golang_cstr 释放
//另外参数 sp 是终止符字符串，但其实只有第一个字符起使用,因为 golang 原始版本即是如此，可以自己再封装
char* (*tcp_recv_string)(int i_client, const char *  sp);

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------

//cpp 下不能像 c 语言那样赋值两个不同类型的指针，但函数太多，全部强类型的话代码太多不好维护
void _point_set(void ** dest, void * src)
{
    //void ** dest = &_dest;
    //void ** src = &_src;
    //*dest = * src;
    *dest = src;

}//

//取得一个函数
void _dll_get_func_(void ** dest, const char * func_name, QLibrary & mylib, int & r)
{
    //int r = 1;


    _point_set(dest, (void*)mylib.resolve(func_name));
    if(*dest)
    {

        printf("LoadGolangDll_QTCPP(): %s ok.\r\n", func_name);
    }else{
        r = 0;
        printf("LoadGolangDll_QTCPP(): %s error.\r\n", func_name);
    }

}//

int LoadGolangDll_QTCPP()
{
    int r = 1;

    //QLibrary mylib("dllhook");
    QLibrary mylib("libgolang");  //不要 dll 或者 so 的后缀名
    if(mylib.load())
    {
        //qDebug() << "dllhook加载成功";
        printf("LoadGolangDll_QTCPP(): dll/so ok.\r\n");

        //_run_golang_json = mylib.resolve("run_golang_json");
        _point_set((void**)&_run_golang_json, (void*)mylib.resolve("run_golang_json"));
        if(_run_golang_json)
        {
            //qDebug() << "SetKeyBoardHook加载成功";
            //bool flag = open((HWND)this->winId());
            printf("LoadGolangDll_QTCPP(): run_golang_json ok.\r\n");
        }else{
            r = 0;
            printf("LoadGolangDll_QTCPP(): run_golang_json error.\r\n");
        }

        //这一行代替上面的一大堆
        _dll_get_func_((void**)&_run_golang_json, "run_golang_json", mylib, r);

        //创建一个 tcp 连接
        _dll_get_func_((void**)&create_tcp_connect, "create_tcp_connect", mylib, r);
        _dll_get_func_((void**)&free_tcp_connect, "free_tcp_connect", mylib, r);

        //释放内存的，基本上都要调用
        _dll_get_func_((void**)&_free_golang_cstr, "free_golang_cstr", mylib, r);
        _dll_get_func_((void**)&tcp_connect, "tcp_connect", mylib, r);
        _dll_get_func_((void**)&tcp_send_string, "tcp_send_string", mylib, r);
        _dll_get_func_((void**)&tcp_recv_string, "tcp_recv_string", mylib, r);



    }else{
        r = 0;
        printf("LoadGolangDll_QTCPP(): dll/so error.\r\n");
    }


    return r;
}//



