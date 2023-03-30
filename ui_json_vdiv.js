/// <reference path="ui_json_interface.ts" />
///// <reference path="std_div.ts" />
///// <reference path="std_edit.ts" />
/// <reference path="ui_json.ts" />
//js 版本才有的虚拟 div 层。只是 ui_json 的方便性函数集而已，不一定能用在 lua/c++ 版本中。
//类似 MakeStdDiv
//--------------------------------------------------------
//2023 用 npx 前缀的话是要更新的，所以最好是直接用包的命令行工具 tstl 。因为一来有兼容性问题，二来有些环境下会卡，因为要先访问网络
//tstl ui_json_vdiv.ts --allowJs --noImplicitSelf true
//tsc  ui_json_vdiv.ts --allowJs
//--------------------------------------------------------
//interface TStdDiv extends HTMLDivElement
//参考 http://www.ruanyifeng.com/blog/2012/07/three_ways_to_define_a_javascript_class.html
//主要是为了原始 js 版本的代码提示兼容性，也方便转译为 c 结构体和 lua
//这是个虚拟类，确定不会再修改的才放到这里面来，不通用的，比如动画，还是用原有的 c 风格函数为好
function MakeVDiv(view, uijson) {
    //--------------------------------------------------------
    var t = {
        uijson: uijson,
        view: view,
        "class": "div",
        AsEdit: function () {
            this_["class"] = "edit";
            return this_;
        },
        AsImage: function () {
            this_["class"] = "image";
            return this_;
        },
        Show: function () {
            View_Show(view);
        },
        Hide: function () {
            View_Hide(view);
        },
        //lua 语法的设置点击事件
        // SetOnClick : function(view_handle, obj, func):void{
        //     uijson.SetOnClick(view_handle, obj, func);
        // },
        //typescript 不能重载? //应该是不认为是类，是所谓“对象文本”//也就是说在这里它本质上是 kv 所以只能有一个，即 t["SetOnClick"] = func...
        SetOnClick: function (obj, func) {
            var view_handle = view;
            uijson.SetOnClick(view_handle, obj, func);
        },
        //delphi 风格
        SetCaption: function (txt) {
            UI_Div_SetTxt_forObj(uijson.uijson_handle, view, txt);
        },
        //通用的
        SetText: function (txt) {
            if ("edit" == this_["class"]) {
                //编辑器的文本是比较特殊的
                View_Edit_SetText(view, txt);
            }
            else {
                //默认当做 div
                UI_Div_SetTxt_forObj(uijson.uijson_handle, view, txt);
            }
        },
        //针对 edit 的，如果用错了在 c 语言版本里会崩溃。js 版本为了方便直接忽略好了
        SetEditText: function (txt) {
            //编辑器的文本是比较特殊的
            View_Edit_SetText(view, txt);
        },
        GetEditText: function () {
            //编辑器的文本是比较特殊的
            return View_Edit_GetText(view);
        },
        //设置大小位置
        SetRect: function (rect) {
            View_SetRect(view, rect);
        },
        GetRect: function () {
            return View_GetRect(view);
        },
        //针对 image 的，div 不能用 //c 语言版本应该会崩溃，如果用错的话
        //设置要显示的图片文件名
        //这个函数还未完成，实际上还不能区分是否是全路径
        SetImage: function (fn) {
            UI_Div_SetImage_forObj(uijson.uijson_handle, view, fn);
        },
        //背景色
        SetBackgroundColor: function (hex_color) {
            //编辑器的文本是比较特殊的
            View_SetBackgroundColor(view, hex_color);
        },
        //设置圆角
        SetRadius: function (radius) {
            UI_Div_SetRadius_forObj(uijson.uijson_handle, view, radius);
            //CreateFromJson()
        }
    }; //
    var this_ = t; //如果是在类定义外面，想用 this 是不行的，会指向函数本身，这时候就要用这个虚拟的 this_
    //--------------------------------------------------------
    //----
    return t;
} //
//--------------------------------------------------------
