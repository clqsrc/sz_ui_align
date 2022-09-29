//
// Created by ccc on 8/12/21.
//

#include <jni.h>
#include "java_view.h"
#include "java_functions.h"
#include "base_ndk.h"
#include "lstring.h"

//访问 java(android) 的 view //当然以后直接换纯 c 的


//必须缓存一些信息才可能调整访问 //其实相当于缓存 iis
struct java_functions_info
{
    JNIEnv* env;
    //jclass class_view;               //类的接口

    jclass class_Functions;  // jclass class_ = (*env)->FindClass(env, "hpublic/Functions"); 的缓存



    //jmethodID setBackgroundColor;
    jmethodID func_ShowMessage;
    jmethodID func_SetFrame;   //static public void SetFrame(View view, int left, int top, int w, int h)
    jmethodID func_SetFrame_dp;
    jmethodID func_SetFrame_dp_c;
    jmethodID func_ShowToLeft;
    jmethodID func_hexStringToColor;
    jmethodID func_GetParentView;

    //--------
    jclass class_NDK_c_interface;  //NDK_c_interface.java 的缓存
    jmethodID func_View_SetOnClick;
    jmethodID func_View_SetOnClick_v2;
    jmethodID func_View_BringToFront;
    jmethodID func_View_SetOnSize;
    jmethodID func_UIScrollView_ShowAllChild;

};

struct java_functions_info g_java_functions_info = {};

//---------------------------------------------------------------
//取 Functions 的各个函数指针
void get_Functions_func();
//取 NDK_c_interface 的各个函数指针
void get_NDK_c_interface_func();

//---------------------------------------------------------------

//只调用一次，不释放也可以
void * init_java_functions_info(JNIEnv* env)
{
    //1.new一个Hashmap：

    //FindClass
    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");

    g_java_functions_info.class_Functions = class_;

    //转换为全局引用。如果不转，showmesage 这样的函数在 c 的事件中就调用就会崩溃
    //将涉及到的 class 全部改为全局引用就没有崩溃现象了。
    g_java_functions_info.class_Functions = change_class_ref(env, g_java_functions_info.class_Functions);
    //--------
    //FindClass
//    jclass class_2 = (*env)->FindClass(env, "hpublic/NDK_c_interface");
//
//    g_java_functions_info.class_NDK_c_interface = class_2;
//    g_java_functions_info.class_NDK_c_interface = change_class_ref(env, g_java_functions_info.class_NDK_c_interface);
    //--------

    //(*env)->ExceptionClear(env);  //原因同上
    //jmethodID setBackgroundColor = (*env)->GetMethodID(env, class_, "setBackgroundColor",
    //                                                   "(I)V"  //asd 很强，会自动修正这个参数
    //                                                   );

    //-----------------
    //g_java_functions_info.class_view = class_;
    //g_java_functions_info.setBackgroundColor = setBackgroundColor;
    g_java_functions_info.env = env;

    //----------------
    //取 Functions 的各个函数指针
    get_Functions_func();
    //取 NDK_c_interface 的各个函数指针
    get_NDK_c_interface_func();

}//

