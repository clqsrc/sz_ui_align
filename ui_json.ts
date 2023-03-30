

//对应 ui_json.lua 所以不应该包含任何的 js/jquery 专有代码

/// <reference path="ui_json_interface.ts" />
/// <reference path="std_div.ts" />
/// <reference path="std_edit.ts" />
/// <reference path="ui_json_vdiv.ts" />

declare function print_error(e:any); //print_error.js 中
declare function ToString(value);    //ui_design.js 中
declare function string2json(s);     //ui_design.js 中
declare function _JsonToUIControl(dom_parent, json_obj, uijson);  //ui_design.js 中
declare function GetFileJsonSrc(fn);    //ui_json_2_form.js 中

// declare function MakeVDiv(view:any, uijson:ui_json):VDIV;

//js 版本的 json 生成 form

// function CreateFromJson(parent, fn) {

//     json_src = gJson[fn];

//     //alert("CreateFromJson():" + json_src);

//     //生成一个 dom 节点
//     dom_form = JsonToUIControl(parent,  json_src);

//     return dom_form;
// }//

class ui_json {

    uijson_handle:any;

    //保存 id 对应的 view
    //public HashMap<String, View> nodes = new HashMap<String, View>();
    nodes:any;

    //this.view2json.put(view, json);  //存某个对象对应的 json 节点
    view2json:any;

    root_view:TStdDiv;
    root_json:any; //这个是 json 解码出来的 json_obj //2022.9

    //根据字符串 id 得到 c 语言的原始句柄
    GetView(id:string):TStdDiv {
            
        //return UI_GetView(self.uijson_handle, "pnlSetting");
        return UI_GetView(this.uijson_handle, id);

    }//func

    //js 版本的虚拟 div
    GetDiv(id:string):VDIV {

        let view = this.GetView(id);

        return MakeVDiv(view, this);

    }

    public CreateView(parentView:any):any   //2022.10
    {
        return UI_CreateView(parentView);
    }

    //设置圆角
    public SetRadius(view:any, radius:any):void
    {
        UI_Div_SetRadius_forObj(this.uijson_handle, view, radius);
    }

    //内部实现
    _GetView(id:string) {
            
        return this.nodes[id];

    }//func

    //lua 语法的设置点击事件
    SetOnClick(view_handle, obj, func):void
    {

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

        $(view_handle).click(function(){ 
            //alert("段落被点击了。"); 
            
            //动态创建一个 div 到指定控件下
            //var p = ui_last_click_div;
            //var o = CreateStdDiv(p);
            //p.Width("400px");
            //o.innerHTML = "new";
            //p.SetCaption("new");
            func(obj);
            
        }); 
    }//

    AlignControl():void
    {
        UI_AlignControl(this);
    }//

}//class


//为了给纯 js 的内部代码调用的
function Create_ui_json():ui_json
{
    let uijson = new ui_json();

    uijson.uijson_handle = uijson; //对于 js 来说没有低层实现，所以就是它自己

    uijson.nodes = new Object(); // CreateHashMap();
    uijson.view2json = new Object(); // CreateHashMap();

    return uijson;
}//

//方便纯 js 的代码提示而已
function AsUiJson(obj:any):ui_json
{
	let _obj:ui_json = (obj as ui_json); //unknown

	return _obj;
}//

//2022.9 从 ui_json_2_form.js 移动过来的，在 java 版本中本来就在这个文件里
//与 app 同接口，从文件名创建
function CreateFromJson(parent:any, fn:string):ui_json {

    let json_src = GetFileJsonSrc(fn); //gJson[fn];

    //let json_obj = string2json(json_src);  //字符串转换出的 json 对象

    //alert("CreateFromJson():" + json_src);

    let uijson = Create_ui_json();

    //生成一个 dom 节点
    //let dom_form = JsonToUIControl(parent, json_src, uijson);
    let dom_form = uijson_JsonToUIControl(parent, json_src, uijson);

    uijson.root_view = dom_form as TStdDiv;
    //uijson.root_json = ;

    //return dom_form;
    return uijson;
}//


function uijson_JsonToUIControl(dom_parent:any, s:string, uijson:ui_json) 
{
	try{//这个过程太复杂，所以 try 一下看问题所在
	
	var j = string2json(s);
	var p = dom_parent;

    uijson.root_json = j;  //这个一定要有的
	
	return _JsonToUIControl(p, j, uijson);
	
	}catch(e){
		print_error(e)
		throw e;
	}//try
		
}//


//2022 原始设计太过复杂，实际上总 root view 变化时调整一次子控件位置就可以
//--------
//对齐各个控件
//public void AlignControl(JSONObject json)
function uijson_AlignControl(uijson:ui_json, json_obj:any)
{
	//if (null == json) return;
	if (!json_obj) return;

	let j = json_obj;

	let _id = ToString(j["id"]);

	//JSONArray children = this.GetJsonChildren(json, "children"); //子节点

	let view = uijson.GetView(_id); //let view = this.GetView(_id);
	//this.SetPos_forAlignObject(view, json);
	//o.SetPos_forAlignObject();
    if (!view) return;

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
	if ( j.children)
	for (var i=0; i< j.children.length; i++) { 
		var child = j.children[i];
		
		//_JsonToUIControl(o, child, uijson);

		//this.AlignControl(child_json);
		uijson_AlignControl(uijson, child);
		
	}//for

}//

function UI_AlignControl(uijson:ui_json)
{
    let json_obj = uijson.root_json;
    
    uijson_AlignControl(uijson, json_obj);


}//



//--------------------------------------------------------
//以下函数临时

//创建一个视图
function UI_CreateView(view)
{
    let div = CreateStdDiv2(view);

    return div;
    //return c2l_CreateView(view);

}//


//创建一个视图 //2022 可以指定类名，如 "image"
function UI_CreateViewClass(view, ui_class)
{
    let div = CreateStdDiv2(view);

    return div;

    //return c2l_UI_CreateViewClass(view, ui_class);

}//


function UI_Div_SetTxt_forObj(uijson:ui_json, view:TStdDiv, txt:string)
{
    view.SetCaption(txt);
}//


//设备视图的背景色
function View_SetBackgroundColor(view:TStdDiv, hex_color:string)
{
    view.backgroundColor(hex_color);
}//

//文字的颜色
function UI_Div_SetTxtColor_forObj(uijson:any, view:any, text_color:any)
{

    $(view).css({"color":text_color});

}//

//ui_json_interface.ts 中已经有了
//设置图片
// function UI_Div_SetImage_forObj(uijson:any, view:any, fn:string)
//加一个全屏的
function UI_Div_SetImage_Full(uijson:any, view:any, fn:string)
{
    //var img = AsStdDiv(document.getElementById("map_imgShowFile"));
    //img.innerHTML = "<img src='map_image/loading.gif' style='width:100%' />";
    var img = AsStdDiv(view)
    img.innerHTML = "<img src='" + fn + "' style='width:100%' />";
}//

//似乎原始没有这个函数，得去看 ios 版本
function UI_Div_SetTxtFontSize_forObj(uijson:any, view:any, font_size:any)
{
    $(view).css("font-size", font_size);
}//

//圆角
function UI_Div_SetRadius_forObj(uijson:any, view:any, radius:any)
{
    $(view).css({"border-radius":radius});
    
}//

//--------------------------------------------------------
//root_view_form 暂时作为脚本的根 view ，它不一定是 view ，可能是原生 ui 控件
let root_view_form = null;

//--------------------------------------------------------