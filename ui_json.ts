

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

class ui_json {

    uijson_handle:any;

    //保存 id 对应的 view
    //public HashMap<String, View> nodes = new HashMap<String, View>();
    nodes:any;

    //this.view2json.put(view, json);  //存某个对象对应的 json 节点
    view2json:any;

    root_view:TStdDiv;

    //根据字符串 id 得到 c 语言的原始句柄
    GetView(id:string) {
            
        //return UI_GetView(self.uijson_handle, "pnlSetting");
        return UI_GetView(this.uijson_handle, id);

    }//func

    //内部实现
    _GetView(id:string) {
            
        return this.nodes[id];

    }//func

    //lua 语法的设置点击事件
    SetOnClick(view_handle, obj, func):void
    {
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