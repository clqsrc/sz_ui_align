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

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

//#include "lua/libs_lua.c"

#include "cpp_hashmap.h"
#include "lua_functions_run.c" //在 c 语言中运行 lua 所需要的环境的相关函数都在这里
#include "lua_functions_newloader.c" //对于没有文件系统的 lua 环境，要自己实现加载器

//lua_functions.c 是给 lua 调用的函数集合

//---------------------------------------------------------------
chandle _lua_param_chandle_(lua_State * lua, int index);

//目前的 ui_json 参数比较啰嗦
cui_json _lua_param_uijson_(lua_State * lua, int index);
//----------------------------------------------------------------


//https://www.cnblogs.com/lijiajia/p/8284328.html
//给 lua 调用的函数
//lua_register(L, "SayHello", l_SayHello);//注册C函数到lua

static int l_SayHello_sample(lua_State *L)
{
    const char *d = luaL_checkstring(L, 1);//获取参数，字符串类型
    int len = strlen(d);
    char str[100] = "hello ";
    strcat(str, d);
    lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    return 1;
}//

//记录下 onsize 等事件参数对应的 lua_State ，因为要在事件中找到它们再调用原来的 lua 环境
//其实 onclick 现在也用这个
chandle * lua_OnSize_Param2lua_State = NULL;  //是个 hashmap

//为方便，程序中只用一个 lua 运行环境
lua_State * GUI_LUA_EVN = NULL;


//接受两个字符串参数
static int c2l_UI_CreateFromJson(lua_State * l)
{
    //if (NULL == lua_OnSize_Param2lua_State) lua_OnSize_Param2lua_State = create_hashmap();
    
    //------------------------------------------------
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    const char * _parentView = luaL_checkstring(l, 1);//获取参数，字符串类型
    
    //chandle parentView = Functions_StrToInt(_parentView);
    //chandle parentView = (chandle)Functions_StrToInt64(_parentView);
    chandle parentView = (chandle)Functions_lua_StrToInt64(_parentView);

    
    const char * fn = luaL_checkstring(l, 2);//获取参数，字符串类型

    cui_json uijson = UI_CreateFromJson(parentView, fn);
    
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)uijson.handle);
    
    //lua_pushstring(l, uijson.handle);
    lua_pushstring(l, s_handle.str);

    return 1;
}//

//uijson 中的 onsize 事件分发给 lua 代码
void Functions_lua_ui_json_onSize(chandle param)
{
    mempool * mem = newmempool();
    
    lua_State * l = NULL; //目前只能取全局变量
    l = (lua_State *)hashmap_get(lua_OnSize_Param2lua_State, (cint64)param); //取出运行环境
    
    //在事件发生时调用 lua 函数即可
    //lua 函数名
    lua_getglobal(l, "UI_OnSize");

    
    cint64 int64_param = (cint64)param;
    lua_str_param s_param = Functions_lua_Int64ToStr(int64_param);
    
    //参数入栈
    lua_pushstring(l, s_param.str);
    
    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);
    
    //开始调用函数，有1个参数，0个返回值
    //lua_call(l, 1, 0);  //这个出错的话会退出程序
    
    //luaL_loadfile(L, "test_01.lua");
    //status=lua_pcall(L, 0, LUA_MULTRET,0);
    
    //int status = lua_pcall(l, 0, LUA_MULTRET,0);
    //开始调用函数，有1个参数，0个返回值
    int status = lua_pcall(l, 1, LUA_MULTRET, 0);
    
    if (status != LUA_OK)
    {
        //将错误代码和栈顶的字符串打印出来
        printf("error code:%d,msg:%s\r\n",status,lua_tostring(l, -1));
        
        lstring * s = NewString("Lua run error [Functions_lua_ui_json_onSize]:", mem);
        s->AppendConst(s, lua_tostring(l, -1));
        
        //ShowMessage_c(lua_tostring(l, -1), 0);
        ShowMessage_c(s->str, 0);

    }//if

    //lua_close(L);
    
    freemempool(mem);
}//

//uijson 中的 onclick 事件分发给 lua 代码
void Functions_lua_ui_json_onClick(chandle param)
{
    
    lua_State * l = NULL; //目前只能取全局变量
    l = (lua_State *)hashmap_get(lua_OnSize_Param2lua_State, (cint64)param); //取出运行环境
    
    //在事件发生时调用 lua 函数即可
    //函数名
    lua_getglobal(l, "UI_OnClick");

    
    cint64 int64_param = (cint64)param;
    lua_str_param s_param = Functions_lua_Int64ToStr(int64_param);
    
    //参数入栈
    lua_pushstring(l, s_param.str);
    
    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);
    
    //开始调用函数，有1个参数，0个返回值
    //lua_call(l, 1, 0);
    safe_lua_call(l, 1, 0, __FUNCTION__);
    
}//

