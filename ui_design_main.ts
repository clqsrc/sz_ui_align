
//原来设计视图的 index.html 的内容都放到这里来

/// <reference path="js.d.ts" />
/// <reference path="ui_json_interface.ts" />
/// <reference path="ui_json.ts" />
/// <reference path="ui_json_view.ts" />
/// <reference path="std_div.ts" />
/// <reference path="ui_design_prop_class.ts" />


declare function ui_design_prop_create(): any;
declare let ui_json_t1: any;
declare function CreateFromJson(parent:any, fn:any):any;
declare function CreateFromJson_Src(parent:any, fn:any):any;
declare function ui_design_src_create();
declare function ui_design_toolbar_create_v2(a:any);
declare function JsonToUIControl(a:any, b:any);
declare function GetSaveKey(a:any):any;




function CreateDesignFormsAndStart(){

    let pnlDesign = document.getElementById("pnlDesign");
                
    let form_toolbar = CreateFromJson(pnlDesign, "form_toolbar.json").root_view;

    //form_toolbar.SetPos("30px", "0px");

    ui_design_toolbar_create_v2(form_toolbar);

    ui_design_prop_create(); //显示设计界面//属性窗口
    ui_design_src_create();

    //--------------------------------------------------------
    //解码 json 并放到设计面板上
    
    var ui_design_root_form_parent = CreateStdDiv(pnlDesign) ; //还是给 ui_design_root_form 一个固定的父窗体，要不不好转换
    var o:any = ui_design_root_form_parent;
    o.SetId("ui_design_root_form_parent");
    o.Width("1920");
    o.Height("1080");

    // alert(View.GetRect(o).height);
    // //let t = CreateStdDiv2(o) as TStdDiv;
    // let t = CreateStdDiv2(o); //这样也是可以的
    // t.innerHTML = "aaaaaa"; 
    // t.backgroundColor("#ff0000");
    // //t.SetBackgroundColor("#ff0000");
    // return;


    o.SetPos("30px", "250px");
    o.backgroundColor("#222222");
    
    var save_src = GetSaveKey("src");
    
    //如果上次存有代码
    if (save_src != undefined && save_src != null && save_src.length>0)
    {
        //ui_design_root_form = JsonToUIControl(null,  save_src);
        //ui_design_root_form = JsonToUIControl(ui_design_root_form_parent, save_src);
        ui_design_root_form = CreateFromJson_Src(ui_design_root_form_parent, save_src).root_view;

        
    }
    else
    {
        //ui_design_root_form = JsonToUIControl(null, ui_json_t1); //没有就加载程序内嵌的
        //ui_design_root_form = JsonToUIControl(ui_design_root_form_parent, ui_json_t1); //没有就加载程序内嵌的
        ui_design_root_form = CreateFromJson_Src(ui_design_root_form_parent, ui_json_t1).root_view; //没有就加载程序内嵌的
    }


    ui_last_click_div = ui_design_root_form;  //最后点击控件也很重要


}//