/// <reference path="js.d.ts" />
/// <reference path="main_textfile_app_plugin_api.d.ts" />

ShowMessage("hi");


console.log(js_request);

console.log(JSON.stringify(js_request));

var app_id = "find_file.newbt.net";  //实际上是常量，每个插件不能相同。也不能在程序中改变。

//注意不能两个函数间是不能共享变量的，因为它们并不是同一个时间内运行的，实际上分属两个不同的运行时。
//似乎不太好，比如事件响应这些就做不了了，还是一个插件面板对应一个运行环境吧，删除面板时就删除了运行环境

//创建自己的界面时
function plugin_on_create(parent_panel)
{
    console.log("plugin_on_create() 1");

    var panel = panel_new(parent_panel);
    panel_set_parent(panel, parent_panel);
    panel_set_bounds(panel, 300, 10, 50, 20);
    panel_set_bounds(panel, 10, 350, 50, 20);
    panel_show(panel);

    var edit = edit_new(panel);
    edit_set_parent(edit, panel);
    edit_set_bounds(edit, 10, 5, 50, 20);
    edit_show(edit);

    var edit = edit_new(parent_panel);
    edit_set_parent(edit, parent_panel);
    edit_set_bounds(edit, 10, 5, 50, 20);
    edit_show(edit);

    //js_request.ui = { form:"aaa", edit:"edit" }; //这样可以
    //js_request.ui = { form:"aaa", edit:edit };  //这样就不行

    js_request.ui = { form:"aaa" }; //这样可以

    ////set_js_request_field(js_request, "edit", edit);  //相当于 js_request.edit = edit;

    ////js_request.ui.edit = edit; //不能这样写，因为 js_request 这里是 golang 中的对象
    //js_request.edit = edit;


    ////edit_hide(js_request.ui.edit);  //不过在脚本里面这里还是正确的

    ////save_var(js_request);

    //set_var("edit", edit);  //全局变量
    set_var(app_id + "edit", edit);  //全局变量


    console.log("plugin_on_create() 2");

}//

//将查找到的内容加入到程序搜索结果框中
//function API_AddToFindResult(pre_string string, fn string, line_no int, find string, end_string string){
// function API_AddToFindResult(pre_string, fn, line_no, find, end_string);

//处理一个文件时
function plugin_on_file(fn)
{
    console.log("plugin_on_file() 1");
    //console.log(JSON.stringify(js_request));

    API_AddToFindResult("查找文件：", fn, 0, "要查找的内容", "end");

    // var edit = js_request.ui.edit;
    // var s = edit_get_text(edit);
    //var edit = js_request.edit;
    ////var edit = get_js_request_field(js_request, "edit");

    var edit = get_var(app_id + "edit");  //全局变量

    var s = edit_get_text(edit);

    console.log("plugin_on_file() s:", s);

    vcl.ShowMessage(s);

    console.log("plugin_on_file() 2");
}//

//处理一行时
function plugin_on_line(line)
{
    return line;
}//

