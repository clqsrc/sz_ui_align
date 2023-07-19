
//ui 设计器的属性窗口

//每个属性控件增加一个 GetValueToShow 接口函数，窗口保存所有的属性控件列表，在需要显示控件值时全部调用一次就可以了

AddLog("load ui_design_prop.js");

// import_ok("ui_design_prop.js");

//import_("functions.js", true); 
import_("control_edit.js", true);
//alert(import_)

//测试一下旧版本的 json 解析,看看 top 读取后有没有带 px ,如果不有看看没有 px 时是怎样写入的 //px 是 css 属性的聚会带来的

var ui_design_prop_form = null;  //奇怪，以前为什么去掉
//创建设计器，主要是属性编辑功能
function ui_design_prop_create(parent)
{
	//属性的列表很多，所以放一个高度标识 //大家都用这个标识作为自己的当前位置，同时标明自己需要占用的高度
	//单位目前是 px
	var propEdit_curTop = 0;
	function AddTop(self_height)
	{
		var r = propEdit_curTop;
		
		propEdit_curTop += self_height;
		
		r = String(r) + "px";  //js 下需要这样处理
		return r;
	}//
	
	//---------------------------------------------------------------
	
	var line = CreateStdDiv(parent);
	line.isDesign = true;  //表明自己是设计时的控件
	line.ValuesControl = CreateHashMap(); //属性值控件列表
	line.ValuesControlEdit = CreateList();
	//line.Height("900px");
	line.Height("500px");
	$(line).css({"overflow-y":"scroll"}); //太长不好用，加滚动条好了
	$(line).css({"overflow-x":"hidden"}); //太长不好用，加滚动条好了
	line.backgroundColor("#eeeeee");
	line.Width("250px"); //line.width("80%");
	//暂时 //显示在最上层
	line.ShowAtTop();
	line.border_radius("5px");
	line.SetBorderWidth(true, true, true, true, "#cccccc", "1px");
	//line.SetPos("0px", "180px");
	line.SetPos("0px", "260px");
	
	line.alignRight = line.GetParent();
	line.SetPos_forAlignObject();
	
	function CreateBtn(x, y, caption)
	{
		//top 对齐属性
		var o = CreateStdDiv(line);
		o.isDesign = true;  //表明自己是设计时的控件
		//var btnAlignTop = o;
		o.Height("40px");
		//o.backgroundColor("#eeeeee");
		o.Width("40px");
		o.border_radius("5px");
		o.SetBorderWidth(true, true, true, true, "#cccccc", "1px");
		o.SetCaption(caption);
		o.SetPos(x, y);
		
		$(o).click(function(){ 
			var p = ui_last_click_div;
			
			//alert(p.alignTop);
			
			if (null == p.alignTop) return;
			//alert(p.alignTop.innerHTML);
			alert(p.alignTop);  //现在不一定是类
			
		});  
	}//
	CreateBtn("80px", "4px", "top");
	CreatePropEdit("124px", "4px", "50px", "alignTop");  //对齐的物体
	CreatePropEdit("180px", "4px",  "40px", "ui_marginTop");
	
	//左 属性
	CreateBtn("4px", "54px", "left");
	CreatePropEdit("48px", "54px","50px",  "alignLeft");  //对齐的物体
	CreatePropEdit("48px", "100px",  "40px",  "ui_marginLeft");
	
	//右属性
	CreateBtn("116px", "54px", "right");	
	CreatePropEdit("160px", "54px", "50px",  "alignRight");  //对齐的物体
	CreatePropEdit("160px", "100px",  "40px", "ui_marginRight");
	
	//bottom 属性
	CreateBtn("80px", "154px", "bottom");	
	CreatePropEdit("124px", "154px", "50px", "alignBottom");  //对齐的物体	
	CreatePropEdit("180px", "154px",  "40px", "ui_marginBottom");
	
	//left pos

	//能够像设置高度一样的属性都可以用这个
	propEdit_curTop = 200;  //属性控件当前 top 位置
	
	//CreateHeightPropSet("200px", "left", "Left:");
	CreatePropSet_Line(AddTop(50), "left", "Left:");
	CreatePropSet_Line(AddTop(50), "top", "Top:");
	CreatePropSet_Line(AddTop(50), "height", "Height:");
	CreatePropSet_Line(AddTop(50), "width", "Width:");
	CreatePropSet_Line(AddTop(50), "id", "Id:",  "jquery_attrib");
	CreatePropSet_Line(AddTop(50), "background-color", "背景色:",  "jquery_css", true);
	CreatePropSet_Line(AddTop(50), "caption", "标题:",  "ui_props");
	CreatePropSet_Line(AddTop(50), "ui_class", "类名:",  "ui_props");  //用来实现这个控件的实际类名 

	ui_design_prop_class_create(document.getElementById("ui_class_edit")); //2022
	
	CreatePropSet_Line(AddTop(50), "border_top",  "上边框:",  "ui_props");   //下边框是否显示. true , false
	CreatePropSet_Line(AddTop(50), "border_bottom",  "下边框:",  "ui_props");   //下边框是否显示. true , false
	CreatePropSet_Line(AddTop(50), "border_left",  "左边框:",  "ui_props");   //下边框是否显示. true , false
	CreatePropSet_Line(AddTop(50), "border_right",  "右边框:",  "ui_props");   //下边框是否显示. true , false

	CreatePropSet_Line(AddTop(50), "border_color",  "边框颜色",  "ui_props");   //边框色//默认全局
	CreatePropSet_Line(AddTop(50), "border_width",  "边框宽度",  "ui_props");   //宽度//默认为 1,或者是全局最小宽度

	
	CreatePropSet_Line(AddTop(50), "border_radius",  "圆角:",  "ui_props");   //圆角
//	CreateBtn("4px", "850px", "字体大小");  CreateMarginPropSet("850px", "54px", "ui_fontSize"); //直接操作 dom 属性
	CreatePropSet_Line(AddTop(50), "ui_fontSize", "字体大小",  "ui_props");  //2021.10.26
	CreatePropSet_Line(AddTop(50), "font-size", "fs css",  "jquery_css", false);
	CreatePropSet_Line(AddTop(50), "text_left", "标题靠左",  "ui_props");  //标题靠左
	CreatePropSet_Line(AddTop(50), "text_color", "标题颜色",  "ui_props");  //标题颜色
	CreatePropSet_Line(AddTop(50), "ui_img_name", "图片文件",  "ui_props");  //带图片的 view 还是太常见了，还是独立为一个类为好
	
	//记得在 ui_design_src.js 文件的  design_prop_key_list 数组中也要加新添的属性
	//--------
	//设置属性值编辑框的 css
	function SetValueEditCss(id)
	{
		$("#" + id).css({"border":"red solid 0px"});
		//第一种是HTML5的透明，在H5中支持透明背景颜色，但遗憾的是，H5中的半透明背景颜色只支持 rgba的写法，不支持16进制的写法如：
		//background-color:rgba(0,152,50,0.7);// -->70%的不透明度
		//background-color:transparent;支持完全透明
		$("#" + id).css({"background-color": "rgba(255,255,255, 0.9)"});
		
		$("#" + id).css({"left": "8px", "position": "absolute"});
		$("#" + id).css({"width": "110px"});
	}//
	
	//--------
	//高度设置
	//value_type 可以是  "jquery_css" 也可以是 "jquery_attrib"
	//save_raw_value 背景色存入后取出会变化，所以要存一下原始值
	//function CreateHeightPropSet(cur_top, css_key, ui_caption, value_type, save_raw_value)
	function CreatePropSet_Line(cur_top, css_key, ui_caption, value_type, save_raw_value)
	{
		// var cur_top = "254px"; //要放置的位置
		// var css_key = "height"; //要操作的 css 属性
		// var ui_caption = "Height:"; //显示出来的设置名
		//left pos
		var o = CreateStdDiv(line);
		o.isDesign = true;  //表明自己是设计时的控件
		line.ValuesControl.put(css_key, o); //指定这个控件管理这个属性值
		line.ValuesControlEdit.Add(o);
		var btnHeightPos = o;
		o.Height("40px");
		//o.backgroundColor("#eeeeee");
		o.Width("40px");
		o.border_radius("5px");
		o.SetBorderWidth(true, true, true, true, "#cccccc", "1px");
		o.SetCaption(ui_caption);
		o.SetPos("4px", cur_top);
		//o.SetId(css_key);  //2022 应该可以直接用 
		
		//显示上控件上的函数
		o.GetValueToShow = function () {
			var v = GetValue(ui_last_click_div);
			$("#" + o.id + "_edit").val(v);
		}//
		
		//left pos value
		var o = CreateStdDiv(line);
		o.isDesign = true;  //表明自己是设计时的控件
		var btnHeightPosValue = o;
		o.Height("40px");
		o.SetId(css_key);  //2022 应该可以直接用
		//o.backgroundColor("#eeeeee");
		o.Width("140px");
		o.border_radius("5px");
		o.SetBorderWidth(true, true, true, true, "#cccccc", "1px");
		o.SetCaption("<input type='text' id=" + o.id + "_edit  value='" + "" + "' />");
		o.SetPos("54px", cur_top);
		
		$("#" + o.id + "_edit").css({"width": "80%"}); //限制下宽度，因为手机下可能超出父节点宽度
		$("#" + o.id + "_edit").css({"background-color": "#00ff0000"});
		
		SetValueEditCss(o.id + "_edit");
		
		//--------
		//根据不同的类型选择不同的取值，赋值方式
		function GetValue(obj)
		{
			//var p = ui_last_click_div;
			return GetPropValue(obj, css_key);  //2021.8 原来的太复杂了，统一一个
			//return $(obj).css(css_key); //默认取 css 的属性值
		}
		
		function SetValue(obj, value)
		{
			//var p = ui_last_click_div;

			//$(obj).css(String(css_key), value);  //奇怪，不加字符串转换就是错误
			SetPropValue(obj, String(css_key), value);

		}//func
		
		//----
		$(o).click(function(){ 
			
			var value = $("#" + o.id + "_edit").val();
			
			//alert("段落被点击了。" + value); 
			
			var p = ui_last_click_div;
			
			// if (value.length<1)  { 
			// 	//$("#" + o.id + "_edit").val(ui_last_click_div.css(css_key)); 
			// 	$("#" + o.id + "_edit").val(GetValue(p)); 
			// 	return;
			// }//if
			
			//$(p).css(String(css_key), value);  //奇怪，不加字符串转换就是错误
			SetValue(p, value);
			p.ui_props.put(css_key, value); //存一下属性
			
			//var old_color = $(ui_last_click_div).css("background-color");
			//$(ui_last_click_div).css({"background-color": "#ff0000"});
		
			//alert($(p).css("left"));
		});  
	}//
	
	

	//---------------------------------------------------------------
	
	//只有编辑框的 //属性编辑框
	//function CreateMarginPropSet(cur_top, x, css_key)
	function CreatePropEdit(x, y, width, prop_key)
	{
		//var o = CreateStdDiv(line); o.ui_marginTop

		//left pos value
		var o = CreateStdDiv(line);
		o.isDesign = true;  //表明自己是设计时的控件
		
		line.ValuesControl.put(prop_key, o); //指定这个控件管理这个属性值
		line.ValuesControlEdit.Add(o);		
		
		//var btnHeightPosValue = o;
		o.Height("40px");
		//o.backgroundColor("#dddddd");
		//o.Width("40px");
		o.Width(width);
		o.border_radius("5px");
		o.SetBorderWidth(true, true, true, true, "#cccccc", "1px");
		//o.SetCaption("<input type='text' id=" + o.id + "_edit  value='" + "" + "' />");
		o.SetPos(x, y);
		
		var edit = CreateEditControl(o);
		edit.isDesign = true;
		edit.SetId( o.id + "_edit");
		
		$("#" + o.id + "_edit").css({"width": "80%"}); //限制下宽度，因为手机下可能超出父节点宽度
		$("#" + o.id + "_edit").css({"background-color": "#00ff0000"});
		
		SetValueEditCss(o.id + "_edit");
		$("#" + o.id + "_edit").css({"width": "40px"});
		//$("#" + o.id + "_edit").css({"left": "8px"});
		//SetValueEditCss_forObjID(o.id + "_edit");
		//o._caption.SetPos_Center();
		edit.SetPos_Center();
		
		//显示在控件上的函数
		o.GetValueToShow = function () {
			var v = GetValue(ui_last_click_div);
			$("#" + o.id + "_edit").val(v);
		}//
		
		//--------
		//根据不同的类型选择不同的取值，赋值方式
		function GetValue(obj)
		{
			return GetPropValue(obj, prop_key);
		}
		
		function SetValue(obj, value)
		{
			SetPropValue(obj, prop_key, value); return; 
		}
		
		//----
		//$(o).click(  //现在要明确指定让 edit 控件触发事件，因为现在 edit 也会阻止事件发送给父控件
		$("#" + o.id + "_edit").click(
			
			function(){ 
			
			var value = $("#" + o.id + "_edit").val();
			
			//alert("段落被点击了。" + value); 
			
			var p = ui_last_click_div;
			
			// if (value.length<1)  { 
			// 	//$("#" + o.id + "_edit").val(ui_last_click_div.css(css_key)); 
			// 	$("#" + o.id + "_edit").val(GetValue(p)); 
			// 	return;
			// }//if
			
			//$(p).css(String(css_key), value);  //奇怪，不加字符串转换就是错误
			SetValue(p, value);
			p.ui_props.put(prop_key, value); //存一下属性
			
			//var old_color = $(ui_last_click_div).css("background-color");
			//$(ui_last_click_div).css({"background-color": "#ff0000"});
		
			//alert($(p).css("left"));
		});  
	}//
	

	//---------------------------------------------------------------
	ui_design_prop_form = line;
	
	//取控件的所有属性
	ui_design_prop_form.GetControlAllAttrib = function()
	{
		for (var i=0; i<this.ValuesControlEdit.Count(); i++)
		{
			this.ValuesControlEdit[i].GetValueToShow();
		}
		//this.ValuesControlEdit[0].GetValueToShow();
	}//
	
	return line;
	
}//

