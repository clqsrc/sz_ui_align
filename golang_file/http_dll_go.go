package main;

//因为 http_dll.go 分出来的 golang 语言特性部分，这里不涉及任何的 C 安全操作，全部调用封闭好的函数



import (
	// "fmt"
	"fmt"
	"io/ioutil"
//	"database/sql"
	//"fmt"
//	"reflect"
//	"strconv"
	//"strings"
	// "syscall"
	// "unsafe"
	// "net/http"
//	"runtime"
	//"runtime/internal/atomic" //这个是内部包,不能这样引用
	//从 Go\src\syscall\os_windows.go 来看,一个 stdcall 是开了一个线程来调用一个 dll 函数的
	//不对,好象是 tstart_stdcall, newosproc 才开
	
	// "runtime/debug"
	// "runtime"
	"os/exec"
	"os"
	"encoding/json"
	"path/filepath"
	"bytes"

)


//输入，输出都是 json ，相关字符串已经在 C 接口函数中转换过了
func GO_run_golang_json(src_json string) (string) {  

	defer PrintError("run_golang_json()");

    //print("Hello ", C.GoString(src_json)); //这里不能用fmt包，会报错，调了很久...
    
    var r = "{}";
    
    var line = DecodeJson([]byte(src_json)).(map[string]interface{}) //(map[string]interface{}) 类型的强制转换语法很奇特

	var func_name = line["func_name"];  //函数名
	var param1 = ToString(line["param1"]);  //参数1
	var param2 = ToString(line["param2"]);  //参数1
	// var param3 = line["param3"];  //参数1
	// var param4 = line["param4"];  //参数1
	// var param5 = line["param5"];  //参数1

    //--------------------------------------------------------
    //设置回调函数指针 //1
    if ("set_on_event" == func_name) {
    	
		var i_param1 = StrToIntDef(param1, 0);
		//var func_handle = C.longlong(i_param1); 
    	
		GO_set_on_event(i_param1);
		
    }//
    
    //--------------------------------------------------------
    //2 列出一个目录
    if ("file_dir" == func_name) {
    	
		//var i_param1 = StrToIntDef(param1, 0);
		//var func_handle = C.longlong(i_param1); 
		var path = param1; //要查找的路径
		
		
		var put = make(map[string]interface{});
		
		var flist = File_ListAllFile(path);
		
		put["file_list"] = flist;
    	
		r = _OutputJson(put);
		
		//r = "aaa";
		
    }//
    //r = "bbb";
    //r = r + ToString(func_name);
    
    //--------------------------------------------------------
    //3 同步方式执行一个 shell 命令
    if ("shell" == func_name) {
    	
		//var i_param1 = StrToIntDef(param1, 0);
		//var func_handle = C.longlong(i_param1); 
		var cmd = param1; //命令全文
		var work_path = param2; //工作路径
		
		//----
		var result = Execute_DosCmd(cmd, work_path);
		
		//----
		
		var put = make(map[string]interface{});
		
		//var flist = File_ListAllFile(path);
		
		put["result"] = result;
    	
		r = _OutputJson(put);
		
		//r = "aaa";
		
    }//
    //r = "bbb";
    //r = r + ToString(func_name);
    
    //--------------------------------------------------------
    

	return r;
}//



//--------------------------------------------------------

