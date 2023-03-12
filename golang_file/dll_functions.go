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
	// "os/exec"
	// "os"
	// "encoding/json"
	// "path/filepath"
	// "bytes"

)

//来自 libgolang dll 的函数
//dll_functions.go



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



func StringList_Create() ([]map[string]string) {


	//userlist := make(map[string]interface{});
	//userlist := make([]interface{}, 0, 1);

	
	// fileList, e := ioutil.ReadDir(path); //这个的结果是按文件名排序的,所以生成文件名时最好有序
	// if (e != nil) { return nil; }
	
	////userlist := make([]interface{}, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的
	userlist := make([]map[string]string, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的

	return userlist;

}//

//procedure Find_in_dir1(path1:string;out1:tstringlist;sub1:boolean;type1:string);
//兼容 delphi 的函数名而已
func Find_in_dir1(path string, out_list * []map[string]string, find_sub bool, file_type string) {

	File_FindFiles(path, out_list, find_sub, file_type);

}//


//2023 改为可递归，并且返回的数据结构的字段内容不同了
//func File_FindFiles(path string, out_list []map[string]string) (dblines) {
//不行，要作为指针传入才行 //因为 golang 都是值传递
//也就是说 golang 中的数组作为参数时并不引用，而是整体复制
//file_type 可以是 "all", "dir" ，否则的话就是只搜索文件不要目录, "file" 就表示只要文件
func File_FindFiles(path string, out_list * []map[string]string, find_sub bool, file_type string)  {

	//userlist := make(map[string]interface{});
	//userlist := make([]interface{}, 0, 1);
	
	fileList, e := ioutil.ReadDir(path); //这个的结果是按文件名排序的,所以生成文件名时最好有序
	if (e != nil) { return; }
	
	////userlist := make([]interface{}, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的
	//userlist := make([]map[string]string, 0, 1); //最后一个参数是初始容量大小，其实是可以加入超过它的数量的

	//userlist := make(map[string]interface{});
	
	
	for i, v := range fileList {
		
		//if (i < curPos) { continue; };//分页的跳过机制
		//if (i >= endPos) { continue; };//分页的跳过机制
		
		fmt.Println(i, "=", v.Name())
		
		// uname := v.Name();	
		//uname = MakeUserName(uname);

		fn := v.Name();
		
		kv := make(map[string]string);
		
		////kv[fn] = ToString(v.IsDir());
		kv["file_name"] = fn;
		kv["is_dir"] = ToString(v.IsDir());


		//fn 是相对路径，所以要算出完整全路径
		full_path := path + "/" + v.Name();
		kv["full_path"] = full_path;
		

		//----
		//目录的处理
		if (v.IsDir()) {

			//是否递归
			if (find_sub){

				File_FindFiles(full_path, out_list, find_sub, file_type);

			}//if 2

			if (file_type != "all" && file_type != "dir" ) { continue; }

		} else { //假如不是目录

			//if (file_type == "dir" ) { continue; }
			if (file_type != "all" && file_type != "file" ) { continue; }

		}
		//if 1

		
		
		//----
		
		//userlist[i] = kv;
		//fi = append(fi, f)
		//out_list = append(out_list, kv);

		//指针的使用其实和 c 语言一模一样，和 java 差别还是很大的，所以要用 c 的思想来思考 golang 而不是 java 的思想
		*out_list = append((*out_list), kv);

	
	}//for
	
	
	//return *out_list;
	
}//

//可指定处理函数的
func File_FindFiles_onfunc(path string, out_list * []map[string]string, find_sub bool, file_type string, on_func func(string)bool)  {

	fileList, e := ioutil.ReadDir(path); //这个的结果是按文件名排序的,所以生成文件名时最好有序
	if (e != nil) { return; }

	//----------------------------------------------------------------
	if (!on_func(path)) { return; }
	//----------------------------------------------------------------


	
	for i, v := range fileList {
		
		//if (i < curPos) { continue; };//分页的跳过机制
		//if (i >= endPos) { continue; };//分页的跳过机制
		
		fmt.Println(i, "=", v.Name())
		
		// uname := v.Name();	
		//uname = MakeUserName(uname);

		fn := v.Name();
		
		kv := make(map[string]string);
		
		////kv[fn] = ToString(v.IsDir());
		kv["file_name"] = fn;
		kv["is_dir"] = ToString(v.IsDir());


		//fn 是相对路径，所以要算出完整全路径
		full_path := path + "/" + v.Name();
		kv["full_path"] = full_path;
		

		//----
		//目录的处理
		if (v.IsDir()) {

			//是否递归
			if (find_sub){

				File_FindFiles(full_path, out_list, find_sub, file_type);

			}//if 2

			if (file_type != "all" && file_type != "dir" ) { continue; }

		} else { //假如不是目录

			//if (file_type == "dir" ) { continue; }
			if (file_type != "all" && file_type != "file" ) { continue; }

		}
		//if 1

		
		
		//----
		
		//userlist[i] = kv;
		//fi = append(fi, f)
		//out_list = append(out_list, kv);

		//指针的使用其实和 c 语言一模一样，和 java 差别还是很大的，所以要用 c 的思想来思考 golang 而不是 java 的思想
		*out_list = append((*out_list), kv);

	
	}//for
	
	
	//return *out_list;
	
}//

