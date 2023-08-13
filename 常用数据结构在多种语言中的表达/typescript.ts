
//typescript.ts

//如何初始化一个 any 的成员变量，或者是 "类型|null" 的变量 //只能先声明一个变量吗

var t = {

    s:"",

    uijson:null,  //这样在赋值时会报错的

    popup_mini_uijson: null as unknown as ui_json,  //这样就不会报错了

}//


//--------------------------------------------------------
//函数做参数

function View_SetOnClick_H5(view_handle:any, obj:any, func:(s:any)=>string):void
{

}//

//感觉没有 golang 的直观 //简单来说就是，有返回值时要用 “=>” 操作符号，比较难受
//func TextFile_UploadFile_andOssProgress(full_path_local string, full_path_server string, OnFunc func(listener * OssProgressListener_TextFile))
//总的来说 golang 不用写 ":" 分隔符，ts 不用写 "func/function" 关键字; ts 还要在返回值前将 ":" 修改为 "=>" , golang 则直接不用写  
function View_SetOnClick_H5(view_handle:any, obj:any, onfunc:(any)):void
{


}//