//通用的回调事件 //目前 http 等都在这里 //固定有两个参数传递过来
void Functions_lua_onEvent(chandle param, const char * key, const char * value)
{

    lua_State * lua = NULL; //目前只能取全局变量
    lua = (lua_State *)hashmap_get(lua_OnSize_Param2lua_State, (cint64)param); //取出运行环境

    //在事件发生时调用 lua 函数即可
    //函数名
    //lua_getglobal(l, "UI_OnClick");
    lua_getglobal(lua, "UI_OnEvent");


    cint64 int64_param = (cint64)param;
    lua_str_param s_param = Functions_lua_Int64ToStr(int64_param);

    //参数入栈
    lua_pushstring(lua, s_param.str);
    lua_pushstring(lua, key);
    lua_pushstring(lua, value);

    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);

    //开始调用函数，有1个参数，0个返回值
    //lua_call(lua, 1, 0);
    //safe_lua_call(lua, 1, 0, __FUNCTION__);

    //开始调用函数，有3个参数，0个返回值
    safe_lua_call(lua, 3, 0, __FUNCTION__);

}//

//ui 定时器事件分发给 lua 的代码
void Functions_lua_UI_OnTime()
{
    //if (1 == error_at_Functions_lua_run) return;  //2023 如果脚本本身有错误，这个运行也无用

    Functions_lua_RunFunc(GUI_LUA_EVN, 0, "UI_OnTime",
                          NULL, //s_handle.str,
                          NULL, //key, //Functions_lua_Int64ToStr(parentView).str,
                          NULL, //value,
                          NULL, NULL, NULL);
    
}//


//蓝牙事件分发给 lua 的代码 //流程有点乱，以后再整理
//void CarMainForm_Blle_OnEvent(chandle blle_obj, const char * key, const char * value)
void Functions_lua_Blle_OnEvent(chandle blle_obj, const char * key, const char * value)
{
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)blle_obj);
    
    Functions_lua_RunFunc(GUI_LUA_EVN, 3, "Blle_OnEvent",
                          s_handle.str,
                          key, //Functions_lua_Int64ToStr(parentView).str,
                          value,
                          NULL, NULL, NULL);
    
}//

//根据蓝牙地址取蓝牙名
static int c2l_Blle_GetDeviceName(lua_State * lua)
{
    mempool * mem = newmempool();
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    chandle blle = _lua_param_chandle_(lua, 1);
    
    const char * mac_addr = luaL_checkstring(lua, 2);//获取参数，字符串类型
    
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)uijson.handle);
    
    //lua_pushstring(l, uijson.handle);
    //lua_pushstring(l, s_handle.str);
    
    //----
    lstring * deviceName = Blle_GetDeviceName(blle, mac_addr, mem);
    
    lua_pushstring(lua, deviceName->str);

    freemempool(mem);
    
    return 1;
}//

//根据蓝牙地址取蓝牙强度
static int c2l_Blle_GetDeviceRSSI(lua_State * lua)
{
    mempool * mem = newmempool();

    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    chandle blle = _lua_param_chandle_(lua, 1);

    const char * mac_addr = luaL_checkstring(lua, 2);//获取参数，字符串类型

    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str

    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)uijson.handle);

    //lua_pushstring(l, uijson.handle);
    //lua_pushstring(l, s_handle.str);

    //----
    lstring * deviceRSSI = Blle_GetDeviceRSSI(blle, mac_addr, mem);

    lua_pushstring(lua, deviceRSSI->str);

    freemempool(mem);

    return 1;
}//



//设置 ui 的 on size 事件  //两个参数，一个是父窗口，一个是函数名（或者是函数名的索引，看情况）
static int c2l_UI_SetOnSize(lua_State * l)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    const char * _parentView = luaL_checkstring(l, 1);//获取参数，字符串类型

    chandle parentView = (chandle)Functions_lua_StrToInt64(_parentView);

    
    const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    
    cint64 i_func = Functions_lua_StrToInt64(func);

    //cui_json uijson = UI_CreateFromJson(parentView, fn);
    
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)uijson.handle);
    
    //lua_pushstring(l, uijson.handle);
    //lua_pushstring(l, s_handle.str);
    
    //----
    //View_SetOnSize(parentView, Functions_lua_ui_json_onSize, NULL);
    //View_SetOnSize(parentView, Functions_lua_ui_json_onSize, (chandle)func);
    
    hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    View_SetOnSize(parentView, Functions_lua_ui_json_onSize, (chandle)i_func);

    return 1;
}//

//lua 默认不支持 int64 ，所以我们全部当做字符串好了
//取出参数 chandle 类型
//index -- 第几个参数，注意是从 1 开始的
chandle _lua_param_chandle_(lua_State * lua, int index)
{
    //const char * _view = luaL_checkstring(l, 2);//获取参数，字符串类型
    const char * _view = luaL_checkstring(lua, index);//获取参数，字符串类型
    chandle view = (chandle)Functions_lua_StrToInt64(_view);
    
    return view;
}//

//目前的 ui_json 参数比较啰嗦
cui_json _lua_param_uijson_(lua_State * lua, int index)
{
    chandle uijson_handle = _lua_param_chandle_(lua, 1);
    
    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    cui_json uijson = {};
    uijson.handle = uijson_handle;
    ui_json_BindFunctions(&uijson);
    
    return uijson;
}//

