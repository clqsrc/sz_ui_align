/// <reference path="functions_nodejs.js" />


//本地存储 


//参考 https://blog.csdn.net/yiyihuazi/article/details/105488563

let _g_nw_window_max_ = 0;

function GetSaveKey(key)
{

	//nwjs 窗口最大化
	if (0 == _g_nw_window_max_){
		_g_nw_window_max_ = 1;
		NW_WindowMax();
	}//if 1

	//2022 nodejs 版本
	if ("src" == key)
	{
		//alert("GetSaveKey()");

		//let fn_cmd = "D:/clq/base_html/base_html_new/ui_json_form_map_main.json";  //从命令行参数中取得文件名

		//nwjs 取命令行参数
		let fn_cmd = NW_GetCmd();

		let s = LoadFromFile(fn_cmd);

		//alert(s[s.length-1]);  //显示最后一个字符，应该为 "}"

		//alert(s);

		return s;
	}//if 1

	//localStorage.setItem("site", "js8.in");
	return localStorage.getItem(key);
}//

// function SetSaveKey(k, v)
function SetSaveKey(key, v)
{
	//alert("SetSaveKey() 1");

	//2022 nodejs 版本
	if ("src" == key)
	{
		//alert("SetSaveKey()");

		//let fn_cmd = "D:/clq/base_html/base_html_new/ui_json_form_map_main.json";  //从命令行参数中取得文件名

		//nwjs 取命令行参数
		//let fn_cmd = NW_GetCmd();
		let fn_cmd = NW_GetCmd()[0];  //linux 下要这样

		// let s = SaveToFile(fn_cmd, v);
		SaveToFile(fn_cmd, v, 1);

		//因为有 js 的版本，所以还要存一份 js 的源码
		//--------------------------------------------------------
		let src_js_json = "";

		//let fn_js = "form_map_main.json.js";
		let fn_js = Functions_ExtractFileName(fn_cmd);

		src_js_json = src_js_json + 'SetFileJsonSrc("' + fn_js + '",\r\n';

		src_js_json = src_js_json + '`\r\n';  //注意这是反引号

		src_js_json = src_js_json + v;
		src_js_json = src_js_json + '\r\n';

		src_js_json = src_js_json + '`\r\n';  //注意这是反引号
		src_js_json = src_js_json + ');\r\n'
		src_js_json = src_js_json + 'import_ok("' + fn_js + '.js");\r\n';


		SaveToFile(fn_cmd + ".js", src_js_json, 1);

		//--------------------------------------------------------

		return;
	}//if 1

	


	//localStorage.setItem("site", "js8.in");
	localStorage.setItem(k, v);
}//


