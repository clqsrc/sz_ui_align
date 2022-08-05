

//标准化 div
//std_div.js 的强类型修改版本

//--------------------------------------------------------
//可以参考在线版 https://typescripttolua.github.io/play
//在线版转换出来的更简单清晰

//set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH%
//npx typescript-to-lua class1.ts
//tsc dom1.ts --allowJs
//npx typescript-to-lua dom1.ts --allowJs

//或者是发布是只复制 local function __TS__Class(self) 这样的函数过去就可以了

//浏览器的话要用 webpack 打包

//--------------------------------------------------------

//标题居中
//可以设置边框
//

//--------------------------------------------------------
//原有的 js 代码要写 TypeScript 声明文件 
//https://www.runoob.com/typescript/ts-ambient.html
//https://www.jianshu.com/p/150f2893d643?

/// <reference path = "js.d.ts" />
/// <reference path = "jquery.ts/jquery/index.d.ts" />

declare let ui_design_prop_form: any;   //tsc 编译需要而已

//----------------------------------------------------------------
//tsc --declaration std_div.ts 可以生成部分 d.ts

//https://qa.1r1g.com/sf/ask/888144561/


//https://cloud.tencent.com/developer/article/1890444
//还可以用 js 来反生成 ts
//sudo npm i install -g dtsmake
//3. 生成 d.ts 文件：dtsmake -s ./t.js
//它似乎依赖一个叫 tern 的库
//https://www.jianshu.com/p/d552caf3ec33
//npm install tern
//不过它似乎只生成接口，如何生成完整功能的呢？

//--------------------------------------------------------

//TS编译之后自动加上严格模式，当然这个也可以通过配置去掉！
"use strict";

AddLog("load std_div.js");

//import_("std_div_align.js", true);


//新建一个标准化 div
function CreateStdDiv(parent_div)
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
	
	MakeStdDiv(dom);
	
	return dom;
}//

//方便纯 js 的代码提示而已
function AsStdDiv(div:any):TStdDiv
{
	let _div:TStdDiv = (div as TStdDiv); //unknown

	return _div;
}//