//设置 ui 的 on click 事件  //两个参数，一个是父窗口，一个是函数名（或者是函数名的索引，看情况）
static int c2l_UI_SetOnClick(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _view = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle view = (chandle)Functions_lua_StrToInt64(_view);
    
    //const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);
    chandle view = _lua_param_chandle_(lua, 1);
    chandle func = _lua_param_chandle_(lua, 2);

    //----
    
    //hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    //View_SetOnClick(view, Functions_lua_ui_json_onClick, (chandle)i_func);

    hashmap_put(lua_OnSize_Param2lua_State, (cint64)func, (cint64)lua); //保存运行环境
    View_SetOnClick(view, Functions_lua_ui_json_onClick, func);

    return 1;
}//

//http 访问
static int c2l_UI_HttpPost(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _view = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle view = (chandle)Functions_lua_StrToInt64(_view);

    //const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);
    //chandle view = _lua_param_chandle_(lua, 1);
    const char * url = luaL_checkstring(lua, 1);//获取参数，字符串类型
    chandle func = _lua_param_chandle_(lua, 2);

    //----

    //hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    //View_SetOnClick(view, Functions_lua_ui_json_onClick, (chandle)i_func);

    hashmap_put(lua_OnSize_Param2lua_State, (cint64)func, (cint64)lua); //保存运行环境

    //View_SetOnClick(view, Functions_lua_onEvent, func);
    //HttpPost_AndSetOnEvent("http://www.newbt.net/ms/vdisk/", CarMainForm_Http_OnEvent, NULL);
    HttpPost_AndSetOnEvent(url, Functions_lua_onEvent, func);

    return 1;
}//

static int c2l_UI_HttpPost_SetParam(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _view = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle view = (chandle)Functions_lua_StrToInt64(_view);

    //const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);
    //chandle view = _lua_param_chandle_(lua, 1);
    const char * key = luaL_checkstring(lua, 1);//获取参数，字符串类型
    const char * value = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //chandle func = _lua_param_chandle_(lua, 2);

    //----

    //hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    //View_SetOnClick(view, Functions_lua_ui_json_onClick, (chandle)i_func);

    //hashmap_put(lua_OnSize_Param2lua_State, (cint64)func, (cint64)lua); //保存运行环境

    //View_SetOnClick(view, Functions_lua_onEvent, func);
    //HttpPost_AndSetOnEvent("http://www.newbt.net/ms/vdisk/", CarMainForm_Http_OnEvent, NULL);
    HttpPost_SetParam(key, value);

    return 1;
}//

static int c2l_UI_HttpPost_ClearParam(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _view = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle view = (chandle)Functions_lua_StrToInt64(_view);

    //const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);
    //chandle view = _lua_param_chandle_(lua, 1);
    //const char * key = luaL_checkstring(lua, 1);//获取参数，字符串类型
    //const char * value = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //chandle func = _lua_param_chandle_(lua, 2);

    //----

    //hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    //View_SetOnClick(view, Functions_lua_ui_json_onClick, (chandle)i_func);

    //hashmap_put(lua_OnSize_Param2lua_State, (cint64)func, (cint64)lua); //保存运行环境

    //View_SetOnClick(view, Functions_lua_onEvent, func);
    //HttpPost_AndSetOnEvent("http://www.newbt.net/ms/vdisk/", CarMainForm_Http_OnEvent, NULL);
    HttpPost_ClearParam();

    //return 1;
    return 0;
}//

//下载到文件
static int c2l_UI_HttpGetToFile(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _view = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle view = (chandle)Functions_lua_StrToInt64(_view);

    //const char * func = luaL_checkstring(l, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);
    //chandle view = _lua_param_chandle_(lua, 1);
    const char * url = luaL_checkstring(lua, 1);//获取参数，字符串类型
    const char * fn = luaL_checkstring(lua, 2);//获取参数，字符串类型
    chandle func = _lua_param_chandle_(lua, 3);

    //----

    //hashmap_put(lua_OnSize_Param2lua_State, i_func, (cint64)l); //保存运行环境
    //View_SetOnClick(view, Functions_lua_ui_json_onClick, (chandle)i_func);

    hashmap_put(lua_OnSize_Param2lua_State, (cint64)func, (cint64)lua); //保存运行环境

    //View_SetOnClick(view, Functions_lua_onEvent, func);
    //HttpPost_AndSetOnEvent("http://www.newbt.net/ms/vdisk/", CarMainForm_Http_OnEvent, NULL);
    HttpGetToFile_AndSetOnEvent(url, fn, Functions_lua_onEvent, func);

    return 1;
}//

static int c2l_UI_GetView(lua_State * lua)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //const char * _uijson_handle = luaL_checkstring(l, 1);//获取参数，字符串类型
    //chandle uijson_handle = (chandle)Functions_lua_StrToInt64(_uijson_handle);
    
    chandle uijson_handle = _lua_param_chandle_(lua, 1);
    
    const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    cui_json uijson = {};
    uijson.handle = uijson_handle;
    ui_json_BindFunctions(&uijson);
    
    chandle view = uijson.GetView(uijson, id);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    lua_pushstring(lua, s_handle.str);

    return 1;
}//