//取 Functions 的各个函数指针
void get_Functions_func()
{
    JNIEnv* env = g_java_functions_info.env;

    //jclass class_ = g_java_functions_info.class_Functions;
    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用

    //函数名并不是字符串，而是特殊的 类型签名 。Android NDK 的 Type Signatures（类型签名）
    //类型签名（Type Signatures）
    //这里是可以从 asd 的代码提示中自动得到的

    //jmethodID func_ShowMessage = (*env)->GetStaticMethodID(env, class_, "ShowMessage", "(Ljava/lang/String;Landroid/content/Context;)V");
    jmethodID func_ShowMessage = (*env)->GetStaticMethodID(env, class_, "ShowMessage_ok", "(Ljava/lang/String;Landroid/content/Context;)V");
    g_java_functions_info.func_SetFrame = (*env)->GetStaticMethodID(env, class_, "SetFrame", "(Landroid/view/View;IIII)V");
    g_java_functions_info.func_SetFrame_dp = (*env)->GetStaticMethodID(env, class_, "SetFrame_dp", "(Landroid/view/View;IIIILandroid/content/Context;)V");
    g_java_functions_info.func_SetFrame_dp_c = (*env)->GetStaticMethodID(env, class_, "SetFrame_dp_c", "(Landroid/view/View;IIII)V");


    //动画显示一个 view
    g_java_functions_info.func_ShowToLeft = (*env)->GetStaticMethodID(env, class_, "ShowToLeft", "(Landroid/view/View;)V");

    //颜色
    g_java_functions_info.func_hexStringToColor = (*env)->GetStaticMethodID(env, class_, "hexStringToColor",
                                                                            "(Ljava/lang/String;)I");


    g_java_functions_info.func_GetParentView = (*env)->GetStaticMethodID(env, class_, "GetParentView",
                                                                            "(Landroid/view/View;)Landroid/view/ViewGroup;");

    g_java_functions_info.func_UIScrollView_ShowAllChild = (*env)->GetStaticMethodID(env, class_, "UIScrollView_ShowAllChild",
                                                                                     "(Lui_json/ui_control_panel_ScrollView;I)V");

    g_java_functions_info.func_ShowMessage = func_ShowMessage;
}//

//拉伸 contentSize 以显示所有子控件
void java_ScrollView_ShowAllChild(jobject pnlSC, int sp)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_UIScrollView_ShowAllChild;

    clear_Java_Exception();

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
             pnlSC,
             sp
    );

    clear_Java_Exception();
}//

//取 NDK_c_interface 的各个函数指针
void get_NDK_c_interface_func()
{
    JNIEnv* env = g_java_functions_info.env;

    //jclass class_ = g_java_functions_info.class_NDK_c_interface;

    //---------------------------------------------------------------
    //似乎这个类型不能重复赋值，所以前面的那个 FindClass 要取消掉
    jclass class_2 = (*env)->FindClass(env, "hpublic/NDK_c_interface");

    g_java_functions_info.class_NDK_c_interface = class_2;
    g_java_functions_info.class_NDK_c_interface = change_class_ref(env, g_java_functions_info.class_NDK_c_interface);

    //---------------------------------------------------------------
    jclass class_ = g_java_functions_info.class_NDK_c_interface;

    //---------------------------------------------------------------

    //函数名并不是字符串，而是特殊的 类型签名 。Android NDK 的 Type Signatures（类型签名）
    //类型签名（Type Signatures）
    //这里是可以从 asd 的代码提示中自动得到的

    jmethodID func_View_SetOnclick = (*env)->GetStaticMethodID(env, class_, "View_SetOnclick",
                                                               "(Landroid/view/View;J)V");

    g_java_functions_info.func_View_SetOnClick_v2 = (*env)->GetStaticMethodID(env, class_, "View_SetOnclick_v2",
                                                               "(Landroid/view/View;JJ)V");

    g_java_functions_info.func_View_BringToFront = (*env)->GetStaticMethodID(env, class_, "View_BringToFront",
                                                                              "(Landroid/view/View;)V");

    g_java_functions_info.func_View_SetOnSize = (*env)->GetStaticMethodID(env, class_, "View_SetOnSize",
                                                                          "(Lui_json/ui_control_panel;JJ)V");


    g_java_functions_info.func_View_SetOnClick = func_View_SetOnclick;
}//



//第一个版本传的是原始的控件作为参数，第二版本应当传递 c 传递过来的指针参数，原样在事件中返回
void java_View_SetOnClick(jobject view, jlong func_c)
{
    if (NULL == view) return;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_NDK_c_interface;
    jmethodID func = g_java_functions_info.func_View_SetOnClick;

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 view,
                                 func_c
    );

    clear_Java_Exception();
}//

