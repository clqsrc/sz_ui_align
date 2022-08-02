
//ui 设计器的属性窗口中选择类名

/// <reference path="js.d.ts" />
/// <reference path="ui_json_interface.ts" />
/// <reference path="ui_json.ts" />
/// <reference path="ui_json_view.ts" />
/// <reference path="std_div.ts" />
/// <reference path="std_edit.ts" />



declare function ui_design_prop_create(): any;
declare function CreateFromJson(parent:any, fn:any):any;
declare function ui_design_src_create();
declare function ui_design_toolbar_create_v2(a:any);
declare function JsonToUIControl(a:any, b:any);
declare function GetSaveKey(a:any):any;

//--------------------------------------------------------


//--------------------------------------------------------

//function CreateMapFormsAndStart()
function ui_design_prop_class_create(parent_edit:any)
{
    //parent_edit 是弹出它的 edit 控件

    //--------------------------------------------------------
        
    let pnlDesign = document.getElementById("pnlDesign");
                
    let uijson = CreateFromJson(pnlDesign, "form_class.json");

    let form:Class_DesignClassSelectForm = new Class_DesignClassSelectForm();
    form.uijson = uijson;

    let parent = pnlDesign;

    parent_edit.parent_edit = parent_edit;

    form.onCreate(parent);

    //--------------------------------------------------------
    //取消设计的相关内容
    //ui_at_desgin = false;

    return;


}//



//--------------------------------------------------------
//本页面主类

class Class_DesignClassSelectForm
{
    parentView:any; //onCreate_parent = 0; --//记录创建时的父窗口
    uijson_handle:any;
    uijson:ui_json;

    //----
    parent_edit:TStdEdit; //是弹出它的 edit 控件



    //onCreate(self, parent):void  //成员函数中不用加 self 参数，生成 lua 时会自动加上的
    onCreate(parent):void  //成员函数中不用加 self 参数，生成 lua 时会自动加上的
    {
        //let map_txtFileName = AsStdDiv(document.getElementById("map_txtFileName"));

        //let btn = document.getElementById("map_btnShowFile");

        //this.uijson.SetOnClick(btn, this, this.OnClick_btnShowFile);
        this.uijson.SetOnClick(this.uijson.GetView("btnEdit" ), "edit" , this.OnClick_btnSelect);
        this.uijson.SetOnClick(this.uijson.GetView("btnMemo" ), "memo" , this.OnClick_btnSelect);
        this.uijson.SetOnClick(this.uijson.GetView("btnImage"), "image", this.OnClick_btnSelect);
        

    }//

    //注意，这里传的不再是 self ，而是 item
    //lua 中的 self 是很特殊的，不要用这个，在需要的时候 typescript-to-lua 会自己生成 self 的
    OnClick_btnSelect(class_name:string):void
    {
        //alert("aaa");
        //什么都不做，只是为了屏蔽 OnClick_pnlMain

        //local item = newItem();  --//只是为了代码提示
        //item = pitem;

        //let edit = this.uijson.GetView("map_txtFileName_edit");  //根据 js 事件的特点，这时候的 this 并不是类本身
        //所以还要传一个另外的 self 参数进来

        let edit = this.parent_edit;
        

        View_Edit_SetText(edit, class_name);

    }//

};//


//--------------------------------------------------------
//--------------------------------------------------------
//以下是 c 语言的兼容接口，能兼容 c 语言的就尽量兼容 c 语言的语法


//--------------------------------------------------------
//--------------------------------------------------------
//以下是 js 特有的代码，以后要替换的

// function MapMainForm_ShowMap(form:Class_MapMainForm, fn:string)
// {

// }//


import_ok("ui_design_prop_class.js");