static int c2l_UI_GetRootView(lua_State * lua)
{

    chandle uijson_handle = _lua_param_chandle_(lua, 1);
    
    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    cui_json uijson = {};
    uijson.handle = uijson_handle;
    ui_json_BindFunctions(&uijson);
    
    chandle view = uijson.GetRootView(uijson);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    lua_pushstring(lua, s_handle.str);

    return 1;
}//

static int c2l_UI_AlignControl(lua_State * lua)
{

    chandle uijson_handle = _lua_param_chandle_(lua, 1);
    
    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    cui_json uijson = {};
    uijson.handle = uijson_handle;
    ui_json_BindFunctions(&uijson);
    
    //chandle view = uijson.GetRootView(uijson);
    //ui_json_AlignControl
    uijson.AlignControl(uijson);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str
    
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    //lua_pushstring(lua, s_handle.str);

    return 1;
}//

static int c2l_long_ref(lua_State * lua)
{

    chandle obj = _lua_param_chandle_(lua, 1);

    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    chandle obj_long_ref = long_ref(obj); //取得长效指针


    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok

    //----
    //lua_pushstring(L, str); //返回给lua的值压栈 //想当于 return str

    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)obj_long_ref);

    lua_pushstring(lua, s_handle.str);

    return 1;  //应当是返回值个数
}//


static int c2l_View_GetRect(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    
    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    crect rect = View_GetRect(view);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);
    lua_str_param s_json = {};
    //sprintf(s_json.str,
    //        "{left:%d, top:%d, width:%d, height:%d}",
    //        rect.left, rect.top, rect.width, rect.height); //no // lua 的 json ，key 一定要加双引号
    
    sprintf(s_json.str,
            "{\"left\":%d, \"top\":%d, \"width\":%d, \"height\":%d}",
            rect.left, rect.top, rect.width, rect.height); //lua 的 json ，key 一定要加双引号
    
    lua_pushstring(lua, s_json.str);

    return 1;
}//

//取当前的系统语言
//默认 "cn" ， "en" 则是英语
static int c2l_GetOSLanguage(lua_State * lua)
{

    mempool * mem = newmempool();
    //chandle view = _lua_param_chandle_(lua, 1);
    
    
    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    //crect rect = View_GetRect(view);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);
    lua_str_param s_r = {};
    
    lstring * oslg = GetOSLanguage(mem);
    
    strcpy(s_r.str, oslg->str);
    
//    sprintf(s_json.str,
//            "{\"left\":%d, \"top\":%d, \"width\":%d, \"height\":%d}",
//            rect.left, rect.top, rect.width, rect.height); //lua 的 json ，key 一定要加双引号
    
    lua_pushstring(lua, s_r.str);
                  
    freemempool(mem);

    return 1;
}//

static int c2l_View_GetRect_dp(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);

    //const char * id = luaL_checkstring(lua, 2);//获取参数，字符串类型
    //cint64 i_func = Functions_lua_StrToInt64(func);

    //----
    crect rect = View_GetRect_dp(view);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok

    //----

    //lua_pushstring(lua, s_handle.str);
    lua_str_param s_json = {};
    //sprintf(s_json.str,
    //        "{left:%d, top:%d, width:%d, height:%d}",
    //        rect.left, rect.top, rect.width, rect.height); //no // lua 的 json ，key 一定要加双引号

    sprintf(s_json.str,
            "{\"left\":%d, \"top\":%d, \"width\":%d, \"height\":%d}",
            rect.left, rect.top, rect.width, rect.height); //lua 的 json ，key 一定要加双引号

    lua_pushstring(lua, s_json.str);

    return 1;
}//