//---------------------------------------------------------------
//2021.8  原来的取值太复杂了，还是按属性的名称区分比较简单，原来按类型的久了难以维护
function GetPropValue(obj, css_key)
{
	//var p = ui_last_click_div;
	
	//if ("jquery_attrib" == value_type)
	//	return $(obj).attr(css_key); 
	//else if ("ui_props" == value_type)
	//	return (obj).ui_props.get(css_key); 
	//else
	
	if ("background-color" == css_key)
		return (obj).ui_props.get("raw_" + css_key);
		
	if (
		("ui_image_name" == css_key)
		|| ("border_radius" == css_key)
		)
	{
		return (obj).ui_props.get(css_key);
	}
		
	//早前很多属性是直接在 dom 上用 js 的动态扩展属性实现的，但实际使用下来有时候属性会与原来 js 的相冲突
	//不过目前也没有更好的解决办法，只是将新的属性放到 ui_props 中去了
	//以后可以考虑将设计属性都放到 ui_props 里，而将普通变量和函数放到 obj.ui 里
	
	if (
		("ui_marginTop" == css_key)
		|| ("ui_marginLeft" == css_key)
		|| ("ui_marginRight" == css_key)
		|| ("ui_marginBottom" == css_key)
		)
	{
		//"alignTop"
		//"alignLeft"
		//"alignRight"
		//"alignBottom"
		
		return (obj).ui_props.get(css_key);  //似乎应该是这个,以后应该杜绝 obj[String(css_key)] 因为太容易和原来的 dom 原始属性冲突了
		
		//return obj[String(css_key)];
	}
	
	//id 属性是比较特殊的
	if ("id" == css_key)
	{
		return $(obj).attr(css_key); 
	}
	
	//----  这几个是对象的，也很特殊
	//2021.12.03 原来的太复杂了，根据长久使用的体验其实全部对齐父节点就完全够用了
	//所以全部改为父节点对齐
	//所以这里是值不用特殊处理，直接给原值，或者是给父节的 id 就可以了。
	//目前先试试给原值看看有没有什么影响
	/*
	var obj_name = css_key;
	if ("alignTop" == obj_name)
		return $(obj.alignTop).attr("id"); 
	
	if ("alignLeft" == obj_name)
		return $(obj.alignLeft).attr("id"); 
	
	if ("alignRight" == obj_name)
		return $(obj.alignRight).attr("id"); 
	
	if ("alignBottom" == obj_name)
		return $(obj.alignBottom).attr("id"); 
	
	*/
   
	//----
	//位置也是比较重要的
	if (
		("top" == css_key)
		|| ("left" == css_key)
		|| ("right" == css_key)
		|| ("width" == css_key)
		|| ("height" == css_key)
		)
	{

		//用这个才会加上 px
		//return $(obj).css(css_key); 
		
		//还是去掉 px 吧，因为我们是跨平台的，要去掉 js 化的东西
		////return StrToInt((obj).css(css_key)); //不过 css 值未必存在，所以我们要不取原始值， 不取 ui_prop 的原始值，
		//目前来说是有点两难。原始值有没设置属性的情况下也会被改变，但 ui_prop 中的值不一有，而且每次修改位置
		//大小时写完 js 代码后都要同步更新它。
		if("left" == css_key) return obj.Left();
		if("top" == css_key) return obj.Top();
		
		if("width" == css_key)
		 {
			 var value = obj.Width();
			 
			 return obj.Width();
		 }
		
		if("height" == css_key) return obj.Height();
		
		return StrToInt((obj).css(css_key)); //css 特有的就这样返回吧
	}
	
	//----
	
	//return $(obj).css(css_key); //默认取 css 的属性值
	return (obj).ui_props.get(css_key);  //应该默认返回 ui_props 中的原始设置值
}


