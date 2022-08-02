
//ui_json.ts 还是对应原来的 ui_json.lua 吧。其他不好对应的内容放这里来。

/// <reference path = "js.d.ts" />
/// <reference path = "jquery.ts/jquery/index.d.ts" />
/// <reference path = "std_div.ts" />

class crect{
    //成员变量不给初始值的话，tsc 生成 js 的时候就会忽略
    left:number = 0;    //其实现在的系统一般是支持 float 的宽高的，以后再考虑这种兼容性吧
    top:number = 0;     //ts 用 number 来表示 int 和 float
    width:number = 0;
    height:number = 0;

};



function UI_GetView(uijson_handle:any, id:string):any {

    //return c2l_UI_GetView(uijson_handle, id);
    //return "0";

    return (uijson_handle as ui_json)._GetView(id);
}//


//function View_GetRect(view:HTMLElement):crect {
function View_GetRect(view:any):crect {
    //return View_GetRect_dp(view);

    //let view = (_view as TStdDiv);

    let rect:crect = new crect();

    rect.left = 0;
    rect.top = 0;

    var x = view.Left(); 
    var y = view.Top(); 
    var w = view.Width();
    var h = view.Height();

    rect.left = x;
    rect.top = y;
    rect.width = w;
    rect.height = h;

    return rect;
}//

// function ui_json:SetOnClick(view_handle, obj, func)

//     local event = UiEvent:new();
//     event.obj = obj;
//     event.func = func;

//     local r = UI_SetOnClick(view_handle, SaveFunc(event));

//     --return r;
// end

//注意，这里传的不再是 self ，而是 item
// OnClick_deviceItem = function(pitem)
//     --//什么都不做，只是为了屏蔽 OnClick_pnlMain

//     local item = newItem();  --//只是为了代码提示
//     item = pitem;
// end;

//取 edit 内容
function View_Edit_GetText(edit:any):any {

    let txt = $(edit).val();
    return txt;

}//

//注意，这个是直接操作 view 里面的 edit 内部控件，按道理不应该开放，不过目前暂时可用
function View_Edit_SetText(edit:any, txt:any) {
    
    $(edit).val(txt);

}//
