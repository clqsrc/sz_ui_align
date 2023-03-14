//对应 ui_json.lua 所以不应该包含任何的 js/jquery 专有代码
/// <reference path="ui_json_interface.ts" />
/// <reference path="std_div.ts" />
/// <reference path="std_edit.ts" />
//js 版本的 json 生成 form
// function CreateFromJson(parent, fn) {
//     json_src = gJson[fn];
//     //alert("CreateFromJson():" + json_src);
//     //生成一个 dom 节点
//     dom_form = JsonToUIControl(parent,  json_src);
//     return dom_form;
// }//
var ui_json = /** @class */ (function () {
    function ui_json() {
    }
    //根据字符串 id 得到 c 语言的原始句柄
    ui_json.prototype.GetView = function (id) {
        //return UI_GetView(self.uijson_handle, "pnlSetting");
        return UI_GetView(this.uijson_handle, id);
    }; //func
    //内部实现
    ui_json.prototype._GetView = function (id) {
        return this.nodes[id];
    }; //func
    //lua 语法的设置点击事件
    ui_json.prototype.SetOnClick = function (view_handle, obj, func) {
        // $(view_handle).on("click", function(){ 
        //     //alert("段落被点击了。"); 
        //     //动态创建一个 div 到指定控件下
        //     //var p = ui_last_click_div;
        //     //var o = CreateStdDiv(p);
        //     //p.Width("400px");
        //     //o.innerHTML = "new";
        //     //p.SetCaption("new");
        //     func(obj);
        // }); 
        // return;
        view_handle.isSetEventFunc = true;
        $(view_handle).click(function () {
            //alert("段落被点击了。"); 
            //动态创建一个 div 到指定控件下
            //var p = ui_last_click_div;
            //var o = CreateStdDiv(p);
            //p.Width("400px");
            //o.innerHTML = "new";
            //p.SetCaption("new");
            func(obj);
        });
    }; //
    ui_json.prototype.AlignControl = function () {
        UI_AlignControl(this);
    }; //
    return ui_json;
}()); //class
//为了给纯 js 的内部代码调用的
function Create_ui_json() {
    var uijson = new ui_json();
    uijson.uijson_handle = uijson; //对于 js 来说没有低层实现，所以就是它自己
    uijson.nodes = new Object(); // CreateHashMap();
    uijson.view2json = new Object(); // CreateHashMap();
    return uijson;
} //
//方便纯 js 的代码提示而已
function AsUiJson(obj) {
    var _obj = obj; //unknown
    return _obj;
} //
//2022.9 从 ui_json_2_form.js 移动过来的，在 java 版本中本来就在这个文件里
//与 app 同接口，从文件名创建
function CreateFromJson(parent, fn) {
    var json_src = GetFileJsonSrc(fn); //gJson[fn];
    //let json_obj = string2json(json_src);  //字符串转换出的 json 对象
    //alert("CreateFromJson():" + json_src);
    var uijson = Create_ui_json();
    //生成一个 dom 节点
    //let dom_form = JsonToUIControl(parent, json_src, uijson);
    var dom_form = uijson_JsonToUIControl(parent, json_src, uijson);
    uijson.root_view = dom_form;
    //uijson.root_json = ;
    //return dom_form;
    return uijson;
} //
function uijson_JsonToUIControl(dom_parent, s, uijson) {
    try { //这个过程太复杂，所以 try 一下看问题所在
        var j = string2json(s);
        var p = dom_parent;
        uijson.root_json = j; //这个一定要有的
        return _JsonToUIControl(p, j, uijson);
    }
    catch (e) {
        print_error(e);
        throw e;
    } //try
} //
//2022 原始设计太过复杂，实际上总 root view 变化时调整一次子控件位置就可以
//--------
//对齐各个控件
//public void AlignControl(JSONObject json)
function uijson_AlignControl(uijson, json_obj) {
    //if (null == json) return;
    if (!json_obj)
        return;
    var j = json_obj;
    var _id = ToString(j["id"]);
    //JSONArray children = this.GetJsonChildren(json, "children"); //子节点
    var view = uijson.GetView(_id); //let view = this.GetView(_id);
    //this.SetPos_forAlignObject(view, json);
    //o.SetPos_forAlignObject();
    if (!view)
        return;
    //console.log(_id);
    //2022.09 如果被 innerHTML 修改过，这时候的 _caption 实际上已经不存在了,所以是会有异常的，可以简单的 try 掉
    // try {
    //     view.SetPos_forAlignObject();
    //     console.log(_id + " ok.");
    // } catch (error) {
    //     console.log(_id + " error.");
    // }
    //if (_id == "map_imgShowFile") console.log("map_imgShowFile");
    view.SetPos_forAlignObject();
    //console.log(_id + " ok.");
    //--------  递归
    //子节点再来一次
    if (j.children)
        for (var i = 0; i < j.children.length; i++) {
            var child = j.children[i];
            //_JsonToUIControl(o, child, uijson);
            //this.AlignControl(child_json);
            uijson_AlignControl(uijson, child);
        } //for
} //
function UI_AlignControl(uijson) {
    var json_obj = uijson.root_json;
    uijson_AlignControl(uijson, json_obj);
} //
//--------------------------------------------------------
//以下函数临时
//创建一个视图
function UI_CreateView(view) {
    var div = CreateStdDiv2(view);
    return div;
    //return c2l_CreateView(view);
} //
//创建一个视图 //2022 可以指定类名，如 "image"
function UI_CreateViewClass(view, ui_class) {
    var div = CreateStdDiv2(view);
    return div;
    //return c2l_UI_CreateViewClass(view, ui_class);
} //
function UI_Div_SetTxt_forObj(uijson, view, txt) {
    view.SetCaption(txt);
} //
//设备视图的背景色
function View_SetBackgroundColor(view, hex_color) {
    view.backgroundColor(hex_color);
} //
//文字的颜色
function UI_Div_SetTxtColor_forObj(uijson, view, text_color)
{

    $(view).css({"color":text_color});

}//

//似乎原始没有这个函数，得去看 ios 版本
function UI_Div_SetTxtFontSize_forObj(uijson, view, font_size)
{
    $(view).css("font-size", font_size);
}//

//圆角
function UI_Div_SetRadius_forObj(uijson, view, radius)
{
    $(view).css({"border-radius":radius});
    
}//