//2021.8
//var in_create = true; //标明是在创建过程，象对齐这样的动作先不做，在创建完成后统一处理
function SetPropValue(obj, css_key, value, in_create)
{
	// if ("caption" == css_key) {
	// 	obj.SetCaption(value);
		
	// 	return;
	// }
		
	obj.ui_props.put(css_key, value);
	
	if ("background-color" == css_key)
	{
		(obj).ui_props.put("raw_" + css_key, value);
		//obj[String(css_key)] = value;
	}
	
	if (
		("ui_marginTop" == css_key)
		|| ("ui_marginLeft" == css_key)
		|| ("ui_marginRight" == css_key)
		|| ("ui_marginBottom" == css_key)
		)
	{
		//"alignTop"
		//"alignLeft"
		//"alignRight"
		//"alignBottom"
		
		obj[String(css_key)] = value;
		
		//修改了对齐物体，所以要调整位置
		if (!in_create)
		obj.SetPos_forAlignObject();
	}
	
	//id 属性是比较特殊的
	if ("id" == css_key)
	{
		$(obj).attr(String(css_key), value);  //奇怪，不加字符串转换就是错误
	}
	
	//----  这几个是对象属性，也是比较特殊的
	//var align_obj = document.getElementById(value);
	var align_obj = value;  //现在直接用原值，不一定是类
	obj_name = css_key;
	
	if ("alignTop" == obj_name)
	{
		obj.alignTop = align_obj;
		if (!in_create)
		obj.SetPos_forAlignObject();
	}
		
	if ("alignLeft" == obj_name)
	{
		obj.alignLeft = align_obj;
		if (!in_create)
		obj.SetPos_forAlignObject();
	}
		
	if ("alignRight" == obj_name)
	{
		obj.alignRight = align_obj;
		if (!in_create)
		obj.SetPos_forAlignObject();
	}
		
	if ("alignBottom" == obj_name)
	{
		obj.alignBottom = align_obj;
		if (!in_create)
		obj.SetPos_forAlignObject();
	}
		
	//修改了对齐物体，所以要调整位置
	//obj.SetPos_forAlignObject();
	
	//----
	//位置也是比较重要的
	if (
		("top" == css_key)
		|| ("left" == css_key)
		|| ("right" == css_key)
		|| ("bottom" == css_key)
		|| ("width" == css_key)
		|| ("height" == css_key)
		)
	{
		
		//obj[String(css_key)] = value;
		
		//如果这时 value 是 "100" 字符串的话，不会成功。只能接受 "100px" 或者 "100pt" 这样的
		////$(obj).css(String(css_key), value);  //目前的 jquery 和 js 版本只接受 px pt 这样带单位的字符串。光是数字是不生效的，
		//但糟糕的是它也不报错
		
		//转换成整数类型后就可以接受 value 是 "100" 字符串了。这应该是个很特殊的方式，另外这种情况下单位默认应该就是 px。
		$(obj).css(String(css_key), StrToInt(value));
		
		//修改了对齐物体，所以要调整位置
		//obj.SetPos_forAlignObject();
	}
	
	//----
	
	//2021.8  //click 事件中都已经调用 p.ui_props.put(css_key, value); //存一下属性 了，所以这里不用存值
	PropValue_Show(obj,String(css_key), value);


	//$(obj).css(String(css_key), value);  //奇怪，不加字符串转换就是错误

}//func