//提前  //原始的 java View 是没有这个函数的（至少不是全部的版本都有），所以实际上只能借助公用函数
void java_View_BringToFront(jobject view)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_NDK_c_interface;
    jmethodID func = g_java_functions_info.func_View_BringToFront;

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view
    );

    clear_Java_Exception();
}//


//第一个版本传的是原始的控件作为参数，第二版本应当传递 c 传递过来的指针参数，原样在事件中返回
void java_View_SetOnClick_v2(jobject view, jlong func_c, jlong param_list)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_NDK_c_interface;
    jmethodID func = g_java_functions_info.func_View_SetOnClick_v2;

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 view,
                                 func_c,
                                 param_list
    );

    clear_Java_Exception();
}//



//实际上我们的 ui_control_panel 才支持 OnSize 事件，直接传 View 类型的参数的话是错误的
void java_View_SetOnSize(jobject view, jlong func_c, jlong param_list)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_NDK_c_interface;
    jmethodID func = g_java_functions_info.func_View_SetOnSize;

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            //下面是参数
            view,
            func_c,
            param_list
    );

    clear_Java_Exception();
}//

//源码是 utf8 的话，直接用中文即可
void java_ShowMessage(const char * msg, jobject jthis)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_ShowMessage;

    clear_Java_Exception();

    //(*env)->CallVoidMethod(env, obj, func,   //CallVoidMethod 表示返回值为 void
    //jobject r = (*env)->CallObjectMethod(env, obj, func,   //CallObjectMethod 表示返回值为对象
    //CallStaticObjectMethod  //静态函数是这样的
    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            (*env)->NewStringUTF(env, msg),
            jthis
    );

    clear_Java_Exception();
}//

//单位是原始像素
void java_SetFrame(jobject view, int left, int top, int width, int height)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_SetFrame;

    clear_Java_Exception();


    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view, left,  top,  width,  height
    );

    clear_Java_Exception();
}//

//单位是 dp
void java_SetFrame_dp(jobject view, int left, int top, int width, int height)
{
    //if (NULL == ui_json) return NULL;

    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    ////jmethodID func = g_java_functions_info.func_SetFrame_dp;
    jmethodID func = g_java_functions_info.func_SetFrame_dp_c;

    clear_Java_Exception();


    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 view, left,  top,  width,  height
    );

    clear_Java_Exception();
}//

//动画显示一个 view
void Functions_ShowToLeft(jobject view)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_ShowToLeft;

    clear_Java_Exception();


    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view
    );

    clear_Java_Exception();
}//


//颜色//android 和 ios 的颜色值其实都可以转换成 int64 ，所以用它来表示颜色也没是不可以，虽然严格来说应该定义另外一种类型
_int64 Functions_hexStringToColor(const char * _s_hex)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_hexStringToColor;

    clear_Java_Exception();


    jstring s_hex = (*env)->NewStringUTF(env, _s_hex);

    _int64 r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            s_hex
    );

    clear_Java_Exception();

    return r;
}//

//变更高宽比显示全部图片内容 //2021.10.25
//static public void SetIcon_ShowAll_Scale(ImageView img)
void Functions_SetIcon_ShowAll_Scale(jobject view)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    //jclass class_ = g_java_functions_info.class_Functions;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "SetIcon_ShowAll_Scale",
                                               "(Landroid/widget/ImageView;)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view
    );

    clear_Java_Exception();

}//

//ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
int Functions_Px2Dp(int value, jobject jthis)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    //jclass class_ = g_java_functions_info.class_Functions;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "px2dp", "(Landroid/content/Context;F)I");

    clear_Java_Exception();

    int r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            jthis, (float)value
    );

    clear_Java_Exception();

    return r;

}//

//设置点击事件不要再传递
void Functions_SetDivIs_EventTouchEnd(jobject view)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    //jclass class_ = g_java_functions_info.class_Functions;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "SetDivIs_EventTouchEnd",
                                               "(Lui_json/ui_control_panel;)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view
    );

    clear_Java_Exception();


}//

//ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
int Functions_Dp2Px(int value, jobject jthis)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    //jclass class_ = g_java_functions_info.class_Functions;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "dip2px", "(Landroid/content/Context;F)I");

    clear_Java_Exception();

    int r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                        jthis, (float)value
    );

    clear_Java_Exception();

    return r;

}//

//显示或者隐藏一个 view
void Functions_ShowView(jobject view, int show)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    //jclass class_ = g_java_functions_info.class_Functions;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "ShowView",
                                               "(Landroid/view/View;I)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view, show
    );

    clear_Java_Exception();

}//

void java_Functions_SetSaveValue(const char * key, const char * value)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "SetSaveValue",
                                               "(Ljava/lang/String;Ljava/lang/String;)V");

    clear_Java_Exception();

    jstring jstr_key = CStr2Jstring(env, key);
    jstring jstr_value = CStr2Jstring(env, value);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 jstr_key,jstr_value
    );

    //----
    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_value);
    (*env)->DeleteLocalRef(env, jstr_key);
    //(*env)->DeleteLocalRef(env, class_);
    //(*env)->DeleteLocalRef(env, class_);
}//

//动画平移
void java_View_Anima_toLeft(jobject view, int width, float time_second)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "View_Anima_toLeft",
                                               "(Landroid/view/View;IF)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 view, width, time_second
    );

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
}//

void java_View_Anima_Move(jobject view, int left, int top, float time_second)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "View_Anima_Move",
                                               "(Landroid/view/View;IIF)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 view, left, top, time_second
    );

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);

}//

lstring * java_Functions_GetSaveValue(const char * key, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetSaveValue",
                                               "(Ljava/lang/String;)Ljava/lang/String;");

    clear_Java_Exception();

    jstring jstr_key = CStr2Jstring(env, key);
    //jstring jstr_value = CStr2Jstring(env, value);

    jstring jstr_value = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 jstr_key
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * value = Jstring2lstring(env, jstr_value, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_value);
    (*env)->DeleteLocalRef(env, jstr_key);
    //(*env)->DeleteLocalRef(env, class_);
    //(*env)->DeleteLocalRef(env, class_);

    return value;
}//

//2022.09
lstring * java_Functions_RunJson(const char * key, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "RunJson",
                                               "(Ljava/lang/String;)Ljava/lang/String;");

    clear_Java_Exception();

    jstring jstr_key = CStr2Jstring(env, key);
    //jstring jstr_value = CStr2Jstring(env, value);

    jstring jstr_value = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            jstr_key
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * value = Jstring2lstring(env, jstr_value, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_value);
    (*env)->DeleteLocalRef(env, jstr_key);
    //(*env)->DeleteLocalRef(env, class_);
    //(*env)->DeleteLocalRef(env, class_);

    return value;
}//


//旋转视图 //参数为 视图、旋转角度、持续时间（毫秒）
void java_View_RotateAni(jobject view, int angle_360, int millisecond)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "View_RotateAni",
                                               "(Landroid/view/View;II)V");

    clear_Java_Exception();

    //jstring jstr_fn = CStr2Jstring(env, fn);
    //jstring jstr_value = CStr2Jstring(env, value);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view, angle_360, millisecond
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    //lstring * value = Jstring2lstring(env, jstr_value, mem);

    //----

    clear_Java_Exception();

    //return value;
}//

//读取附加路径的文件 //fn 是相对路径
lstring * java_Functions_LoadStringFromFile_assets(const char * fn, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Functions");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "LoadStringFromFile_assets",
                                               "(Ljava/lang/String;)Ljava/lang/String;");

    clear_Java_Exception();

    jstring jstr_fn = CStr2Jstring(env, fn);
    //jstring jstr_value = CStr2Jstring(env, value);

    jstring jstr_value = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                                        jstr_fn
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * value = Jstring2lstring(env, jstr_value, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, jstr_fn);
    (*env)->DeleteLocalRef(env, jstr_value);

    return value;
}//