//调用生成图片的程序 //第二个版本，没有 3d 的
//func Execute_DosCmd(s_cmd string, work_path string) (map[string]interface{}) {
//func Execute_DosCmd(s_cmd string, work_path string) (map[interface{}]interface{}) {
//exec.Command 的参数不好传递，还是生成一个 bat 文件吧
func Execute_DosCmd(s_cmd string, work_path string) (string) {
	
	defer PrintError("Execute_MapExe2()"); //其实可以不用
	
	//var r = make([]interface{}, 0, 1);
	//var r = make([]interface{}, 0, 1);
	
	
 

    //cmd := exec.Command(s_cmd);
    // cmd := exec.Command(GetCurrentPath() + "/map_exe2/" + "ConsoleContour.exe", 

		// // "-image_width=1920", 
		// // "-image_height=1080", 
		// "-image_width=" + image_width, 
		// "-image_height=" + image_height, 
		// "-method=mba",
		// "");
		
	fn := GetCurrentPath() + "/tmp1.bat";
	
	SaveToFile(s_cmd, fn);
		
    cmd := exec.Command(GetCurrentPath() + "/tmp1.bat");		
		
	
	//奇怪，一定要在它本根目录下才行，大概是因为找不到 dat 文件 //否则 dat 文件也要加全路径
    ////cmd.Dir = GetCurrentPath() + "/map_http_upload";   //2022.06 加上执行的工作目录，兼容性会更高
    //cmd.Dir =  "D:/app_map/_src_map/map_http_upload/"; //暂时写死  //注意，要把 Palettes.cb 也放到这个目录中，否则颜色不对
    
    //这样就可以把 php 命令行的结果重定向了
	//cmd.Stdout = os.Stdout;
    //cmd.Stderr = os.Stderr; //可选
    //cmd.Start(); //这个不会等待
	
	//cmd.Wait(); //等待进程结束//必须要有这个,否则会立即跳过去了
	
	//----
	//创建获取命令输出管道
	stdout, err := cmd.StdoutPipe();
	if err != nil {
		fmt.Printf("Error:can not obtain stdout pipe for command:%s\n", err)
		return err.Error();
	}

	//执行命令
	if err := cmd.Start(); err != nil {
		fmt.Println("Error:The command is err,", err)
		return err.Error();
	}

	//读取所有输出
	bytes, err := ioutil.ReadAll(stdout)
	if err != nil {
		fmt.Println("ReadAll Stdout:", err.Error())
		return  err.Error();
	}

	if err := cmd.Wait(); err != nil {
		fmt.Println("wait:", err.Error())
		return  err.Error();
	}

	fmt.Printf("stdout:\n\n %s", bytes);
	
	//----
	
	return string(bytes);
}//

//golang 获取当前程序执行路径
func GetCurrentPath() (string) {
	dir, err := filepath.Abs(filepath.Dir(os.Args[0]));
	if err != nil {
		//log.Fatal(err)
		return "";
	}
	fmt.Println(dir);
	
	return dir;
}//

//--------------------------------------------------------
func ttt() {
	//执行命令
	var outInfo bytes.Buffer
	//设置命令内容。第一个参数为具体命令，后面跟的多个逗号分隔开的参数
	//例如，windows系统执行命令:taskkill /pid 2021 -f，用于关闭进程，2021为待关闭进程的pid号，CMD输入这一句命令可以将pid为2021的进程关闭，而转换为Go语法则为如下代码
	cmd := exec.Command("taskkill", "/pid","2021","-f")
	//设置接收
	cmd.Stdout = &outInfo
	//执行
	cmd.Run()
	fmt.Println(outInfo.String())
}//


//--------------------------------------------------------

//一个对象转换为 json 字符串
//func Xmppmini_OutputJson(w http.ResponseWriter, r *http.Request, data interface{}) {
func _OutputJson(data interface{}) string {


    var s = "";
	
	//----
    //out := &Result{ret, reason, data}
	//b, err := json.Marshal(out)
    b, err := json.Marshal(data);
    if err != nil {
		
		//w.Write([]byte("json error"));
		s = "json error";
		
        return s;
    }
    
    
    s = string(b);
    
    return s;
	
}//

//列出全部文件
//func XmppminiGroup_ListAllUser(gname string) (interface{}) {
func File_ListAllFile(path string) (interface{}) {
	
	
	//userlist := make(map[string]interface{});
	//userlist := make([]interface{}, 0, 1);

	
	fileList, e := ioutil.ReadDir(path); //这个的结果是按文件名排序的,所以生成文件名时最好有序
	if (e != nil) { return nil; }
	
	userlist := make([]interface{}, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的

	//userlist := make(map[string]interface{});
	
	
	for i, v := range fileList {
		
		//if (i < curPos) { continue; };//分页的跳过机制
		//if (i >= endPos) { continue; };//分页的跳过机制
		
		fmt.Println(i, "=", v.Name())
		
		// uname := v.Name();	
		//uname = MakeUserName(uname);

		//fn := path + "\\" + uname + "\\user_info.txt";
		fn := v.Name();
		
		kv := make(map[string]string);
		
		kv[fn] = ToString(v.IsDir());
		
		//stop := StrToIntDef_(kv["stop"], 0);

		
		//----
		
		//userlist[i] = kv;
		//fi = append(fi, f)
		userlist = append(userlist, kv);
	
	}//for
	
	kv := make(map[string]string);
	kv["aaa"] = "bbb";
	
	userlist = append(userlist, kv);
	
	return userlist;
	
}//

