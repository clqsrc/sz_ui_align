


package main

import (
	//"io/ioutil"
	//"os"
	//	"database/sql"
	"fmt"
	//"io"
	"net/http"
	//"sync"
	//"time"
	//	"net/mail"
	//"html/template" //模板
	//	"reflect"
	//	"strconv"
	//	"strings"
	"encoding/json"
	//"sort"
	// "os/exec"
	// "io/ioutil"
	// "log"
	// "os"
	//"github.com/ying32/govcl/vcl"
	"code.google.com/p/mahonia" //目前没有官方 utf8 转换方法
//	"golang.org/x/net/html/charset" //官方库下载不到,要用 https://github.com/golang/net 的	
	"strconv"
	"html/template" //模板
	"bytes"
)

// 暂时可能不用的函数

//-----------------------------------------------


//用系统的 "打开"  打开一个文件
//应该是没有通用的, 要每个操作系统做一个
func ShellOpen_OS(fn string){

	//os.Open(fn); //这个函数应该无关的，应该是打开读写文件的意思 

	cmd_ := fn;
	c := exec.Command("cmd", "/C", cmd_);   // windows //ok 一定要加上 "/C" 参数
	//c := exec.Command("bash", "-c", cmd);  // mac or linux	

	go c.Start();
	
	return;

	cmd := exec.Command(fn);		
	//cmd := exec.Command("open", fn);	//no	
	//cmd := exec.Command("cmd", fn);	//no	 

	//cmd.Dir = "D:\\new\\Microsoft VS Code";
	cmd.Dir = ExtractFilePath(fn);

	cmd.Start();

	s := GetCmdOut(cmd);

	fmt.Println("ShellOpen_OS()", s);

}//



/*
//取一个 div 对应的 icon 的文件名
func (f *TDesktopForm) GetIconFullpath(sender vcl.IObject) string {

	div := sender;

	////div_ := (vcl.IObject)(div);

	////div__ := div_.(*vcl.TPanel);  //奇怪，这样转换是错误的

	//https://gitee.com/ying32/govcl/wikis/pages?sort_id=2321552&doc_id=102420
	//参考以上资料，要用它封装的函数
	div__ := vcl.AsPanel(div);
	fmt.Println("OnClick_PopMenuItem_IconFile_ShowInfo()", div__);

	var i int64 = int64(div__.Handle()); //govcl 的 Handle() 返回值是 uintptr ，在权威 《Go程序设计语言》中说了，uintptr 的返回值要尽快转换成 unsafe.Pointer
	//因为 uintptr 指向的指针值因为 gc 的原因是有可能改变的。也就是说理论上 uintptr 应该只是作为临时变量，不能当做长期使用的成员变量这样的

	fmt.Println("OnClick_PopMenuItem_IconFile_ShowInfo()", i);

	tmphandle := div__.Handle();
	fn := mm[uint64(tmphandle)].(string);
	
	return fn;
}//

*/






