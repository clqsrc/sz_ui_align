


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





