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

#include "lua_functions.h"

//#include "lua/libs_lua.c"

//在 c 语言中运行 lua 所需要的环境的相关函数都在这里
//lua_functions.c 是给 lua 调用的函数集合

//---------------------------------------------------------------
//安全调用 lua 脚本
//加一个 call_log_tag，提示是在哪里调用的
int safe_lua_pcall (lua_State *L, int nargs, int nresults, int errfunc, const char *  call_log_tag);

//安全调用 lua 函数,其实和调用脚本也差不多，唯一的区别就是调用脚本时要指定 LUA_MULTRET
//即返回值个数为 -1 (LUA_MULTRET)，原因暂时未知。似乎是返回值放在堆栈上的意思，以后再研究
void safe_lua_call (lua_State *L, int nargs, int nresults, const char *  call_log_tag);

//---------------------------------------------------------------

lua_str_param Functions_lua_Int64ToStr(cint64 v)
{
    lua_str_param r = {};
    sprintf(r.str, "%lld", v);  //s_parentView
    
    return r;
}//

cint64 Functions_lua_StrToInt64(const char * s)
{
    cint64 r = 0;
    
    
    r = atoll(s);  //ios 下是可以的
    
    return r;
}//


//------------------------------------------------

//创建一个 lua 环境 //可以创建多个，不过我们只用一个
lua_State * Functions_create_lua()
{
    lua_State *L;
    L = luaL_newstate();  //创建lua状态机
    luaL_openlibs(L);  //打开Lua状态机中所有Lua标准库

    lua_State *l = L;
    
    return l;
}//

//2023 如果运行一个脚本出错时应当不触发 ontime onsize ，以免无法看到错误信息
int error_at_Functions_lua_run = 0;

//装载并运行一个 lua 脚本 //全局函数都可以这样加载
//0 -- 失败，1 -- 成功
int Functions_lua_run(lua_State * L, const char * fn)
{
    mempool * mem = newmempool();
    //lstring * src = Functions_LoadStringFromFile_assets("2.lua", mem);
    lstring * src = Functions_LoadStringFromFile_assets(fn, mem);
    //--------
    
    if (0 == src->len) {
        
        lstring * s = NewString(fn, mem);
        s->AppendConst(s, " 加载失败. 空脚本或者脚本不存在. ");
        
        ShowMessage_c(s->str, 0);
        
        freemempool(mem);

        error_at_Functions_lua_run = 1;
        return 0;
        
    }

    //L = luaL_newstate();  //创建lua状态机
    //luaL_openlibs(L);  //打开Lua状态机中所有Lua标准库

    ////lua_State *l = L;

    //luaL_dofile(L, "add.lua"); //加载lua脚本 //这在手机下应该是无法完成的

    //luaL_loadbufferx 应该是可以将缓冲加载为一个模块

    //luaL_dostring = (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
    //int lr = luaL_dostring(L, src->str); //为 0 才是正确的 //这个异常时会退出，所以自己写一个
    
    //int lr = luaL_loadstring(L, src->str); //其实是 luaL_loadbuffer 的封装，但是没法知道代码当前名字
    //用参数 fn 当做脚本名称，以便出错时显示是哪个脚本出错
    int lr = luaL_loadbuffer(L, src->str, src->len, fn);

    //src = NewString("--json = c2l_self_require(\"ui_lib/json.lua\");\na=5", mem);

    //lr = luaL_loadbuffer(L, src->str, src->len, fn);

    //ShowMessage_c(src->str, 0);
    
    if (0 == lr) //为 0 才是正确的
    {
        //开始调用函数，有0个参数，0个返回值
        ////int status = lua_pcall(L, 0, LUA_MULTRET,0);
        //开始调用函数，有1个参数，0个返回值
        //int status = lua_pcall(L, 1, LUA_MULTRET, 0);
        
        int status = safe_lua_pcall(L, 0, LUA_MULTRET, 0, __FUNCTION__);
        
        lr = status;
    }//
    
    //lua_remove(L, 1);

    if (0 != lr) {

        //将错误代码和栈顶的字符串打印出来
        printf("error code:%d,msg:%s\r\n", lr,lua_tostring(L, -1));
        
        lstring * s = NewString(fn, mem);
        s->AppendConst(s, " 加载失败");
        s->AppendConst(s, lua_tostring(L, -1));
        
        //ShowMessage_c("ui_json.lu 加载失败", parentView);
        ShowMessage_c(s->str, 0);

        freemempool(mem);

        error_at_Functions_lua_run = 1;
        return 0;
        
    }

    //--------
    freemempool(mem);

    return 1;
}//


