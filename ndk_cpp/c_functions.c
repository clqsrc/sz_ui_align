//
// Created by ccc on 8/12/21.
//


#include "base_c.h"

#ifdef _android_
    #include <jni.h>
    #include "java_view.h"
    #include "c_view.h"
    #include "java_functions.h"
    #include "c_functions.h"
    #include "base_ndk.h"
    //#include "base_c.h"
#endif

#ifdef _ios_
    //#include <jni.h>
    //#include "java_view.h"
    #include "c_view.h"
    //#include "java_functions.h"
    #include "c_functions.h"
    //#include "base_ndk.h"
    //#include "base_c.h"
#endif

//---------------------------------------------------------------

//16 进制的颜色//android 和 ios 的颜色值其实都可以转换成 int64 ，所以用它来表示颜色也没是不可以，虽然严格来说应该定义另外一种类型
cint64 HexColor(const char * hex)
{
    _int64 r = Functions_hexStringToColor(hex);

    return r;
}//

cint64 Functions_HexColor(const char * hex)
{
    _int64 r = Functions_hexStringToColor(hex);

    //_USE2(a, b);

    return r;
}//


//暂时 //view 暂定为它的父 view
void ShowMessage_c(const char * msg, chandle view)
{
    jobject context = NULL;
    if (NULL != view)
    {
        context = View_getContext(view);
    }

    //jobject context = View_getContext(view);
    java_ShowMessage(msg, context);

}//

//暂时 //ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
//取消，用 dp 相关函数
//int _phone_size_(int size, chandle view)
//{
//    if (NULL == view) return 0;
//
//    jobject context = View_getContext(view);
//
//    //return Functions_Px2Dp(size, context);
//    return Functions_Dp2Px(size, context);
//
//}//

//测试 //ios 下显示所有控件
void ScrollView_ShowAllChild_c(chandle scrollView, int sp)
{
    if (NULL == scrollView) return;

//    UIScrollView * pnlSC = (UIScrollView *)_obj(scrollView);
//
//    UIScrollView_ShowAllChild(pnlSC, 100.0f); //拉伸 contentSize 以显示所有子控件

    //java_ScrollView_ShowAllChild(scrollView, 100.0f); //拉伸 contentSize 以显示所有子控件
    java_ScrollView_ShowAllChild(scrollView, sp); //拉伸 contentSize 以显示所有子控件
}//

//设置点击事件不要再传递
void SetDivIs_EventTouchEnd(chandle view)
{
    if (NULL == view) return;

    //设置点击事件不要再传递
    Functions_SetDivIs_EventTouchEnd(view);
}//

//动画平移
void View_Anima_toLeft(chandle _view, int width, float time_second)
{
    //动画平移
    java_View_Anima_toLeft(_view, width, time_second);
}//


//动画移动
void View_Anima_Move(chandle view, int left, int top, float time_second)
{
    //动画平移
    java_View_Anima_Move(view, left, top, time_second);
}//


//读取附加路径的文件 //fn 是相对路径
//这是个特别的函数，对于 android 来说就是 assets 目录，对于 ios 来说则是一个引用的源码目录。
//之所以用 ios 的引用目录，在于放到里面的文件会自动编译进 app 而不是在 xcode 中一个个的手工加入
//所以对于 ios 来说这个是一个不固定的目录，目前来说直接指向 ui json 的目录作为根目录
lstring * Functions_LoadStringFromFile_assets(const char * fn, mempool * mem)
{
    //读取附加路径的文件 //fn 是相对路径
    lstring * s = java_Functions_LoadStringFromFile_assets(fn, mem);

    return s;
}//

//保存一个值到手机中
void Functions_SetSaveValue(const char * key, const char * value)
{
    java_Functions_SetSaveValue(key, value);
}//

lstring * Functions_GetSaveValue(const char * key, mempool * mem)
{
    return java_Functions_GetSaveValue(key, mem);
}//

//2022.09
lstring * Functions_RunJson(const char * key, mempool * mem)
{
    return java_Functions_RunJson(key, mem);
}//


//----
//方便性的函数
int Functions_StrToInt(const char * s)
{
    return atoi(s);
}//

double Functions_StrToFloat(const char * s)
{
    return atof(s);
}//

//----

//变更高宽比显示全部图片内容 //2021.10.25
void Functions_ShowImageAll_Scale(chandle view)
{
    Functions_SetIcon_ShowAll_Scale(view);
}//

//----  虚拟 div 的操作
void UI_Div_SetTxtColor(cui_json uijson, const char * id, const char * hexColor)
{
    ui_json_UI_Div_SetTxtColor(uijson, id, hexColor);
}//


void UI_Div_SetTxt(cui_json uijson, const char * id, const char * txt)
{
    ui_json_UI_Div_SetTxt(uijson, id, txt);
}//

void UI_Div_SetImage(cui_json uijson, const char * id, const char * image)
{
    ui_json_UI_Div_SetImage(uijson, id, image);
}//

