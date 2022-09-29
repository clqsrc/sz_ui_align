
#ifndef _C_FUNCTIONS_H_
#define _C_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------

//#include <jni.h>

#include "base_c.h"
#include "lstring.h"
#include "c_ui_json.h"


//独立的全局函数


//暂时 //view 暂定为它的父 view
void ShowMessage_c(const char * msg, chandle view);

//暂时 //ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
//取消，用 dp 相关的函数
//int _phone_size_(int size, chandle view);

//测试 //ios 下显示所有控件
void ScrollView_ShowAllChild_c(chandle scrollView, int sp);

//取配置颜色
cint64 get_config_color_c(const char * vcolor);


//变更高宽比显示全部图片内容 //2021.10.25
void Functions_ShowImageAll_Scale(chandle view);

//设置点击事件不要再传递
void SetDivIs_EventTouchEnd(chandle view);

//动画显示窗口 //向左移动
//+ (void)AnimaWindow_toLeft:(UIView*) view
void View_Anima_toLeft(chandle _view, int width, float time_second);

//动画移动//不回到原点的版本
void View_Anima_Move(chandle _view, int left, int top, float time_second);

//读取附加路径的文件 //fn 是相对路径
//这是个特别的函数，对于 android 来说就是 assets 目录，对于 ios 来说则是一个引用的源码目录。
//之所以用 ios 的引用目录，在于放到里面的文件会自动编译进 app 而不是在 xcode 中一个个的手工加入
//所以对于 ios 来说这个是一个不固定的目录，目前来说直接指向 ui json 的目录作为根目录
lstring * Functions_LoadStringFromFile_assets(const char * fn, mempool * mem);

//保存一个值到手机中
void Functions_SetSaveValue(const char * key, const char * value);

lstring * Functions_GetSaveValue(const char * key, mempool * mem);
//2022.09
lstring * Functions_RunJson(const char * key, mempool * mem);

//----  虚拟 div 的操作
void UI_Div_SetTxtColor(cui_json uijson, const char * id, const char * hexColor);
void UI_Div_SetTxt(cui_json uijson, const char * id, const char * txt);
void UI_Div_SetImage(cui_json uijson, const char * id, const char * image);
//关闭软件键盘，iphone6s 肯定是需要的，安卓待测试
void UI_Div_CloseKeyboard(cui_json uijson);

//----  虚拟 div 的操作 _end;

//----
//蓝牙的暂时也放这里
//创建一个蓝牙
chandle Blle_Create();

//设置蓝牙事件函数
void Blle_SetOnEvent(chandle blle, cint64 func_c);

//启动蓝牙
void Blle_Start(chandle blle);

//根据蓝牙地址取蓝牙名
lstring * Blle_GetDeviceName(chandle blle, const char * mac_addr, mempool * mem);

//根据蓝牙地址连接设备
void Blle_ConnectDevice(chandle blle, const char * mac_addr);

//关闭蓝牙连接
void Blle_CloseConnec(chandle blle);

//根据蓝牙地址取蓝牙信号强度
lstring * Blle_GetDeviceRSSI(chandle blle, const char * mac_addr, mempool * mem);

//后面几个服务和 char 的函数是最复杂的
//取当前设备的服务个数
int Blle_GetServicesCount(chandle blle);

//取当前设备的某个服务下的节点 chr 个数
int Blle_GetServiceCharCount(chandle blle, int service_index);

//判断 char 是否是 notify 类型
int Blle_CharIsNotify(chandle blle, int service_index, int char_index);

//取 char 的 uuid
lstring * Blle_GetCharUUID(chandle blle, int service_index, int char_index, mempool * mem);

//发送一个字符串
void Blle_SendBuf(chandle blle, int service_index, int char_index, const char * buf, int buf_len);

//开始监听某个 char
void Blle_StartNotifyChar(chandle blle, int service_index, int char_index);

//----
//蓝牙的暂时在这里，那么 http 的也放这里好了 //因为没有创建实例，所以其实是单实例的，所以只能在 ui 主线程中调用这些函数

//http 访问及相关的接收事件设置
void HttpPost_AndSetOnEvent(const char * url, cint64 func_c, cint64 param_c);

//下载到文件
void HttpGetToFile_AndSetOnEvent(const char * url, const char * fn, cint64 func_c, cint64 param_c);

void HttpPost_SetParam(const char * key, const char * value);

void HttpPost_ClearParam();

//----
//方便性的函数
int Functions_StrToInt(const char * s);
double Functions_StrToFloat(const char * s);

//这个函数很重要，要仔细实现 //必要时自己实现
cint64 Functions_StrToInt64(const char * s);
//----

//16 进制的颜色//android 和 ios 的颜色值其实都可以转换成 int64 ，所以用它来表示颜色也没是不可以，虽然严格来说应该定义另外一种类型
cint64 HexColor(const char * hex);

cint64 Functions_HexColor(const char * hex);

//方便写代码而已 //其实可以用宏代替 c++ 的作用域 //要不都写 Functions_ 前缀的话代码就太不可读了，就像 gtk 源码一样
//当和其他库有冲突时取消这个定义就可以了
#define _use_functions_
#ifdef  _use_functions_

#define Color Functions_HexColor

#endif  //_use_functions_

//define可以替代多行的代码，例如MFC中的宏定义（非常的经典，虽然让人看了恶心）
#define   MACRO(arg1,   arg2)   do   {   \
   \
stmt1;   \
stmt2;   \
   \
}   while(0)


//模拟 c++ 的 use //不行

#define  _USE(arg1,   arg2)   do   {   \
   \
//stmt1;   \
//define arg1\
//stmt2;   \
   \
}   while(0)


#define  _USE2(arg1,   arg2)   do   {   \
   \
   define arg1\
    int i = 0;  \
   \
}   while(0)




//---------------------------------------------------------------

#ifdef __cplusplus
}
#endif



#endif