//执行一个 lua 函数 //显示一个 uijson 窗口
void Functions_lua_ShowCarMainForm(lua_State *L, chandle parentView)
{

    //函数名
    lua_getglobal(L,"ShowCarMainForm");
    
    //参数入栈
    //lua_pushnumber(L, x);
    //参数入栈
    //lua_pushnumber(L, y);
    
    cint64 int64_parentView = (cint64)parentView;
    char s_parentView[256] = {};
    sprintf(s_parentView, "%lld", int64_parentView); //ios 可以这样用，其他环境还是自己写一个好了
    
    lua_pushstring(L, s_parentView);
    
    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);
    
    //开始调用函数，有1个参数，0个返回值
    //lua_call(L, 1, 0);
    safe_lua_call(L, 1, 0, __FUNCTION__);
    
    //取出返回值
    //sum = (int)lua_tonumber(L, -1);
    //清除返回值的栈
    //lua_pop(L,1);
    
    //return sum;
}//


//执行一个 lua 函数 //显示一个 uijson 窗口
void Functions_lua_ShowForm(lua_State *L, const char * func_name, chandle parentView)
{

    //函数名
    //lua_getglobal(L,"ShowCarMainForm");
    lua_getglobal(L, func_name);

    //参数入栈
    //lua_pushnumber(L, x);
    //参数入栈
    //lua_pushnumber(L, y);
    
    cint64 int64_parentView = (cint64)parentView;
    char s_parentView[256] = {};
    sprintf(s_parentView, "%lld", int64_parentView); //ios 可以这样用，其他环境还是自己写一个好了
    
    lua_pushstring(L, s_parentView);
    
    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);
    
    //开始调用函数，有1个参数，0个返回值
    //lua_call(L, 1, 0);
    safe_lua_call(L, 1, 0, __FUNCTION__);
    
    //取出返回值
    //sum = (int)lua_tonumber(L, -1);
    //清除返回值的栈
    //lua_pop(L,1);
    
    //return sum;
}//

//执行一个 lua 函数 //带 6 个参数[无返回值]，应该很够用了,参数都是字符串
void Functions_lua_RunFunc(lua_State *L, int pcount, const char * func_name,
                           const char * p1,
                           const char * p2,
                           const char * p3,
                           const char * p4,
                           const char * p5,
                           const char * p6
                           )
{

    if (1 == error_at_Functions_lua_run) return;  //2023 如果脚本本身有错误，这个运行也无用

    //函数名
    //lua_getglobal(L,"ShowCarMainForm");
    lua_getglobal(L, func_name);

    //参数入栈
    //lua_pushnumber(L, x);
    //参数入栈
    //lua_pushnumber(L, y);

    if (pcount >= 1) lua_pushstring(L, p1);
    if (pcount >= 2) lua_pushstring(L, p2);
    if (pcount >= 3) lua_pushstring(L, p3);
    if (pcount >= 4) lua_pushstring(L, p4);
    if (pcount >= 5) lua_pushstring(L, p5);
    if (pcount >= 6) lua_pushstring(L, p6);

    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);
    
    //开始调用函数，有1个参数，0个返回值
    //lua_call(L, 1, 0);
    //safe_lua_call(L, 1, 0, __FUNCTION__);
    //开始调用函数，有 6 个参数，0个返回值
    //safe_lua_call(L, 6, 0, __FUNCTION__);
    char tag[256] = {};
    sprintf(tag, "[%s()] func_name:[%s]", __FUNCTION__, func_name);
    safe_lua_call(L, pcount, 0, tag);

    //取出返回值
    //sum = (int)lua_tonumber(L, -1);
    //清除返回值的栈
    //lua_pop(L,1);
    
    //return sum;
}//

