

//map


//json


//函数作为函数的参数

//可以看到，直接写成函数的声明样式就可以了，只是不要原函数中的参数名

func listenSSL(port int, f func(net.Conn)) {

}

func File_FindFiles_onfunc(path string, out_list * []map[string]string, find_sub bool, file_type string, on_func func(string)bool)  {
}

//--------------------------------------------------------

//2.
//可写出类似 js 一样的方便的结构体变量再转换成 json
type json_map map[string]interface{} //定义一个记录行
type json_arr []interface{} //定义一个记录行动态数组,做为结果集

//这两个结构体配合 ToJson() 函数，基本上可以写出和 js 一样方便的 json obj 的构造语法代码，例如：（原始需求来自 oss 的 sts 请求构造）
	var policy_item1 json_map = json_map{
	
		"Effect": "Allow",
		"Action":json_arr{"oss:ListObjects"},
		
		//"Resource":_Resource_arr ,
		"Condition":json_map{
					"StringLike":json_map{
						"oss:Prefix":"$path/*",
						//"oss:Prefix":path + "/*",
					},
	    },
	
	};

// var r = ToJson(policy_item1);
//2022.11.08 其实和 js 一样，golang 任何对象都能转换成 json
func ObjToJson(data interface{}) string {

    b, err := json.Marshal(data);
    if err != nil {
		
		//w.Write([]byte("json error"));
		
        return "{}";
    }
    // w.Write(b);
    
    return string(b);
	
}//


//2023.6 解码任意类型的 json
buf := HttpGet_TimeOut(url, 5);

	rjson := DecodeJson_toMapList(buf);

	//line_list := make( [] map[string]interface{}, 0); //这是一个 map 数组
	//line_list := new (interface{}); //ok//这是一个 map 数组 //不是数组，是单个的对象
	//line_list := new (map[string]string); //ok  //取不出 Succeed 的 True
	line_list := new (map[string]interface{}); //ok

	err := json.Unmarshal(buf, &line_list) //居然是可以解码到 map 中的
	if err != nil {
		fmt.Println("error:", err)
	}

	Succeed := (*line_list)["Succeed"];

	if (Succeed == true) {
		vcl.ShowMessage("成功");
	}else{
		vcl.ShowMessage("失败");
	}

	fmt.Println(rjson);
	fmt.Println(Succeed);

	vcl.ShowMessage(string(buf));

//----------------------------------------------------------------
//动态数组

//其实 golang 术语叫切片
var JSVM_List = make([]string, 0);

JSVM_List = append(JSVM_List, "");