//创建一个蓝牙
jobject java_Blle_Create()
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "CreateBlle",
                                               "()Lhpublic/Blle;");

    clear_Java_Exception();

    jobject blle = (*env)->CallStaticObjectMethod(env, class_, func  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            //view, show
    );

    clear_Java_Exception();

    //2021.11.22 要转换为全局引用指针才行的
    blle = change_obj_ref(env, blle);  //在这里调用 change_obj_ref 就好了，这个接口不应该在 c 接口使用过程中出现

    clear_Java_Exception();

    return blle;

}//

//设置蓝牙事件函数
void java_Blle_SetOnEvent(jobject blle, jlong func_c)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "SetEventFunc",
                                               "(Lhpublic/Blle;JJ)V");

    clear_Java_Exception();

    //int i = (int)123;//func_c;
    //long long i = (long long)func_c;

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            //blle, (jlong)blle, (jlong)func_c//(int)func_c
            //blle, (jlong)blle, 444//func_c //这个在最新环境下会出错!
            //blle, (jlong)blle, (jlong)444//func_c
            //blle, 0, 0//func_c //没有 (jLong的强制转换，居然有某些编译环境下会导致传递的参数异常！即 SetEventFunc 中得到的参数会不是 0 ！但我之前的 ndk 一直是正确有！)
            ////blle, (jlong)blle, (jlong)0//func_c //ok
            //blle, blle, func_c  //error 真机 ok,模拟器会崩溃!
            blle, (jlong)blle, (jlong)func_c //最后正确的是这个
            //blle, (jlong)blle, func_c //这个也可以，就是说 jobject 当做其他类型参数（比如 jlong）时一定要加强制转换！会引起不知所谓的错误！（至少最新 ndk 下是如此）
    );

    clear_Java_Exception();

}//

//启动蓝牙
void java_Blle_Start(jobject blle)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "Blle_Start",
                                               "(Lhpublic/Blle;)V");

    clear_Java_Exception();

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 blle
    );

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);

}//

//根据蓝牙地址取蓝牙名
lstring * java_Blle_GetDeviceName(jobject blle, const char * mac_addr, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetDeviceName",
                                               "(Lhpublic/Blle;Ljava/lang/String;)Ljava/lang/String;");

    clear_Java_Exception();

    jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jobject device_name = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 blle,
                                 jstr_mac_addr
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * _device_name = Jstring2lstring(env, device_name, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, device_name);
    (*env)->DeleteLocalRef(env, jstr_mac_addr);
    //(*env)->DeleteLocalRef(env, class_);


    return _device_name;

}//

//根据蓝牙地址取蓝牙强度
lstring * java_Blle_GetDeviceRSSI(jobject blle, const char * mac_addr, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetDeviceRSSI",
                                               "(Lhpublic/Blle;Ljava/lang/String;)Ljava/lang/String;");

    clear_Java_Exception();

    jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jobject device_rssi = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                                         blle,
                                                         jstr_mac_addr
    );

    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * _device_rssi = Jstring2lstring(env, device_rssi, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, device_rssi);
    (*env)->DeleteLocalRef(env, jstr_mac_addr);
    //(*env)->DeleteLocalRef(env, obj_view);

    return _device_rssi;

}//


//根据蓝牙地址连接设备
void java_Blle_ConnectDevice(jobject blle, const char * mac_addr)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "ConnectDevice",
                                               "(Lhpublic/Blle;Ljava/lang/String;)V");

    clear_Java_Exception();

    jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                                         blle,
                                                         jstr_mac_addr
    );

    //----

    clear_Java_Exception();

}//