static int c2l_View_SetRect(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    
    //获取参数，5.3 之后的 int64 类型
    //但目前 lua 暂时只有 tonumber，而没有 tointeger 所以类型太容易错误。还是先转成浮点数吧
    //int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
    //int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
    //int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
    //int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型
    int left   = (int)luaL_checknumber(lua, 2);//获取参数，5.3 之后的 int64 类型
    int top    = (int)luaL_checknumber(lua, 3);//获取参数，5.3 之后的 int64 类型
    int width  = (int)luaL_checknumber(lua, 4);//获取参数，5.3 之后的 int64 类型
    int height = (int)luaL_checknumber(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    crect rect = {};
    rect.left = left;
    rect.top = top;
    rect.width = width;
    rect.height = height;

    View_SetRect(view, rect);
    //View_SetRect_dp(view, rect);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

static int c2l_View_SetRect_dp(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);

    //获取参数，5.3 之后的 int64 类型
    //但目前 lua 暂时只有 tonumber，而没有 tointeger 所以类型太容易错误。还是先转成浮点数吧
    //int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
    //int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
    //int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
    //int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型
    int left   = (int)luaL_checknumber(lua, 2);//获取参数，5.3 之后的 int64 类型
    int top    = (int)luaL_checknumber(lua, 3);//获取参数，5.3 之后的 int64 类型
    int width  = (int)luaL_checknumber(lua, 4);//获取参数，5.3 之后的 int64 类型
    int height = (int)luaL_checknumber(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    crect rect = {};
    rect.left = left;
    rect.top = top;
    rect.width = width;
    rect.height = height;

    //View_SetRect(view, rect);
    View_SetRect_dp(view, rect);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok

    //----

    //lua_pushstring(lua, s_handle.str);


    return 0;  //这个是指返回值的个数
}//


static int c2l_Functions_ShowImageAll_Scale(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    Functions_ShowImageAll_Scale(view);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//


static int c2l_View_BringToFront(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    View_BringToFront(view);
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//


static int c2l_View_SetBackgroundColor(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    
    const char * hex_color = luaL_checkstring(lua, 2);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    View_SetBackgroundColor(view, HexColor(hex_color));

    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

static int c2l_UI_Div_SetTxtColor(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * view_id = luaL_checkstring(lua, 2);

    const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    UI_Div_SetTxtColor(uijson, view_id, hex_color);


    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;  //这个指的是返回值的个数
}//

//关闭软键盘
static int c2l_UI_Div_CloseKeyboard(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    UI_Div_CloseKeyboard(uijson);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 0; //return 1;  //这个指的是返回值的个数
}//

static int c2l_UI_Div_SetImage(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * view_id = luaL_checkstring(lua, 2);

    const char * image = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    UI_Div_SetImage(uijson, view_id, image);


    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

static int c2l_UI_Div_SetTxt(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * view_id = luaL_checkstring(lua, 2);

    const char * txt = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    UI_Div_SetTxt(uijson, view_id, txt);


    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

//通过 对象 设置标题 //为了支持动态创建，确实还是需要这样的另一个版本的函数的
static int c2l_UI_Div_SetTxt_forObj(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 2);

    const char * txt = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    ui_json_UI_Div_SetTxt_forObj(uijson, view, txt);


    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

//设置一个 uijson 页面的语言包
static int c2l_UI_SetLanguage(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    //chandle view = _lua_param_chandle_(lua, 2);

    const char * lng = luaL_checkstring(lua, 2);
    

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    ui_json_UI_SetLanguage(uijson, lng);


    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 1;
}//

//取 edit 内容
static int c2l_View_Edit_GetText(lua_State * lua)
{
    mempool * mem = newmempool();

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 1);

    //const char * txt = luaL_checkstring(lua, 2);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    lstring * txt = View_Edit_GetText(view, mem);
    
    //----

    lua_pushstring(lua, txt->str);
    
    freemempool(mem);

    return 1; //返回值的个数
}//

//语言函数
static int c2l_UI_LG(lua_State * lua)
{
    mempool * mem = newmempool();

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //chandle view = _lua_param_chandle_(lua, 1);

    const char * key = luaL_checkstring(lua, 1);
    const char * cn_ref = luaL_checkstring(lua, 2);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    lstring * txt = UI_LG(key, cn_ref, mem);
    
    //----

    lua_pushstring(lua, txt->str);
    
    freemempool(mem);

    return 1; //返回值的个数
}//


static int c2l_View_Edit_SetText(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 1);

    const char * txt = luaL_checkstring(lua, 2);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    View_Edit_SetText(view, txt);
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 0; //返回值的个数
}//


//设置为密码框
static int c2l_View_Edit_SetPass(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 1);

    //const char * txt = luaL_checkstring(lua, 2);
    int as_pass   = (int)luaL_checknumber(lua, 2);//获取参数，5.3 之后的 int64 类型

//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----

    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    View_Edit_SetPass(view, as_pass);

    //----

    //lua_pushstring(lua, s_handle.str);

    return 0; //返回值的个数
}//

static int c2l_UI_Div_SetTxtColor_forObj(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 2);

    const char * color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    ui_json_UI_Div_SetTxtColor_forObj(uijson, view, color);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 0; //参数的个数，最好还是写正确
}//

static int c2l_UI_Div_SetImage_forObj(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 2);

    const char * fn_image = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    //ui_json_UI_Div_SetTxtColor_forObj(uijson, view, color);
    ui_json_UI_Div_SetImage_forObj(uijson, view, fn_image);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 0; //参数的个数，最好还是写正确
}//

//字符串居左边
static int c2l_UI_Div_SetTxtAlignLeft_forObj(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 2);

    //const char * color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    ui_json_UI_Div_SetTxtAlignLeft(uijson, view);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 1;
}//


//设置圆角
static int c2l_UI_Div_SetRadius_forObj(lua_State * lua)
{

    //chandle view = _lua_param_chandle_(lua, 1);
    cui_json uijson = _lua_param_uijson_(lua, 1);
    chandle view = _lua_param_chandle_(lua, 2);

    const char * radius = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");
    ui_json_UI_Div_SetRadius_forObj(uijson, view, radius);

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);

    return 1;
}//

