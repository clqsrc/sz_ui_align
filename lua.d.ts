//lua.d.ts 为 lua 版 ui_align 的 typescript 代码提示接口文件

// 值 函数
declare function AddLog(message: string): void;

declare function self_require(fn:string):any;  //2022.10
//设备视图的背景色
declare function View_SetBackgroundColor(view:any, hex_color:string):void;  //2022.10
declare function HexColor(s:string):any;
declare function View_SetRect_dp(view:any, rect:any):void;
declare function View_GetRect(view:any):any;
declare function UI_GetRootView(uijson_handle:any):any;
declare function View_SetRect(view:any, rect:any):void;
declare function UI_AlignControl(uijson_handle:any):void;

declare function tostring(obj:any):string; //2022.11


//--------------------------------------------------------
//2022.11
//2022 c2l_UI_RunJson 运行一个 json 参数的函数调用  //lua 支持两个返回值，但别的语言未必支持
//function UI_RunJson(json_or_bin_string_or_buf, buf_len)
//typescript 也可以支持两个返回值 //之所以这个函数定义为两个返回值是因为 lua 是无类型的，返回一个类的话会没有代码提示
//declare function UI_RunJson(json_or_bin_string_or_buf:any, buf_len:number):[any, number];
declare function UI_RunJson_lua(json_or_bin_string_or_buf:any, buf_len:number):[any, number];

//调用的方法如下
// let [r,l] = UI_RunJson('{ "func_name":"file_dir", "param1":"d:\\\\" }', 0);
// ShowMessage(r);
// ShowMessage(l);
// [r,l] = UI_RunJson('{ "func_name":"shell", "param1":"dir" }', 0);
//但这个转换会成 npx typescript-to-lua 转换错误。所以还是不要用这个函数

declare function UI_RunJson(json_or_bin_string_or_buf:any, buf_len:number):UI_RunJson_Result;
declare class UI_RunJson_Result
{

    //r:string;
    r:any;      //其实是二进制数据或者字符串
    l:number;   //二进制数据的长度 size ，不一定是字符串的长度，目前还未传递过二进制数据，所以以后可能会修改

}//

//--------------------------------------------------------


//jquery 的声明文件
//https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/jquery/index.d.ts

//其实定义这个就能编译通过，只是没有代码提示而已 //不能上传那么多文件的在线编译器就可以用这种声明
//declare function $(a: any): any;

/*/// <reference path = "jquery.ts/jquery/index.d.ts" />*/

declare function CreateHashMap(): any;
declare function import_ok(a:any): any;
declare function AssignDivAlignFunction(a:any): any;
declare function CreateHashMap(): any;
declare function function_GetGUID(): any;
//取静态 html 的参数
declare function function_GetUrlParm(parm_name):any;
//子字符串是否存在//不区分大小写
declare function FindStr(s:string, substr:string):boolean;

declare function ShowMessage(s:any): any;


declare function AsStdDiv(obj:any):any;

//lua View 的 ts 接口
//declare class View 
declare class View_  //因为 typescript-to-lua 转换后的 ":" 调用问题，因此废弃这个，使用下面的 namespace View
//declare namespace View 
{

    //name: string;
    public static Hide(view:any):void;
    public static Show(view:any):void;
    //设备视图的背景色
    //public static SetBackgroundColor(view:any, hex_color:string):void;  //2022.10
    //typescript-to-lua 对 static 的调用似乎是错误的，这时候它仍然使用 ":" 运算符而不是 ":" ，这导致会传递一个不存在的 this 参数过去
    //所以为了兼容这种情况，只好使用 namespace 中的函数来代替 class 中的类函数
    static SetBackgroundColor(view:any, hex_color:string):void;  //2022.10

    //public SetBackgroundColor;any;

}//



interface Console2 {
    log(...data: any[]): void;
    
}

declare var console2: Console2;



//ui_json 的 ts 接口，可以直接抄 ui_json.lua
declare class ui_json {

    public uijson_handle:any;

    //----

    public GetView(id:any):any;

    public GetDiv(id:any):any;

    public SetOnClick(view_handle:any, obj:any, func:any):any;

    public CreateView(parentView:any):any;   //2022.10
    //设置圆角
    public SetRadius(view:any, radius:any):void;

    public AlignControl():void;

    root_view:any; //TStdDiv;

}//

declare class crect{
    //成员变量不给初始值的话，tsc 生成 js 的时候就会忽略
    left:number;// = 0;    //其实现在的系统一般是支持 float 的宽高的，以后再考虑这种兼容性吧
    top:number;// = 0;     //ts 用 number 来表示 int 和 float
    width:number;// = 0;
    height:number;// = 0;

}//


declare function View_Hide(div:any):any;
declare function View_Show(div:any):any;
declare function View_Edit_GetText(div:any):any;
declare function View_Edit_SetText(div:any, txt:any):void;

declare function UI_Div_SetImage_forObj(uijson:any, map_pnlMap_imgBak:any, url:any):void;
declare function UI_Div_SetTxt_forObj(uijson:ui_json, view:TStdDiv, txt:string):void;

declare function CreateFromJson(parent:any, fn:any):ui_json;

//对应 view.lua

///// <reference path="ui_json_interface.ts" />

// declare namespace View {

//     function Hide(view:any):void;
//     function Show(view:any):void;

//     //设备视图的背景色
//     function SetBackgroundColor(view:any, hex_color:string):void;  //2022.10
    
// }//namespace



declare class View {


    //c 函数别名而已，为方便函数的功能分类
    static GetRect(view:any):crect;

    static SetRect(view:any, rect:any);
    

    static Show(view:any);

    static Hide(view:any):void;

    //设备视图的背景色
    static SetBackgroundColor(view:any, hex_color:string):void;  //2022.10

}//class

//--------------------------------------------------------
//root_view_form 暂时作为脚本的根 view ，它不一定是 view ，可能是原生 ui 控件
declare let root_view_form:any; // = null;

//--------------------------------------------------------


//--------------------------------------------------------
//原有的 js 代码要写 TypeScript 声明文件 
//https://www.runoob.com/typescript/ts-ambient.html
//https://www.jianshu.com/p/150f2893d643?

/* 

// 类型 接口
interface Person {name: string;}

// 类型 类型别名
type Fruit = {size: number};

// 值 变量
declare let a: number;

// 值 函数
declare function log(message: string): void;

// 值 类
declare class Person {name: string;}

// 值 枚举
declare enum Color {Red, Green}

// 值 命名空间
declare namespace person {let name: string;}

*/


//--------------------------------------------------------


