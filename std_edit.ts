

//标准化 edit ，另外应该是支持所有 std_div 属性、接口的
//std_edit.js 的强类型修改版本

//--------------------------------------------------------
//最原始的 edit ，因为在实现过程中发现默认居中的 edit 组合控件实际上非常难在各平台下统一实现

//--------------------------------------------------------
//原有的 js 代码要写 TypeScript 声明文件 
//https://www.runoob.com/typescript/ts-ambient.html
//https://www.jianshu.com/p/150f2893d643?

/// <reference path = "js.d.ts" />
/// <reference path = "jquery.ts/jquery/index.d.ts" />
/// <reference path = "std_div.ts" />

//declare let ui_design_prop_form: any;   //tsc 编译需要而已

//----------------------------------------------------------------

//TS编译之后自动加上严格模式，当然这个也可以通过配置去掉！
"use strict";

AddLog("load std_edit.js");

//import_("std_div_align.js", true);


//新建一个标准化 div
function CreateStdEdit_v1(parent_div)
{
	if (null == parent_div || undefined == parent_div) 
		parent_div = document.body; //$(document.body).append(boarddiv); 
	
	var childdiv = $('<div>&nbsp</div>');        //创建一个子div
	//childdiv.attr('id','child');            //给子div设置id
	//childdiv.addclass('childdiv');    //添加css样式
	//childdiv.appendto(parent_div);        //将子div添加到父div中
	//parentdiv.appendto('body');            //将父div添加到body中
	
	$(parent_div).append(childdiv); 
	
	var dom = childdiv.get(0);  //DOM对象 
	
	MakeStdDiv(dom);    //这一步的确是要有的
	MakeStdEdit(dom);   //这是 edit 特有的
	
	return dom;
}//

//方便纯 js 的代码提示而已
function AsStdEdit(div:any):TStdEdit
{
	let _div:TStdDiv = (div as TStdEdit); //unknown

	return _div;
}//

//其实这里用 ts 来说很别扭，但是为了兼容 jquery 似乎只能继承自 HTMLElement 类族
function CreateStdEdit(parent_div):TStdEdit
{
	if (null == parent_div || undefined == parent_div) 
		parent_div = document.body; //$(document.body).append(boarddiv); 

	//let div:TStdDiv = new TStdDiv();
	//let div = document.createElement("div");
	//let div = document.createElement("input");  //这个 ts 下编译不了，因为返回值是 HTMLInputElement 的原因吗?
	let div:HTMLElement = document.createElement("input");

	//div.innerHTML = "&nbsp";

	$(parent_div).append(div); 

	MakeStdDiv(div);    //这一步的确是要有的
	MakeStdEdit(div);   //这是 edit 特有的

	let _div:TStdEdit = (div as TStdEdit); //unknown

	return _div;

	//--------
	/*
	var childdiv = $('<div>&nbsp</div>');        //创建一个子div
	//childdiv.attr('id','child');            //给子div设置id
	//childdiv.addclass('childdiv');    //添加css样式
	//childdiv.appendto(parent_div);        //将子div添加到父div中
	//parentdiv.appendto('body');            //将父div添加到body中
	
	$(parent_div).append(childdiv); 
	
	var dom = childdiv.get(0);  //DOM对象 
	
	MakeStdDiv(dom);
	
	return dom;
	*/
}//

//标准化一个 div
function MakeStdEdit(div)
{
	//如果是语法错误，这里的 try 是无法捕捉的。除非是在另外一个文件中调用才可能发现错误，
	//大概是因为语法错误导致整个文件 js 全部失效了。
	//另外，据说在另外一个函数中 try 比较好
	try{
		return _MakeStdEdit(div); 
	}catch(e){
		AddLog("error at MakeStdEdit()." + e);
	}
	
}//

//标明最后一个点击的 div ，目前用在设计中
//var ui_last_click_div = null;
var ui_last_click_div:TStdDiv;
var ui_at_desgin = true;
var ui_design_root_form = null; //还是显示整体的源码吧 //设计窗体根 dom


//--------------------------------------------------------
//--------------------------------------------------------

//参考 TStdDiv 
interface TStdEdit extends HTMLDivElement  {  //HTMLImageElement //HTMLElement //HTMLDivElement
	

	backgroundColor(color:any) :void;

	//isDesign:any;  //表明自己是设计时的控件
	isControlPart:any;
	isCaption:any; //isCaption 和 isSubControl, isControlPart 都表示事件要给父亲控件处理
	isSubControl:any;
	isDesign:any;  //表明自己是设计时的控件

	Width(w:any) :void;
	Height(h:any) :void;
	SetCaption(caption:any) :void;
	SetPos(x:any, y:any):void;
	SetId(id:any):void;

	//--------

	

}//

//--------------------------------------------------------
//--------------------------------------------------------

//会有异常
function _MakeStdEdit(div)
{
	//try{
		
	//这时候的 div 是原生 dom ，本身带有大师属性值，所以为了不冲突要有命令规则
	//原生 dom 的属性多为小写开头，所以短小的属性要加 _ 下划线在前面，或者改为大写开头
	//改为下划线分割单词应该可以避免大多数冲突

	//AddLog("MakeStdDiv()");
	$(div).css({display: "inline-block", position: "absolute"});
	
	$(div).css({"box-sizing": "border-box"});
	
	//$(div).css({"background-color": "#ff0000"});

	//----
	//input 特有的 css
	$(div).css({"outline": "none"});
	//$(div).css({"border-width": "0px"});
	$(div).css({"border-color": "#FF000000"});  //应该是 rgba 格式，用这个比 "border-width": "0px" 好，
	//因为那样会让文字上提，而这个不会改变布局本身，只是用了透明边框

	
	//需要保存到布局配置文件的 ui 属性
	//div.ui_props = CreateHashMap();
	//div.isControlPart = false;  //caption 这样的只是控件的一部分，所以不用保存
	




	//div.alignAsDivBlock = false; //像默认的 div 一样对齐，即 delphi 的 align = alTop //ios 中似乎只能动态计算 
	
	//div.text_left = ""; //如果为 true 则文字对齐左边
	

	//---------------------------------------------------------------
	
	
	//有些原生 dom (比如 edit) 有这个属性，所以要换一下
	div.SetText = function (v) {
		
		$(this).val(v);

	}//

	div.GetText = function (v) {
		
		return $(this).val();
	}//
	
	
	
	//---------------------------------------------------------------
	//div.SetBackgroundColor("#ff0000");
	
	div.SetId(GetUI_NewID()); //给一个默认 id
	
	//AddLog("MakeStdDiv() ok");
	
	return div;
	// }catch(e){
		
	// 	AddLog("error.")
	// }	
}//



import_ok("std_edit.js");
AddLog("load std_edit.js ok");