//关闭软件键盘，iphone6s 肯定是需要的，安卓待测试
void UI_Div_CloseKeyboard(cui_json uijson)
{
    ui_json_UI_Div_CloseKeyboard(uijson);
}//

//----  虚拟 div 的操作 _end;

//----
//蓝牙的暂时也放这里
//创建一个蓝牙
chandle Blle_Create()
{
    return java_Blle_Create();
}//

//设置蓝牙事件函数
void Blle_SetOnEvent(chandle blle, cint64 func_c)
{
    java_Blle_SetOnEvent(blle, func_c);
}//

//启动蓝牙
void Blle_Start(chandle blle)
{
    java_Blle_Start(blle);
}//

//根据蓝牙地址取蓝牙名
lstring * Blle_GetDeviceName(chandle blle, const char * mac_addr, mempool * mem)
{
    return java_Blle_GetDeviceName(blle, mac_addr, mem);
}//

//根据蓝牙地址连接设备
void Blle_ConnectDevice(chandle blle, const char * mac_addr)
{
    //根据蓝牙地址连接设备
    java_Blle_ConnectDevice(blle, mac_addr);
}//

//关闭蓝牙连接
void Blle_CloseConnec(chandle blle)
{
    java_Blle_CloseConnec(blle);
}


//根据蓝牙地址取蓝牙信号强度
lstring * Blle_GetDeviceRSSI(chandle blle, const char * mac_addr, mempool * mem)
{
    return java_Blle_GetDeviceRSSI(blle, mac_addr, mem);
}//

//后面几个服务和 char 的函数是最复杂的
//取当前设备的服务个数
int Blle_GetServicesCount(chandle blle)
{
    return java_Blle_GetServicesCount(blle);
}//

//取当前设备的某个服务下的节点 chr 个数
int Blle_GetServiceCharCount(chandle blle, int service_index)
{
    return java_Blle_GetServiceCharCount(blle, service_index);
}//

//判断 char 是否是 notify 类型
int Blle_CharIsNotify(chandle blle, int service_index, int char_index)
{
    return java_Blle_CharIsNotify(blle, service_index, char_index);
}//

//取 char 的 uuid
lstring * Blle_GetCharUUID(chandle blle, int service_index, int char_index, mempool * mem)
{
    return java_Blle_GetCharUUID(blle, service_index, char_index, mem);
}//

//发送一个字符串
void Blle_SendBuf(chandle blle, int service_index, int char_index, const char * buf, int buf_len)
{
    java_Blle_SendBuf(blle, service_index, char_index, buf, buf_len);
}


//开始监听某个 char
void Blle_StartNotifyChar(chandle blle, int service_index, int char_index)
{
    java_Blle_StartNotifyChar(blle, service_index, char_index);
}//

//----

//----
//蓝牙的暂时在这里，那么 http 的也放这里好了 //因为没有创建实例，所以其实是单实例的，所以只能在 ui 主线程中调用这些函数

//http 访问及相关的接收事件设置
void HttpPost_AndSetOnEvent(const char * url, cint64 func_c, cint64 param_c)
{
    //JNIEnv* env = g_java_functions_info.env;
    JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/HttpFunctions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "HttpPost_UI",
                                               "(Ljava/lang/String;JJ)V");

    clear_Java_Exception();

    jstring jstr_url = CStr2Jstring(env, url);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 jstr_url,
                                 func_c,
                                 param_c
    );

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_url);
}//

void HttpGetToFile_AndSetOnEvent(const char * url, const char * fn, cint64 func_c, cint64 param_c)
{
    //JNIEnv* env = g_java_functions_info.env;
    JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/HttpFunctions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "HttpGetToFile_UI",
                                               "(Ljava/lang/String;Ljava/lang/String;JJ)V");

    clear_Java_Exception();

    jstring jstr_url = CStr2Jstring(env, url);
    jstring jstr_fn = CStr2Jstring(env, fn);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 jstr_url, jstr_fn,
                                 func_c,
                                 param_c
    );

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_url);
    (*env)->DeleteLocalRef(env, jstr_fn);

}//

void HttpPost_SetParam(const char * key, const char * value)
{
    //JNIEnv* env = g_java_functions_info.env;
    JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/HttpFunctions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "Http_SetParam",
                                               "(Ljava/lang/String;Ljava/lang/String;)V");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);
    jstring jstr_key = CStr2Jstring(env, key);
    jstring jstr_value = CStr2Jstring(env, value);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 jstr_key,
                                 jstr_value
    );

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_key);
    (*env)->DeleteLocalRef(env, jstr_value);
}//

void HttpPost_ClearParam()
{
    //JNIEnv* env = g_java_functions_info.env;
    JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/HttpFunctions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "Http_ParamClear",
                                               "()V");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    (*env)->CallStaticVoidMethod(env, class_, func//,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
//                                 blle,
//                                 service_index,
//                                 char_index
    );

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
}//

//----