//其实这里用 ts 来说很别扭，但是为了兼容 jquery 似乎只能继承自 HTMLElement 类族
function CreateStdDiv2(parent_div):TStdDiv
{
	if (null == parent_div || undefined == parent_div) 
		parent_div = document.body; //$(document.body).append(boarddiv); 

	//let div:TStdDiv = new TStdDiv();
	let div = document.createElement("div");
	div.innerHTML = "&nbsp";

	$(parent_div).append(div); 

	MakeStdDiv(div);

	let _div:TStdDiv = (div as TStdDiv); //unknown

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
function MakeStdDiv(div)
{
	//如果是语法错误，这里的 try 是无法捕捉的。除非是在另外一个文件中调用才可能发现错误，
	//大概是因为语法错误导致整个文件 js 全部失效了。
	//另外，据说在另外一个函数中 try 比较好
	try{
		return _MakeStdDiv(div); 
	}catch(e){
		AddLog("error at MakeStdDiv()." + e);
	}
	
}//

//标明最后一个点击的 div ，目前用在设计中
//var ui_last_click_div = null;
var ui_last_click_div:TStdDiv;
var ui_at_desgin = true;
var ui_design_root_form = null; //还是显示整体的源码吧 //设计窗体根 dom


//--------------------------------------------------------
//--------------------------------------------------------

//继承于标准 dom 节点，本意是给其加上属性和函数
//class TStdDiv implements HTMLDivElement  {  //HTMLImageElement //HTMLElement
//class TStdDiv extends HTMLDivElement  {  //HTMLImageElement //HTMLElement //HTMLDivElement
//实际上这个类并没有被创建出来，所以它就应该是 interface 而不是 class
//因为这个操作是附上标准的 dom 节点上的，也无法创建
//所以我们这里相当于写(其实是声明)它的接口而已
interface TStdDiv extends HTMLDivElement  {  //HTMLImageElement //HTMLElement //HTMLDivElement
	
	//public makeCall(phoneNumber : string) : void    //重写了基类方法
    //{
		//console.log(this.owner, "make call to ", phoneNumber, " by IPhone");
	//}

	//ts 有构造函数？
	// TStdDiv()
	// {document.createElement("div");

	// }//

	// new(): HTMLElement
	// {
	// 	return this;
	// }//

	// backgroundColor = function (color) {
	// 	$(this).css({"background-color": color});
	// 	this.ui_props.put("background-color", color); //存一下背景色
	// }

	//ui_props:Map<any, any>;
	//ui_props:any;

	//如果加 public 修饰的话会在生成 js 时变成 
	//TStdDiv.prototype.backgroundColor_
	//反而不对了

	//ts 的成员函数写法，如果写成 js 的语法就不能有代码提示了
	backgroundColor(color:any) :void;

	isDesign:any;  //表明自己是设计时的控件

	Width(w:any) :void;
	Height(h:any) :void;
	SetCaption(caption:any) :void;
	SetPos(x:any, y:any):void;
	SetId(id:any):void;

	// public backgroundColor(color) {
	// 	$(this).css({"background-color": color});
	// 	this.ui_props.put("background-color", color); //存一下背景色
	// 	//this.ui_props.set("background-color", color); //存一下背景色
	// }//

	// //public SetBackgroundColor(color) {
	// SetBackgroundColor  = function (color) {
	// 	let this_:any = this;

	// 	this_.backgroundColor(color);
	// }//

	// Left_() {

	// 	let this_:any = this;

	// 	return this_.Left();

	// 	return this.offsetLeft;
	// 	//return div.clientWidth;
	// }//

	

}//

//--------------------------------------------------------
//--------------------------------------------------------

//会有异常
function _MakeStdDiv(div)
{
	//try{
		
	//这时候的 div 是原生 dom ，本身带有大师属性值，所以为了不冲突要有命令规则
	//原生 dom 的属性多为小写开头，所以短小的属性要加 _ 下划线在前面，或者改为大写开头
	//改为下划线分割单词应该可以避免大多数冲突

	//AddLog("MakeStdDiv()");
	$(div).css({display: "inline-block", position: "absolute"});
	
	$(div).css({"box-sizing": "border-box"});
	
	//$(div).css({"background-color": "#ff0000"});
	
	// function Person(){
	
	//   this.name="xx";
	
	//   this.age=20;
	
	// }
	// var p1=new Person();
	// p1 = div;
	
	// p1.abc=function show1(){
	
	//   window.alert("hello"+this.name);
	
	// }
	
	// p1.abc();
	
	//需要保存到布局配置文件的 ui 属性
	div.ui_props = CreateHashMap();
	div.isControlPart = false;  //caption 这样的只是控件的一部分，所以不用保存
	
	div.ui = new Object();  //2021.8 更安全是写法是放将属性和方法放到这里面来，以免和 dom 原有的冲突.不过从继承的思想来说其实也没有问题。
	
	div.ui_alignTop_id  = "";  //要保存到属性文件中的值 //只能是控件的 id, 因为不能存指针值这些
	div.ui_alignLeft_id  = ""; 
	div.ui_alignBottom_id  = ""; 
	div.ui_alignRight_id  = ""; 
	
	div.ui_marginLeft = 0;  //等同于安卓的 layout_marginLeft，只有存在对齐物体时才起作用
	div.ui_marginTop = 0;
	div.ui_marginRight = 0;
	div.ui_marginBottom = 0;	

	//字体大小，这个是 ui 设计中的字体大小名，因为映射成各个系统下的字体大小时是不一样大的 
	//所以这个字段实际上是字符串
	//目前取值为:  大(large) 中(normal) 小(small) 暂定各有三级，即 small1, small2, small3 就有 9 个级别了
	div.ui_fontSize = "";	
	//--------
	//下面是非常重要的对齐属性，其实可以将它独立出去作为一个独立文件，不过目前 std_div 本身就很小
	//上下左右的 4 个对齐控件. 在写入配置文件时只能写它们的 id 号，所以要给它们设置 id,否则是不会成功的
	div.alignTop = null; //没有就表示不顶对齐
	div.alignLeft = null;
	div.alignBottom = null;
	div.alignRight = null;

	div.alignAsDivBlock = false; //像默认的 div 一样对齐，即 delphi 的 align = alTop //ios 中似乎只能动态计算 
	
	div.text_left = ""; //如果为 true 则文字对齐左边
	
	
	//---------------------------------------------------------------
	//点击事件
	div.isDesign = false; //表明自己不是设计时的控件
	$(div).click(function(){
		//alert("段落被点击了。2"); 
		if (div.isCaption) 
		{
			//alert("我是 caption"); 
			return;
		}
		
		//if (div.isDesign) return;  //设计器自身的 div 就不向下走了
		if (div.isDesign) return false;  //设计器自身的 div 就不向下走了 //2022 似乎应该是这样，因为这时候也是不让父控件响应
		//alert("我不是 caption." + $(div).attr('id')  );  //测试下来是子控件先响应
		
		ui_last_click_div = div;
		
		if (true == ui_at_desgin) 
		{		
			
			ui_design_prop_form.GetControlAllAttrib(); //取控件的所有属性 //如果有设计器的话
			this.FlashWindow(); //处于设计状态的话就闪烁一下窗体
		}
		
		return false; //不让父控件响应
	}); 
	//---------------------------------------------------------------
	
	div.backgroundColor = function (color) {
		$(this).css({"background-color": color});
		this.ui_props.put("background-color", color); //存一下背景色
	}//
	
	//div.width
	div._width = function (v) {
		
		if (undefined == v || null == v) return this.Width(); //没有值的话就当是取值
		
		$(this).css({width: v});
	}//
	
	//有些原生 dom (比如 edit) 有这个属性，所以要换一下
	//div.height
	div._height = function (v) {
		
		if (undefined == v || null == v) return this.Height(); //没有值的话就当是取值
		
		$(this).css({"height": v});
	}//
	
	//显示为占一整行
	div.SetLineDiv = function ()
	{
		//$(div).css({display: "block", position: ""});
		SetLineDiv(this)
		
		AddLog("div.SetLineDiv() ok");
	}//
	
	//圆角
	div.border_radius = function (v) {
		$(this).css({"border-radius":v});
		
		this.ui_props.put('border-radius', v); //这个属性要存一下
	}//
	
	//边框. 参考 ios 版本 
	//void UIView_SetBorderWith(UIView * view, BOOL top, BOOL left, BOOL bottom, BOOL right, UIColor * borderColor, CGFloat borderWidth)
	div.SetBorderWidth = function(top, left, bottom, right, borderColor, borderWidth)
	{
		    var color = borderColor;
		    var width = borderWidth;
			
			//border:  #ccc solid  1px;
			var v = color + " solid " + width;
		    
		    if (top)
		    {
		        $(this).css({"border-top":v});
		    }else { $(this).css({"border-top":"unset"}); } //如何去掉一个属性 //据说不行，但可以用 unset //2021.8
		    
		    if (left)
		    {
		        $(this).css({"border-left":v});
		    }else { $(this).css({"border-left":"unset"}); }
		    
		    if (bottom)
		    {
		        $(this).css({"border-bottom":v});
		    }else { $(this).css({"border-bottom":"unset"}); }
		    
		    if (right)
		    {
		        $(this).css({"border-right":v});
		    }else { $(this).css({"border-right":"unset"}); }

	}//
	
	//自由位置 . 注意只有自由布局下才有效，不过 ios 下默认就是这样的
	//ios 下不需要 parent_relative 参数
	//这个是相对父控件的定位
	//div.SetPos = function (x, y, parent_relative)
	div.SetPos = function (x, y)
	{
		//这时候父节点不是 absolute 就得是 relative
		//if (parent_relative) 
		//alert($(this.GetParent()).css("position").toLowerCase() )
		if ("absolute".toLowerCase() != $(this.GetParent()).css("position").toLowerCase())
			$(this.GetParent()).css({"position": "relative"});  //h5 里是要设置这个的,要不会相对其他 div 偏移，而不是父节点
			
		//$(this).css({"position": "relative"}); //其实还是设置相对位置，因为 ios 它们是没有绝对位置的概念的
		$(this).css({"left":x, "top":y});
	}//
	
	//自由位置 . 注意只有自由布局下才有效，不过 ios 下默认就是这样的
	//ios 下不需要 parent_relative 参数
	//这个是绝对定位，感觉尽量不要用。不过 android 的相对布局则是有可能用到的
	div.SetPos_absolute = function (x, y, parent_relative) {
		if (parent_relative) 
			$(this.GetParent()).css({"position": "relative"});  //h5 里是要设置这个的,要不会相对其他 div 偏移，而不是父节点
			
		//$(this).css({"position": "relative"}); //其实还是设置相对位置，因为 ios 它们是没有绝对位置的概念的
		$(this).css({"left":x, "top":y});
	}//	
	
	//取父节点
	div.GetParent = function () {

		//AddLog(this.parent);
		//AddLog(this.parentNode); //这个才是对的
		return this.parentNode;
	}//
	
	//居中的标题. 这个有很多方法，如果只是生成布局文件的话还是用 css 为好，不过计算绝对位置也是可以的
	//这个是比较通用的绝对位置算法
	div.SetPos_Center = function () {
		//if (parent_relative) 
		//	$(this.parent).css({"position": "relative"});  //h5 里是要设置这个的,要不会相对其他 div 偏移，而不是父节点
			
		//var p_width = $(this.parent).width();  //这个其实是不太正确的	
		//var p_height = $(this.parent).height();  //这个其实是不太正确的	
		var p_width = $(this.parentNode).width();  //这个其实是不太正确的	
		var p_height = $(this.parentNode).height();  //这个其实是不太正确的	
		
		var w = this.Width();
		var h = this.Height();
		
		AddLog(w); AddLog(h);
		AddLog(p_width + " - " + this.parentNode.clientWidth + " - " + this.parentNode.offsetWidth);
		// AddLog(p_height);
		// AddLog(this.parent);
		// AddLog(this.parentNode); //这个才是对的
		
		var x = p_width / 2.0 - w/2.0;
		var y = p_height / 2.0 - h/2.0;
			
		//$(this).css({"position": "relative"}); //其实还是设置相对位置，因为 ios 它们是没有绝对位置的概念的
		//$(this).css({"left":x, "top":y});
		this.SetPos(x, y);
	}//		
	
	//居左，但上下居中
	div.SetPos_Left = function () {
		//if (parent_relative) 
		//	$(this.parent).css({"position": "relative"});  //h5 里是要设置这个的,要不会相对其他 div 偏移，而不是父节点
			
		//var p_width = $(this.parent).width();  //这个其实是不太正确的	
		//var p_height = $(this.parent).height();  //这个其实是不太正确的	
		var p_width = $(this.parentNode).width();  //这个其实是不太正确的	
		var p_height = $(this.parentNode).height();  //这个其实是不太正确的	
		
		var w = this.Width();
		var h = this.Height();
		
		AddLog(w); AddLog(h);
		AddLog(p_width + " - " + this.parentNode.clientWidth + " - " + this.parentNode.offsetWidth);
		// AddLog(p_height);
		// AddLog(this.parent);
		// AddLog(this.parentNode); //这个才是对的
		
		var x = p_width / 2.0 - w/2.0;
		var y = p_height / 2.0 - h/2.0;
		
		x = 0; //2021.6
			
		//$(this).css({"position": "relative"}); //其实还是设置相对位置，因为 ios 它们是没有绝对位置的概念的
		//$(this).css({"left":x, "top":y});
		this.SetPos(x, y);
	}//		
	
	//宽高有两种概念，包括全部的、只含客户区的
	div.ClientWidth =  function () {
		//return div.offsetWidth;
		return this.clientWidth;
	}//
	
	//这个是全部的宽度
	div.Width =  function (v) {
		
		if (undefined != v && null != v)
			$(this).css({"width": v});
		
		return this.offsetWidth;
		//return div.clientWidth;
	}//
	
	div.ClientHeight =  function () {
		//return div.offsetWidth;
		return this.clientHeight;
	}//
	
	div.Height =  function (v) {
		
		if (undefined != v && null != v) 
			$(this).css({"height": v});
		
		return this.offsetHeight;
		//return div.clientWidth;
	}//
	
	//相对 offsetParent 的偏移位置
	div.Left =  function () {
		return this.offsetLeft;
		//return div.clientWidth;
	}//

	//相对 offsetParent 的偏移位置
	div.Top =  function () {
		return this.offsetTop;
		//return div.clientWidth;
	}//
	
	
	//再加一个 div 来实现 caption
	div._caption = null;
	div.isCaption = false;
	div.SetCaption = function (s) {
		if (null == this._caption)
			this._caption = CreateStdDiv(this);
			
		this._caption.innerHTML = s;
		this._caption.isCaption = true; //标明一下,说明它不是一个独立的 div ，只是其他 div 的附属控件
		this._caption.isControlPart = true;  //caption 这样的只是控件的一部分，所以不用保存
		
		this.ui_props.put("caption", s); //要设置属性值
		//alert($(this).css("position") != "absolute".toLowerCase()); 
		this._caption.SetPos_Center(); //修改宽度后要再次调整
	}//	
	
	//私有函数 重置标题位置
	div.reSetCaptionPos = function (s) {
		if (null != this._caption)
		 {
			 
			this._caption.SetPos_Center(); //修改宽度后要再次调整
			
			if (this.text_left == "true")
			this._caption.SetPos_Left(); //修改宽度后要再次调整
			
			if (this.ui_props && this.ui_props.get("text_left") == "true") //2021 文字靠左
			{
				//alert(this.ui_props.get("text_left"));
				this._caption.SetPos_Left(); //修改宽度后要再次调整
			}
			
		 }
	}//

	//根据 4 个对齐对象调整位置 //比较复杂，移动到 std_div_align.js 当中
	//div.SetPos_forAlignObject = function()
	AssignDivAlignFunction(div);
	
	//给 div 设置id
	div.SetId =  function (s) {
		$(this).attr('id', s);            //给 div 设置id
		
		this.ui_props.put('id', s); //这个属性要存一下
	}//
	
	//暂时 //显示在最上层
	div.ShowAtTop =  function (s) {
		this.style.zIndex = 10000;
	}//
	
	div.Show =  function () {
		$(this).show();
	}//
	
	div.Hide =  function () {
		$(this).hide();
	}//
	
	//取 css 值，方便性函数，不跨平台
	div.css =  function (key) {
		//return $(p).css("left");
		return $(this).css(key);
	}//
	
	//让 div 闪烁一下，一般用于提示控件位置
	div.FlashWindow =  function (color) {

		var old_color = $(this).css("background-color");
		
		if (undefined == color || null == color)
			color = "#ff0000";
		
		//$(this).css({"background-color": "#ff0000"});
		$(this).css({"background-color": color});
		
		//5秒后执行testFunction()函数，只执行一次。
		setTimeout(function(){ 
				//alert(old_color); 
				//$(ui_last_click_div).css({"background-color": "#ffffff"});
				$(div).css({"background-color": old_color});
			}, 
			500);  //5秒后执行testFunction()函数，只执行一次。
		
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

//显示为占一整行
function SetLineDiv(div)
{
	$(div).css({display: "block", position: ""});
	
}//

//生成一个 ui 配置文件中的控件 id
var var_count_for_GetUI_NewID = 0; //目前来说只能用全局变量累加
function GetUI_NewID()
{
	var_count_for_GetUI_NewID++;
	
	return "id_" + var_count_for_GetUI_NewID;
}//

import_ok("std_div.js");
AddLog("load std_div.js ok");