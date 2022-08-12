


//#include <jni.h>
//#include <windows.h>
#include <QMainWindow>
#include <QLabel>
// #include "base_ndk.h"
// #include "java_view.h"
// #include "java_functions.h"
#include "base_c.h"
//#include "c_view.h"
#include "c_ui_json.h"
#include "c_ui_json_windows.h"
#include "ui_json.h"
#include "cpp_string_hashmap.h"
#include "c_functions.h"
#include "ui_panel.h"



//创建一个视图
chandle CreateView(chandle parentView)
{
    //ui_json_CreateView("view", parentView);
    return ui_json_CreateView("view", parentView);  //qt android 手机下是一定要有返回值的，否则会崩溃
    //奇怪的是 qt Windows 桌面版则不会
    //2022.6 修正
}//

//2022 //可以创建类的，不过目前也只是支持 image 而已，可以自己扩展
//创建一个视图
chandle UI_CreateViewClass(chandle parentView, const char * ui_class)
{
    mempool * mem = newmempool();
    //return ui_json_CreateView("view", parentView);

    chandle r = 0;

    lstring * s = NewString(ui_class, mem);

    if (s->equals_cstr(s, "image"))
    {
        //QtDiv * view = new QtDiv();
        QLabel * view = new QLabel();

        view->setParent((QWidget *)parentView);

        view->show();

        r = (chandle)view;
    }


    freemempool(mem);

    return r;
}//

//从 json 中创建
cui_json UI_CreateFromJson(chandle parentView, const char * fn)
{
    cui_json r = {0};

    //JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;

    r.handle = ui_json_CreateFromJson((QWidget *)parentView, fn);

    //r.handle = change_obj_ref(env, r.handle);  //在这里调用 change_obj_ref 就好了，这个接口不应该在 c 接口使用过程中出现

    ui_json_BindFunctions(&r);

    return r;

}//

//取一个视图
chandle ui_json_GetView_c(cui_json uijson, const char * id)
{
    chandle r = NULL;

    mempool * mem = newmempool();

    ui_json * uj = (ui_json *)uijson.handle;

    //lstring * _id = inttostr
    lstring * _view = string_hashmap_get(uj->nodes, id, mem);

    cint64 i_view = Functions_StrToInt64(_view->str);

    // r = ui_json_GetView(uijson.handle, id);

    // return r;

    freemempool(mem);

    return (chandle)i_view;

    return 0;
}//


//---------------------------------------------------------------
//ui_json 相关的

//取根视图
chandle ui_json_GetRootView(cui_json uijson)
{
    // jobject view = getViewGroupField(uijson.handle, "root_view"); //getViewField() 不认 ViewGroup 这样的子类的，必须换函数

    ui_json * uj = (ui_json *)uijson.handle;

    return uj->root_view;


    // return view;

    return 0;

}//

//取根 json
//取一个对象的 view 字段
//jobject getViewField(jobject jthis, const char * field_name)
chandle ui_json_GetRootJson(cui_json uijson)
{
    const char * field_name = "root_json";

    //---------------------------------------------------------------

    // JNIEnv* env = java_ndk_GetEnv();  //g_java_view_info.env;
    // //jclass cls = (*env)->GetObjectClass(env, jthis);
    // jclass cls = (*env)->FindClass(env, "ui_json/NDK_ui_json");
    // jobject jthis = uijson.handle;

    // //设置局部静态变量，保证其只会被赋值一次
    // //static jfieldID key_id = NULL;
    // jfieldID key_id = NULL;
    // if (key_id == NULL)
    // {
    //     //key_id = (*env)->GetFieldID(cls, "view2", "Landroid/view/View;");
    //     key_id = (*env)->GetFieldID(env, cls,  field_name, "Lorg/json/JSONObject;");

    //     jobject view = (*env)->GetObjectField(env, jthis, key_id);
    //     //View_setBackgroundColor(view, 0xFF000000); //黑色，0 的话是透明

    //     return view;
    // }

    return NULL;
}//