//隐藏视图
static int c2l_View_Hide(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");

    View_Hide(view);
    
    
    
    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

static int c2l_View_Show(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");

    View_Show(view);
    

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//

//动画移动一个视图
static int c2l_View_Anima_Move(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
    double time_second  = luaL_checknumber(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    View_Anima_Move(view, left, top, time_second);
    

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    //lua_pushstring(lua, s_handle.str);


    return 0;  //返回参数的个数
}//

//滚动视图拉伸内容高度，以显示所有控件
static int c2l_ScrollView_ShowAllChild(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----

    //View_Show(view);
    ScrollView_ShowAllChild_c(view, 0);
    

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----

    //lua_pushstring(lua, s_handle.str);


    return 1;
}//


static int c2l_GetParentView(lua_State * lua)
{

    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");

    chandle parentView = GetParentView(view);
    

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)parentView);

    lua_pushstring(lua, s_handle.str);


    return 1;
}//


static int c2l_Blle_ConnectDevice(lua_State * lua)
{
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * mac_addr = luaL_checkstring(lua, 2);

    //----
    Blle_ConnectDevice(blle_obj, mac_addr);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)parentView);

    //lua_pushstring(lua, s_handle.str);

    //return 1;
    return 0;  //这个是指返回参数的个数
}//

static int c2l_Blle_CloseConnect(lua_State * lua)
{
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);

    //----
    Blle_CloseConnec(blle_obj);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)parentView);

    //lua_pushstring(lua, s_handle.str);

    //return 1;
    return 0;  //这个是指返回参数的个数
}//

//取当前设备的服务个数
static int c2l_Blle_GetServicesCount(lua_State * lua)
{
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);

    //----
    int r = Blle_GetServicesCount(blle_obj);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    lua_pushstring(lua, s_r.str);

    return 1;
}//

//取当前设备的某个服务下的节点 chr 个数
static int c2l_Blle_GetServiceCharCount(lua_State * lua)
{
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    cint64 service_index = luaL_checkinteger(lua, 2);

    //----
    int r = Blle_GetServiceCharCount(blle_obj, (int)service_index);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    lua_pushstring(lua, s_r.str);

    return 1;
}//


static int c2l_Blle_GetCharUUID(lua_State * lua)
{
    mempool * mem = newmempool();
    
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    cint64 service_index = luaL_checkinteger(lua, 2);
    cint64 char_index = luaL_checkinteger(lua, 3);

    //----
    lstring * uuid = Blle_GetCharUUID(blle_obj, (int)service_index, (int)char_index, mem);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    lua_pushstring(lua, uuid->str);
    
    freemempool(mem);

    return 1;
}//

//发送一个字符串 //可以的，测试 ok 
static int c2l_Blle_SendBuf(lua_State * lua)
{
    //mempool * mem = newmempool();
    
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    cint64 service_index = luaL_checkinteger(lua, 2);
    cint64 char_index = luaL_checkinteger(lua, 3);
    
    const char * str_to_send = luaL_checkstring(lua, 4);  //要发送的内容

    int len = (int)strlen(str_to_send);
    
    //----
    //lstring * uuid = Blle_GetCharUUID(blle_obj, (int)service_index, (int)char_index, mem);
    
    Blle_SendBuf(blle_obj, (int)service_index, (int)char_index, str_to_send, len);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushstring(lua, uuid->str);
    
    //freemempool(mem);

    return 1;
}//


//是否可监听
static int c2l_Blle_CharIsNotify(lua_State * lua)
{
    //mempool * mem = newmempool();
    
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    cint64 service_index = luaL_checkinteger(lua, 2);
    cint64 char_index = luaL_checkinteger(lua, 3);

    //----
    int is_notify = Blle_CharIsNotify(blle_obj, (int)service_index, (int)char_index);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    lua_pushinteger(lua, is_notify);
    
    //freemempool(mem);

    return 1;
}//

//旋转视图 //参数为 视图、旋转角度、持续时间（毫秒）
//void View_RotateAni(chandle view, int angle_360, int millisecond)
static int c2l_View_RotateAni(lua_State * lua)
{
    //mempool * mem = newmempool();
    
    chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    //cint64 angle_360 = luaL_checkinteger(lua, 2);
    double angle_360 = luaL_checknumber(lua, 2);
    cint64 millisecond = luaL_checkinteger(lua, 3);

    //----
    View_RotateAni(view, (int)angle_360, (int)millisecond);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushinteger(lua, is_notify);
    
    //freemempool(mem);

    return 0; // 这个应该是返回值的个数
}//

static int c2l_GetSaveValue(lua_State * lua)
{
    mempool * mem = newmempool();
    
    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * key = luaL_checkstring(lua, 1);
    //cint64 angle_360 = luaL_checkinteger(lua, 2);
    //cint64 millisecond = luaL_checkinteger(lua, 3);

    //----
    //View_RotateAni(view, (int)angle_360, (int)millisecond);
    
    lstring * value = Functions_GetSaveValue(key, mem);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushinteger(lua, is_notify);
    lua_pushstring(lua, value->str);

    freemempool(mem);

    return 1; // 这个应该是返回值的个数
}//

//2022.09
static int c2l_RunJson(lua_State * lua)
{
    mempool * mem = newmempool();

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * key = luaL_checkstring(lua, 1);
    //cint64 angle_360 = luaL_checkinteger(lua, 2);
    //cint64 millisecond = luaL_checkinteger(lua, 3);

    //----
    //View_RotateAni(view, (int)angle_360, (int)millisecond);

    lstring * value = Functions_RunJson(key, mem);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushinteger(lua, is_notify);
    lua_pushstring(lua, value->str);

    freemempool(mem);

    return 1; // 这个应该是返回值的个数
}//


