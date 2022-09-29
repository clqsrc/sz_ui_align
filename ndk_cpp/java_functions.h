
#ifndef _JAVA_FUNCTIONS_H_
#define _JAVA_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------

#include <jni.h>

#include "lstring.h"

//独立的全局函数

//只调用一次，不释放也可以
void * init_java_functions_info(JNIEnv* env);

void java_ShowMessage(const char * msg, jobject jthis);
void java_View_SetOnClick(jobject view, jlong func_c);
//第一个版本传的是原始的控件作为参数，第二版本应当传递 c 传递过来的指针参数，原样在事件中返回
void java_View_SetOnClick_v2(jobject view, jlong func_c, jlong param_list);

//拉伸 contentSize 以显示所有子控件
void java_ScrollView_ShowAllChild(jobject pnlSC, int sp);

//设置点击事件不要再传递
void Functions_SetDivIs_EventTouchEnd(jobject view);

//----
//蓝牙的暂时也放这里
//创建一个蓝牙
jobject java_Blle_Create();

//设置蓝牙事件函数
void java_Blle_SetOnEvent(jobject blle, jlong func_c);

//启动蓝牙
void java_Blle_Start(jobject blle);

//根据蓝牙地址取蓝牙名
lstring * java_Blle_GetDeviceName(jobject blle, const char * mac_addr, mempool * mem);

//根据蓝牙地址取蓝牙强度
lstring * java_Blle_GetDeviceRSSI(jobject blle, const char * mac_addr, mempool * mem);

//根据蓝牙地址连接设备
void java_Blle_ConnectDevice(jobject blle, const char * mac_addr);

//关闭蓝牙连接
void java_Blle_CloseConnec(jobject blle);

//后面几个服务和 char 的函数是最复杂的
//取当前设备的服务个数
int java_Blle_GetServicesCount(jobject blle);

//取当前设备的某个服务下的节点 chr 个数
int java_Blle_GetServiceCharCount(jobject blle, int service_index);

//判断 char 是否是 notify 类型
int java_Blle_CharIsNotify(jobject blle, int service_index, int char_index);

//取 char 的 uuid
lstring * java_Blle_GetCharUUID(jobject blle, int service_index, int char_index, mempool * mem);

//发送一个字符串
void java_Blle_SendBuf(jobject blle, int service_index, int char_index, const char * buf, int buf_len);

//开始监听某个 char
void java_Blle_StartNotifyChar(jobject blle, int service_index, int char_index);

//----

void java_Functions_SetSaveValue(const char * key, const char * value);

lstring * java_Functions_GetSaveValue(const char * key, mempool * mem);

//2022.09 json 格式的全部接口
lstring * java_Functions_RunJson(const char * key, mempool * mem);



//动画平移
void java_View_Anima_toLeft(jobject view, int width, float time_second);

void java_View_Anima_Move(jobject view, int left, int top, float time_second);

//读取附加路径的文件 //fn 是相对路径
lstring * java_Functions_LoadStringFromFile_assets(const char * fn, mempool * mem);

//实际上我们的 ui_control_panel 才支持 OnSize 事件，直接传 View 类型的参数的话是错误的
void java_View_SetOnSize(jobject view, jlong func_c, jlong param_list);

//旋转视图 //参数为 视图、旋转角度、持续时间（毫秒）
void java_View_RotateAni(jobject view, int angle_360, int millisecond);

//提前  //原始的 java View 是没有这个函数的（至少不是全部的版本都有），所以实际上只能借助公用函数
void java_View_BringToFront(jobject view);

//显示或者隐藏一个 view
void Functions_ShowView(jobject view, int show);

//变更高宽比显示全部图片内容 //2021.10.25
//static public void SetIcon_ShowAll_Scale(ImageView img)
void Functions_SetIcon_ShowAll_Scale(jobject view);

lstring * Jstring2lstring(JNIEnv * env, jstring jstr, mempool * pool);
jstring CStr2Jstring(JNIEnv * env, const char * pat);

//ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
int Functions_Px2Dp(int value, jobject jthis);

//ios 里的大小直接是映射成逻辑单位的，而 android 则需要转换
int Functions_Dp2Px(int value, jobject jthis);

//单位是原始像素
void java_SetFrame(jobject view, int left, int top, int width, int height);

//单位是 dp
void java_SetFrame_dp(jobject view, int left, int top, int width, int height);

//动画显示一个 view
void Functions_ShowToLeft(jobject view);

//颜色//android 和 ios 的颜色值其实都可以转换成 int64 ，所以用它来表示颜色也没是不可以，虽然严格来说应该定义另外一种类型
_int64 Functions_hexStringToColor(const char * _s_hex);

jobject Functions_GetParentView(jobject view);

//---------------------------------------------------------------

#ifdef __cplusplus
}
#endif



#endif