//关闭蓝牙连接
void java_Blle_CloseConnec(jobject blle)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "CloseConnect",
                                               "(Lhpublic/Blle;)V");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 blle//,
                                 //jstr_mac_addr
    );

    //----

    clear_Java_Exception();

}//

//取当前设备的服务个数
int java_Blle_GetServicesCount(jobject blle)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetServicesCount",
                                               "(Lhpublic/Blle;)I");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jint r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                 blle
    );

    //----

    clear_Java_Exception();

    return r;
}//

//取当前设备的某个服务下的节点 chr 个数
int java_Blle_GetServiceCharCount(jobject blle, int service_index)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetServiceCharCount",
                                               "(Lhpublic/Blle;I)I");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jint r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                         blle,
                                         service_index
    );

    //----

    clear_Java_Exception();

    return r;
}//

//判断 char 是否是 notify 类型
int java_Blle_CharIsNotify(jobject blle, int service_index, int char_index)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "CharIsNotify",
                                               "(Lhpublic/Blle;II)I");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jint r = (*env)->CallStaticIntMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                         blle,
                                         service_index,
                                         char_index
    );

    //----

    clear_Java_Exception();

    return r;
}//

//取 char 的 uuid
lstring * java_Blle_GetCharUUID(jobject blle, int service_index, int char_index, mempool * mem)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "GetCharUUID",
                                               "(Lhpublic/Blle;II)Ljava/lang/String;");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    jstring uuid = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                         blle,
                                         service_index,
                                         char_index
    );

    //----
    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    lstring * _uuid = Jstring2lstring(env, uuid, mem);

    //----

    clear_Java_Exception();

    return _uuid;
}//

//发送一个字符串
void java_Blle_SendBuf(jobject blle, int service_index, int char_index, const char * buf, int buf_len)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "SendString",
                                               "(Lhpublic/Blle;IILjava/lang/String;)V");

    clear_Java_Exception();

    jstring _buf = CStr2Jstring(env, buf);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                                  blle,
                                                  service_index,
                                                  char_index,
                                                  _buf
    );

    //----
    //---- 转换出字符串
    //mempool * mem = newmempool();
    //mempool * mem = out_device_name->pool;

    //lstring * _uuid = Jstring2lstring(env, uuid, mem);

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);
    (*env)->DeleteLocalRef(env, _buf);


    //return _uuid;
}//

//开始监听某个 char
void java_Blle_StartNotifyChar(jobject blle, int service_index, int char_index)
{
    JNIEnv* env = g_java_functions_info.env;

    clear_Java_Exception();

    jclass class_ = (*env)->FindClass(env, "hpublic/Blle");  //这样才有提示  //写代码时临时用
    jmethodID func = (*env)->GetStaticMethodID(env, class_, "StartNotifyChar",
                                               "(Lhpublic/Blle;II)V");

    clear_Java_Exception();

    //jstring jstr_mac_addr = CStr2Jstring(env, mac_addr);

    (*env)->CallStaticVoidMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
                                         blle,
                                         service_index,
                                         char_index
    );

    //----

    clear_Java_Exception();

    (*env)->DeleteLocalRef(env, class_);

}//

//------------------------------------------------

jobject Functions_GetParentView(jobject view)
{
    JNIEnv* env = g_java_functions_info.env;
    //jobject obj = ui_json;
    jclass class_ = g_java_functions_info.class_Functions;
    jmethodID func = g_java_functions_info.func_GetParentView;

    clear_Java_Exception();

    jobject r = (*env)->CallStaticObjectMethod(env, class_, func,  //注意，静态函数时第二个参数由对象变成了类
            // 下面是参数
            view
    );

    clear_Java_Exception();

    return r;
}//