//2023 //允许带一个 bin 参数，里面是原始的二进制内容,不过 java 端目前也是用不上
static int c2l_RunJsonBin(lua_State * lua)
{
    mempool * mem = newmempool();

    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * str_json = luaL_checkstring(lua, 1);
    const char * str_bin = luaL_checkstring(lua, 12);
    //cint64 angle_360 = luaL_checkinteger(lua, 2);
    //cint64 millisecond = luaL_checkinteger(lua, 3);

    //----
    //View_RotateAni(view, (int)angle_360, (int)millisecond);

    lstring * value = Functions_RunJsonBin(str_json, str_bin, mem);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushinteger(lua, is_notify);
    lua_pushstring(lua, value->str);

    freemempool(mem);

    return 1; // 这个应该是返回值的个数
}//


static int c2l_SetSaveValue(lua_State * lua)
{
    //mempool * mem = newmempool();
    
    //chandle view = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * key = luaL_checkstring(lua, 1);
    const char * value = luaL_checkstring(lua, 2);
    //cint64 angle_360 = luaL_checkinteger(lua, 2);
    //cint64 millisecond = luaL_checkinteger(lua, 3);

    //----
    //View_RotateAni(view, (int)angle_360, (int)millisecond);
    
    Functions_SetSaveValue(key, value);

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushinteger(lua, is_notify);
    //lua_pushstring(lua, value->str);

    //freemempool(mem);

    return 0; // 这个应该是返回值的个数
}//


//开始监听
static int c2l_Blle_StartNotifyChar(lua_State * lua)
{
    //mempool * mem = newmempool();
    
    chandle blle_obj = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * mac_addr = luaL_checkstring(lua, 2);
    cint64 service_index = luaL_checkinteger(lua, 2);
    cint64 char_index = luaL_checkinteger(lua, 3);

    //----
    Blle_StartNotifyChar(blle_obj, (int)service_index, (int)char_index);
      
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    //lua_str_param s_r = Functions_lua_Int64ToStr((cint64)r);

    //lua_pushstring(lua, uuid->str);
    
    //freemempool(mem);

    return 1;
}//

//创建一个视图
static int c2l_CreateView(lua_State * lua)
{

    chandle parentView = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    //const char * view_id = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);
    
//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----
    
    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");

    chandle view = CreateView(parentView);
    

    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok
    
    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    lua_pushstring(lua, s_handle.str);


    return 1;
}//

//创建一个视图
static int c2l_UI_CreateViewClass(lua_State * lua)
{

    chandle parentView = _lua_param_chandle_(lua, 1);
    //cui_json uijson = _lua_param_uijson_(lua, 1);
    const char * ui_class = luaL_checkstring(lua, 2);

    //const char * hex_color = luaL_checkstring(lua, 3);

//    int left   = (int)luaL_checkinteger(lua, 2);//获取参数，5.3 之后的 int64 类型
//    int top    = (int)luaL_checkinteger(lua, 3);//获取参数，5.3 之后的 int64 类型
//    int width  = (int)luaL_checkinteger(lua, 4);//获取参数，5.3 之后的 int64 类型
//    int height = (int)luaL_checkinteger(lua, 5);//获取参数，5.3 之后的 int64 类型

    //----

    //View_SetBackgroundColor(view, HexColor("#0000FF00"));
    //UI_Div_SetTxtColor(uijson, "txtDriver", "#ffffff");

    //chandle view = CreateView(parentView);
    chandle view = UI_CreateViewClass(parentView, ui_class);


    //View_SetBackgroundColor(view, HexColor("#FF0000")); //test ok

    //----
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    lua_str_param s_handle = Functions_lua_Int64ToStr((cint64)view);

    lua_pushstring(lua, s_handle.str);


    return 1;
}//

//
static int c2l_ShowMessage(lua_State * l)
{
    //lua 默认不支持 int64 ，所以我们全部当做字符串好了
    const char * s = luaL_checkstring(l, 1);//获取参数，字符串类型

    ShowMessage_c(s, 0);

    return 1;
}//

//自定义的 require 函数
static int c2l_self_require(lua_State * l)
{
    //第一个参数是文件名
    const char * fn = luaL_checkstring(l, 1);//获取参数，字符串类型
   
    Functions_lua_run(l, fn);
    
    return 1;
}//