//2023 有字符串返回值的版本
//执行一个 lua 函数 //带 6 个参数[有返回值]，应该很够用了,参数都是字符串
lstring * Functions_lua_RunFunc_andResult(lua_State *L, int pcount, const char * func_name,
                           const char * p1,
                           const char * p2,
                           const char * p3,
                           const char * p4,
                           const char * p5,
                           const char * p6,
                           mempool * mem
)
{

    if (1 == error_at_Functions_lua_run) return NewString("", mem);  //2023 如果脚本本身有错误，这个运行也无用

    //函数名
    //lua_getglobal(L,"ShowCarMainForm");
    lua_getglobal(L, func_name);

    //参数入栈
    //lua_pushnumber(L, x);
    //参数入栈
    //lua_pushnumber(L, y);

    if (pcount >= 1) lua_pushstring(L, p1);
    if (pcount >= 2) lua_pushstring(L, p2);
    if (pcount >= 3) lua_pushstring(L, p3);
    if (pcount >= 4) lua_pushstring(L, p4);
    if (pcount >= 5) lua_pushstring(L, p5);
    if (pcount >= 6) lua_pushstring(L, p6);

    //开始调用函数，有2个参数，1个返回值
    //lua_call(L, 2, 1);

    //开始调用函数，有1个参数，0个返回值
    //lua_call(L, 1, 0);
    //safe_lua_call(L, 1, 0, __FUNCTION__);
    //开始调用函数，有 6 个参数，0个返回值
    //safe_lua_call(L, 6, 0, __FUNCTION__);
    char tag[256] = {};
    sprintf(tag, "[%s()] func_name:[%s]", __FUNCTION__, func_name);
    //safe_lua_call(L, pcount, 0, tag); //调用无返回值的 lua 函数
    safe_lua_call(L, pcount, 1, tag);  //有返回值的 //调用无返回值的 lua 函数 //第 2 个参数不能搞错，否则后面的 lua_pop 会异常
    //safe_lua_call(L, pcount, 3, tag);  //有返回值的 //调用无返回值的 lua 函数 //第 2 个参数不能搞错，否则后面的 lua_pop 会异常
    //safe_lua_call(L, pcount, 2, tag);  //有返回值的 //调用无返回值的 lua 函数 //第 2 个参数不能搞错，否则后面的 lua_pop 会异常

    int result_count = lua_gettop( L );  //本意是想取返回值的个数，但它实际上等于 safe_lua_call 的第二个参数
    printf( "调用函数后栈大小:%d\n", lua_gettop( L ) );

    //lua_pcall会调用函数并自动清除上面和函数有关的栈

    //取出返回值
    //int sum = (int)lua_tonumber(L, -1);
    //清除返回值的栈
    //lua_pop(L,1);

    //取出返回值
    //const char * s = lua_tostring(L, -1);
    //lstring * r = NewString(s, mem);  //有一个问题，怎么知道 s 的具体长度呢 //要用 lua_tolstring

    size_t len = 0;
    const char * s = lua_tolstring(L, -1, &len); //-1 就是栈顶值，也是 ok 的
    //const char * s = lua_tolstring(L, 1, &len); //ok //指定取第几个返回值，因为 lua 是允许多返回值的，不过//成功调用后会将返回值从右到左入栈
    //lstring * r = NewString(s, mem);  //有一个问题，怎么知道 s 的具体长度呢 //要用 lua_tolstring

    lstring * r = NewString("", mem);
    r->AppendCString(r, s, len);

    //清除返回值的栈
    ////lua_pop(L,1); //奇怪，这个会崩溃
    //lua_pop(L,1); //奇怪，这个会崩溃 //safe_lua_call 中指明有参数返回就不会

    //手动清空所有栈 //比指定个数安全很多，就算参数指定不正确也不至于崩溃
    lua_pop( L, -1);


    //return sum;
    return r;
}//