//--------
//暂时放这里
//https://blog.csdn.net/u013037007/article/details/50992541
//从源码看是要用 free 来释放的
char * Jstring2CStr(JNIEnv * env, jstring jstr, int * out_buf_len)
{
    char * buf = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    //jstring strencode = env->NewStringUTF("GB2312");//转换成Cstring的GB2312，兼容ISO8859-1
    jstring strencode = (*env)->NewStringUTF(env, "UTF-8");//转换成Cstring的GB2312，兼容ISO8859-1
    jmethodID method = (*env)->GetMethodID(env, clsstring,"getBytes","(Ljava/lang/String;)[B");
    jbyteArray arr = (jbyteArray)(*env)->CallObjectMethod(env, jstr,method,strencode); //String.getByte("GB2312");
    jsize len = (*env)->GetArrayLength(env, arr);
    jbyte * ba = (*env)->GetByteArrayElements(env, arr,JNI_FALSE); //jbyte 不是类，不能删除本地变量

    //---- 2021.10.28 原来的转换似乎是有问题 //这个时候 asd 的调试器中是看不到 key 中的真实值的，不过可以看到转换后的 pChar 值
    //https://blog.csdn.net/u013718730/article/details/116779189
    ////const char *pChar = (*env)->GetStringUTFChars(env, key, 0);
    ////(*env)->ReleaseStringUTFChars(env, key, pChar);

    //按道理说，这个算法更好

    //----

    //奇怪，有时候是正确的，似乎是内存异常了 2021.10.28 //是下面的长度指针没写地址符号造成的

    if(len > 0)
    {
        buf = (char*)malloc(len+1);
        memcpy(buf, ba, len);
        buf[len] = 0;  //"\0"
    }

    //out_buf_len = len;  //ndk 居然没有报错
    (*out_buf_len) = len; //2021.10.28

    (*env)->ReleaseByteArrayElements(env, arr, ba, 0);

    //---- 似乎不用也可以
    (*env)->DeleteLocalRef(env, clsstring);
    (*env)->DeleteLocalRef(env, strencode);
    (*env)->DeleteLocalRef(env, arr);
    //(*env)->DeleteLocalRef(env, ba); //jbyte 不是类，不能删除本地变量
    //(*env)->DeleteLocalRef(env, clsstring);

    return buf;
}//

lstring * Jstring2lstring(JNIEnv * env, jstring jstr, mempool * pool)
{
    if (NULL == jstr) return NewString("", pool);  //2021.10.28 当输入的 String 为 null 时确实是会发生这种情况的

    //--------

    int out_buf_len = 0;
    char * buf = Jstring2CStr(env, jstr, &out_buf_len);

    //lstring * r = NewString(NULL, pool);
    lstring * r = NewString("", pool);  //ndk 中第一个参数不能为 NULL ，似乎是 strlen 引起的异常
    r->AppendCString(r, buf, out_buf_len);

    free(buf); //注意，要释放原内存

    return r;
}//


jstring CStr2Jstring(JNIEnv * env, const char * pat)
{
    //jclass strClass = (*env)->FindClass(env, "Ljava/lang/String;");
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID method = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = (*env)->NewByteArray(env, (jsize)strlen(pat));
    (*env)->SetByteArrayRegion(env, bytes, 0, (jsize)strlen(pat), (jbyte*)pat); //将char* 转换为byte数组

    //jstring encoding = env->NewStringUTF("GB2312");
    jstring encoding = (*env)->NewStringUTF(env, "UTF-8");//转换成Cstring的GB2312，兼容ISO8859-1

    //return (jstring)(*env)->NewObject(env, strClass, method, bytes, encoding);

    jstring r = (jstring)(*env)->NewObject(env, strClass, method, bytes, encoding);


    //---- 这个应该经常用到，加上手动删除后工业机就基本正常了
    (*env)->DeleteLocalRef(env, strClass);
    (*env)->DeleteLocalRef(env, bytes);
    //(*env)->DeleteLocalRef(env, method);  //method 是 id 不是对象，是不用删除的，也不能删
    (*env)->DeleteLocalRef(env, encoding);
    //(*env)->DeleteLocalRef(env, jstr_fn);

    return r;
}//








