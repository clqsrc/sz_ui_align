
///// <reference path="ui_json_interface.d.ts" />
///// <reference path="std_div.d.ts" />
///// <reference path="std_edit.ts" />


//js 版本才有的虚拟 div 层。只是 ui_json 的方便性函数集而已，不一定能用在 lua/c++ 版本中。

//类似 MakeStdDiv

//--------------------------------------------------------

//2023 用 npx 前缀的话是要更新的，所以最好是直接用包的命令行工具 tstl 。因为一来有兼容性问题，二来有些环境下会卡，因为要先访问网络
//tstl ui_json_vdiv.ts --allowJs --noImplicitSelf true
//tsc  ui_json_vdiv.ts --allowJs

//--------------------------------------------------------


//interface TStdDiv extends HTMLDivElement


declare function MakeVDiv(view:any, uijson:ui_json):VDIV;

//--------------------------------------------------------

declare interface VDIV {

    uijson: ui_json,  //传入的
    view:any,       //传入的参数

    class:"div",     //默认是 div ，目前可以是 edit, image

    //当做 edit 类
    AsEdit():VDIV,

    //当做图片类
    AsImage():VDIV,

    Show():void; 

    Hide():void;  

    //lua 语法的设置点击事件
    //SetOnClick(view_handle:any, obj:any, func:any):void;
    SetOnClick(obj:any, func:any):void;

    //delphi 风格
    SetCaption(txt:string):void;
    
    //通用的
    SetText(txt:string):void;  

    //针对 edit 的，如果用错了在 c 语言版本里会崩溃。js 版本为了方便直接忽略好了
    SetEditText(txt:string):void;  

    GetEditText():string;

    //设置大小位置
    SetRect(rect:crect):void; 

    GetRect():crect;

    //针对 image 的，div 不能用 //c 语言版本应该会崩溃，如果用错的话
    //设置要显示的图片文件名
    //这个函数还未完成，实际上还不能区分是否是全路径
    SetImage(fn:string):void;  

    //背景色
    SetBackgroundColor(hex_color:string):void;  

    //设置圆角
    SetRadius(radius:any):void;  

}//