//2021.8 配置的值在 ui 中显示出变化
//单个的属性才可以这样处理，对齐这些比较复杂的暂时不行
function PropValue_Show(obj, key, value)
{
	if ("caption" == key) {
		obj.SetCaption(value);
		
	}
		
	if ("background-color" == key) {
	
		var vcolor = get_config_color(value);
		$(obj).css({"background-color": vcolor});

	}
	
	//2021.8  //click 事件中都已经调用 p.ui_props.put(css_key, value); //存一下属性 了，所以这里不用存值
	if ("text_color" == key) {

		//----------------
		//特定的类 //2023
		let j = obj["ui_props"]; //这个是 CreateHashMap 来的，所以要用它的函数，而不是 js 操作来取其中的 kv
		//调试还是比较啰嗦，以后还是要改成全 ts

		// let ui_class = j["ui_class"];
		// let id = j["id"];
		let ui_class = j.get("ui_class");
		let id = j.get("id");

		//----------------

		var text_color = get_config_color(value);
		//if (String(text_color).length > 0) //空字符串值也是有效值，否则就不能恢复到缺省值了
		{
			$(obj).css({"color":text_color});
		}//

		//---- 2023 给 edit 加颜色
		if ("edit" == ui_class){
			$("#" + id + "_edit").css({"color":text_color});
		}//if 2
		
	}//if 1
	
	if ("border_radius" == key) {
	
		//if (String(value).length > 0) 
		obj.border_radius(value); //给 div 加实际的圆角 css 
		
	}
	
	if (
		  ("border_top"     == key)
		|| ("border_left"   == key)
		|| ("border_right"  == key)
		|| ("border_bottom" == key)
		)
	{
		var border_top    = String(obj.ui_props.get("border_top"));
		var border_bottom = String(obj.ui_props.get("border_bottom"));
		var border_left   = String(obj.ui_props.get("border_left"));
		var border_right  = String(obj.ui_props.get("border_right"));
		
		var borderColor = "#cccccc";  //临时，以后还是要再指定的
		var borderWidth = "1px";  //临时，以后还是要再指定的
		obj.SetBorderWidth(StrToBool(border_top), StrToBool(border_left), StrToBool(border_bottom), StrToBool(border_right), borderColor, borderWidth);
		
	}
	
	//$(obj).css(String(css_key), value);  //奇怪，不加字符串转换就是错误

}//func

AddLog("load ui_design_prop.js ok");
import_ok("ui_design_prop.js");
//alert("");
