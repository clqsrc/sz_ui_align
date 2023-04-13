

//所有的 json 都保存在这个变量中
//奇怪，后面的 import_ok() 会导致这个后加载而导致重复定义
//全局变量放在这里其实是有问题的
//let gJson = {};
//gJson = {};  //全局变量实际上应该象这样不加前导声明

let gJson = document.gJson;

//防止重入本 js 导致 gJson 被清空
if (null == gJson || undefined == gJson)
{
    gJson = {};
    document.gJson = gJson;
}//

//let gJson4 = {};


//用函数其实要更安全，js 在调用函数时似乎会等待大家加载，而变量则不会
//设置一个文件的内容
function SetFileJsonSrc(fn, src)
{
    if (null == gJson)
    {
        gJson = {};
    }//

    gJson[fn] = src;

}//


//js 版本的 json 生成 form

//直接从源码创建 //目前只在 js 版本中才有
function CreateFromJson_Src(parent, src) {

    let fn = "__memory__";
    gJson[fn] = src;

    return CreateFromJson(parent, fn);

}//

function GetFileJsonSrc(fn)
{
    let json_src = gJson[fn];

    return json_src;

}//

//2022.9 移动到 ji_json.ts 文件中，在 java 版本中本来就在那个文件里
// //与 app 同接口，从文件名创建
// function CreateFromJson(parent, fn) {

//     let json_src = gJson[fn];

//     //alert("CreateFromJson():" + json_src);

//     let uijson = Create_ui_json();

//     //生成一个 dom 节点
//     let dom_form = JsonToUIControl(parent, json_src, uijson);

//     uijson.root_view = dom_form;

//     //return dom_form;
//     return uijson;
// }//



import_ok("ui_json_2_form.js");