//--------
//对齐各个控件
//void AlignControl(NSDictionary * json)
//public void AlignControl(JSONObject json)
void ui_json_AlignControl(cui_json uijson)
{

    // JNIEnv* env = java_ndk_GetEnv();  
    // jclass cls = (*env)->FindClass(env, "ui_json/NDK_ui_json");
    // jobject jthis = uijson.handle;

    // jobject root_json = ui_json_GetRootJson(uijson);

    // jmethodID funcAlignControl = (*env)->GetMethodID(env,cls, "AlignControl", "(Lorg/json/JSONObject;)V");

    // (*env)->CallVoidMethod(env, jthis, funcAlignControl, root_json);

}//


//对齐子控件
//void ui_json_AlignControlSub(cui_json uijson)
//对齐子控件 //手工修改了控件有没睡的时候才使用，一般不要用
void ui_json_AlignControlSub(cui_json uijson, chandle view)
{

    // JNIEnv* env = java_ndk_GetEnv(); 
    // jclass cls = (*env)->FindClass(env, "ui_json/NDK_ui_json");
    // jobject jthis = uijson.handle;
    // jmethodID funcAlignControlSub = (*env)->GetMethodID(env,cls, "AlignControlSub",
    //                                                     "(Landroid/view/View;)V");

    // (*env)->CallVoidMethod(env, jthis, funcAlignControlSub, view);

}//

//临时
chandle ui_json_GetParentView(chandle view)
{
    return 0;

}//

chandle create_view(chandle parentView) //实际上在 c_ui_json.h 中声明的
{
    //HANDLE view = create_windows_panel((HANDLE)parentView);  //奇怪，这样就是不行
    ////HANDLE view = create_windows_panel((HWND)parentView); //ok
    //HANDLE view = create_windows_panel(parentView);  //这样也是不行的

    chandle view = (chandle)create_windows_panel((QWidget *)parentView); //ok

    return view;
}//


//临时
chandle ui_json_CreateView(const char * _ui_class, chandle parentView)
{

    return create_view(parentView);


    return 0;

}//

//绑定函数
void ui_json_BindFunctions(cui_json * uijson)
{
    cui_json r = {0};

    //---- 原始句柄一定要注意保留!!!
    r.handle = uijson->handle;

    //--------

    //创建一个视图
    r.CreateView = CreateView;
    //r.CreateView = ui_json_CreateView;

    //从 json 中创建
    //ui_json CreateFromJson(chandle parentView, const char * fn);

    //取一个视图
    r.GetView = ui_json_GetView_c;//GetView;

    //r.GetParentView = GetParentView;
    r.GetParentView = ui_json_GetParentView;

    //取根视图
    //chandle ui_json_GetRootView(struct st_ui_json uijson);
    r.GetRootView = ui_json_GetRootView;

    //取根 json
    //chandle ui_json_GetRootJson(struct st_ui_json uijson);
    r.GetRootJson = ui_json_GetRootJson;


    //对齐各个控件
    //void ui_json_AlignControl(struct st_ui_json uijson);
    r.AlignControl = ui_json_AlignControl;

    //----
    *uijson = r;
}//


//----  虚拟 div 的操作
void UI_Div_SetTxtColor(cui_json uijson, const char * id, const char * hexColor)
{

}//

void UI_Div_SetTxt(cui_json uijson, const char * id, const char * txt)
{
    
}//

void UI_Div_SetImage(cui_json uijson, const char * id, const char * image)
{
    
}//

//临时的，和 uijson 无关 //2022
//void ui_json_UI_Div_SetImage_forObj(cui_json uijson, chandle view, const char * fn_image)
void Div_SetImage(chandle view, const char * fn_image)
{
    QLabel * label = (QLabel *)view;

    //https://www.freesion.com/article/6874503538/
    //因为是在手机中，所以是加载资源中的图片
    QImage *img = new QImage; //新建一个image对象

    //img->load(":/new/label/img/wholeBody.jpg"); //将图像资源载入对象img，注意路径，可点进图片右键复制路径
    img->load(fn_image);
    ////label->setPixmap(QPixmap::fromImage(*img)); //将图片放入label，使用setPixmap,注意指针*img
    ////label->setScaledContents(true); //图片自适应label大小  //这个会拉伸，怎样显示全部图片？默认的效果则是显示一部分

    QPixmap pixmap = QPixmap::fromImage(*img);
    label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));  // 保持比例 & 平滑缩放(无锯齿)

}//

//关闭软件键盘，iphone6s 肯定是需要的，安卓待测试
void UI_Div_CloseKeyboard(cui_json uijson)
{
    
}//

//----  虚拟 div 的操作 _end;