//一个测试
void Function_load_lua_test(lua_State *L,char *filename)
{
    mempool * mem = newmempool();
    lstring * src = Functions_LoadStringFromFile_assets("2.lua", mem);
    //--------

    L = luaL_newstate();  //创建lua状态机
    luaL_openlibs(L);  //打开Lua状态机中所有Lua标准库

    lua_State *l = L;

    //luaL_dofile(L, "add.lua"); //加载lua脚本 //这在手机下应该是无法完成的

    //luaL_loadbufferx 应该是可以将缓冲加载为一个模块

    //(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
    int lr = luaL_dostring(L, src->str); //为 0 才是正确的

    if (0 != lr) { freemempool(mem); return; }

    //--
    src = Functions_LoadStringFromFile_assets("1.lua", mem);  //如果用到了其他的 lua 就要先加载，否则也是会失败的
    lr = luaL_dostring(L, src->str); //为 0 才是正确的
    if (0 != lr) { freemempool(mem); return; }

    //if (luaL_loadstring(L, "a=1"))//src->str))
    //if(luaL_dostring(L, "print('1')")) // 执行Lua命令。
    //if (luaL_loadstring(l, src->str))
//    {
        //lua_pcall(l,0,0,0);
//
//        int r = luaadd(l, 9 ,10);  //实际上是需要内存保护的

//        printf("r : %d", r);
//    }else{
//        luaL_error(L,"load file error! %s",lua_tostring(L,-1));
//    }//



//    if(luaL_loadfile(*L,filename) || lua_pcall(*L,0,0,0))
//    {
//
//        luaL_error(*L,"load file error! %s",lua_tostring(*L,-1));
//
//    }//if

    printf("aaa\r\n");

    //--------
    freemempool(mem);

}//

//----------------------------------------------------------------

//安全调用 lua 脚本
//luaL_loadstring = (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
//lua_pcall与lua_call之间的区别
//
//定义：
//
//void lua_call (lua_State *L, int nargs, int nresults);
//int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
//
//这两个api的前三个参数含义一样，只是lua_pcall在保护模式（protection mode）下调用函数。
//执行脚本时目前 lua 的注释说明要求返回值个数一定要是 LUA_MULTRET，即 -1
//int safe_lua_pcall (lua_State *L, int nargs, int nresults, int errfunc)
//加一个 call_log_tag，提示是在哪里调用的
int safe_lua_pcall (lua_State *L, int nargs, int nresults, int errfunc, const char *  call_log_tag)
{
    int r = 0;  //似乎 lua 里面返回 0 表示成功
    
    mempool * mem = newmempool();
    
    //开始调用函数，有0个参数，0个返回值
    //int status = lua_pcall(L, 0, LUA_MULTRET,0);
    //开始调用函数，有1个参数，0个返回值
    //int status = lua_pcall(L, 1, LUA_MULTRET, 0);
    
    int status = lua_pcall(L, nargs, nresults, errfunc);
    
    if (status != LUA_OK)
    {
        //https://www.cnblogs.com/itangfeng/p/12244543.html 说要清空，似乎实测不用
        //将ExceptionHandle从栈中删掉，注意这里不用lua_pop,而是lua_remove是因为此时ExceptionHandle不一定在栈顶*
        //lua_remove(L, 1);
        
        //将错误代码和栈顶的字符串打印出来
        printf("error code:%d,msg:%s\r\n",status,lua_tostring(L, -1));
        
        //lstring * s = NewString("Lua run error [Functions_lua_ui_json_onSize]:", mem);
        lstring * s = NewString("Lua run error ", mem);
        s->AppendConst(s, "[");
        s->AppendConst(s, call_log_tag);
        s->AppendConst(s, "]");
        s->AppendConst(s, lua_tostring(L, -1));
        
        //lua_remove(L, 1);
        
        //ShowMessage_c(lua_tostring(l, -1), 0);
        ShowMessage_c(s->str, 0);

    }//if
    
    r = status;
    
    freemempool(mem);
    
    return r;
}//



//安全调用 lua 函数,其实和调用脚本也差不多，唯一的区别就是调用脚本时要指定 LUA_MULTRET
//即返回值个数为 -1 (LUA_MULTRET)，原因暂时未知。似乎是返回值放在堆栈上的意思，以后再研究

//void safe_lua_call (lua_State *L, int nargs, int nresults)
void safe_lua_call (lua_State *L, int nargs, int nresults, const char *  call_log_tag)
{
    //其实就是错误处理函数没有而已
    safe_lua_pcall(L, nargs, nresults, 0, call_log_tag);
}//