//注册C函数到 lua
void RegAll_C2LuaFunctions(lua_State * l)
{
    if (NULL == lua_OnSize_Param2lua_State) lua_OnSize_Param2lua_State = create_hashmap();
 
    //------------------------------------------------
    
    lua_register(l, "c2l_UI_CreateFromJson", c2l_UI_CreateFromJson);
    lua_register(l, "c2l_UI_SetOnSize", c2l_UI_SetOnSize);
    lua_register(l, "c2l_ShowMessage", c2l_ShowMessage);
    lua_register(l, "c2l_self_require", c2l_self_require);
    lua_register(l, "c2l_UI_GetView", c2l_UI_GetView);
    lua_register(l, "c2l_UI_GetRootView", c2l_UI_GetRootView);
    lua_register(l, "c2l_long_ref", c2l_long_ref); //取长效指针
    lua_register(l, "c2l_UI_AlignControl", c2l_UI_AlignControl);
    lua_register(l, "c2l_View_GetRect", c2l_View_GetRect);
    lua_register(l, "c2l_View_GetRect_dp", c2l_View_GetRect_dp);
    lua_register(l, "c2l_View_SetRect", c2l_View_SetRect);
    lua_register(l, "c2l_View_SetRect_dp", c2l_View_SetRect_dp);
    lua_register(l, "c2l_Functions_ShowImageAll_Scale", c2l_Functions_ShowImageAll_Scale);
    lua_register(l, "c2l_View_SetBackgroundColor", c2l_View_SetBackgroundColor);
    lua_register(l, "c2l_View_BringToFront", c2l_View_BringToFront);
    lua_register(l, "c2l_UI_Div_SetTxtColor", c2l_UI_Div_SetTxtColor);
    lua_register(l, "c2l_UI_Div_SetImage", c2l_UI_Div_SetImage);
    lua_register(l, "c2l_UI_Div_SetTxt", c2l_UI_Div_SetTxt);
    lua_register(l, "c2l_View_Edit_GetText", c2l_View_Edit_GetText);//取 edit 内容
    lua_register(l, "c2l_UI_LG", c2l_UI_LG); //语言函数
    lua_register(l, "c2l_UI_SetLanguage", c2l_UI_SetLanguage); //设置一个 uijson 页面的语言包
    lua_register(l, "c2l_View_Edit_SetText", c2l_View_Edit_SetText);
    lua_register(l, "c2l_View_Edit_SetPass", c2l_View_Edit_SetPass);
    lua_register(l, "c2l_View_Hide", c2l_View_Hide);
    lua_register(l, "c2l_View_Show", c2l_View_Show);
    lua_register(l, "c2l_GetParentView", c2l_GetParentView);
    lua_register(l, "c2l_Blle_GetDeviceName", c2l_Blle_GetDeviceName);
    lua_register(l, "c2l_Blle_GetDeviceRSSI", c2l_Blle_GetDeviceRSSI);
    lua_register(l, "c2l_CreateView", c2l_CreateView);
    lua_register(l, "c2l_UI_CreateViewClass", c2l_UI_CreateViewClass);  //2022
    lua_register(l, "c2l_UI_Div_SetTxt_forObj", c2l_UI_Div_SetTxt_forObj);
    lua_register(l, "c2l_UI_Div_SetTxtColor_forObj", c2l_UI_Div_SetTxtColor_forObj);
    lua_register(l, "c2l_UI_Div_SetRadius_forObj", c2l_UI_Div_SetRadius_forObj);
    lua_register(l, "c2l_ScrollView_ShowAllChild", c2l_ScrollView_ShowAllChild);
    lua_register(l, "c2l_UI_Div_SetTxtAlignLeft_forObj", c2l_UI_Div_SetTxtAlignLeft_forObj);
    lua_register(l, "c2l_UI_Div_SetImage_forObj", c2l_UI_Div_SetImage_forObj);
    lua_register(l, "c2l_View_Anima_Move", c2l_View_Anima_Move);
    lua_register(l, "c2l_GetOSLanguage", c2l_GetOSLanguage);
    lua_register(l, "c2l_UI_Div_CloseKeyboard", c2l_UI_Div_CloseKeyboard);
    
    
    

    lua_register(l, "c2l_Blle_ConnectDevice", c2l_Blle_ConnectDevice);
    lua_register(l, "c2l_Blle_CloseConnect", c2l_Blle_CloseConnect);
    lua_register(l, "c2l_Blle_GetServicesCount", c2l_Blle_GetServicesCount);
    lua_register(l, "c2l_Blle_GetServiceCharCount", c2l_Blle_GetServiceCharCount);
    lua_register(l, "c2l_Blle_GetCharUUID", c2l_Blle_GetCharUUID);
    lua_register(l, "c2l_Blle_SendBuf", c2l_Blle_SendBuf);
    lua_register(l, "c2l_Blle_CharIsNotify", c2l_Blle_CharIsNotify);
    lua_register(l, "c2l_Blle_StartNotifyChar", c2l_Blle_StartNotifyChar);

    lua_register(l, "c2l_UI_HttpPost", c2l_UI_HttpPost);  //2022
    lua_register(l, "c2l_UI_HttpGetToFile", c2l_UI_HttpGetToFile);  //2022
    lua_register(l, "c2l_UI_HttpPost_SetParam", c2l_UI_HttpPost_SetParam);  //2022
    lua_register(l, "c2l_UI_HttpPost_ClearParam", c2l_UI_HttpPost_ClearParam);  //2022





    lua_register(l, "c2l_View_RotateAni", c2l_View_RotateAni);
    lua_register(l, "c2l_GetSaveValue", c2l_GetSaveValue);
    lua_register(l, "c2l_RunJson", c2l_RunJson);

    lua_register(l, "c2l_SetSaveValue", c2l_SetSaveValue);
    
    
    
    
    lua_register(l, "c2l_UI_SetOnClick", c2l_UI_SetOnClick);
    
    //------------------------------------------------
    //下面是 app 相关的，按道理以后要淘汰
    
    
}//







