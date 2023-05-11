package main

import (
	"fmt"
//	"net/http"
//	"html/template"
//	"log"
	"strings"
//	"reflect"
//	"runtime"
//	"net"
//	"bufio"
//	"time"
//	"crypto/tls"
//	"crypto/rand"
	//"runtime/debug"
//	"encoding/base64"
//	"net"
	"io/ioutil"
	"os"
	"io"
	
)

//同 delphi TStringList

//字符串保存为文件
func SaveToFile(s string, fn string) (bool) {

	return String2File(s, fn);
}//

//参考 func WriteFile(filename string, data []byte, perm os.FileMode) error {
func AppendFile(filename string, data []byte, perm os.FileMode) error {
	
	//f, err := os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, perm)
	f, err := os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_APPEND, perm)
	
	if err != nil {
		return err
	}
	n, err := f.Write(data)
	if err == nil && n < len(data) {
		err = io.ErrShortWrite
	}
	if err1 := f.Close(); err == nil {
		err = err1
	}
	return err
}//

//字符串保存为文件,加在文件后面
func SaveToFile_append(s string, fn string) (bool) {
	
	var d1 = []byte(s);
	//err2 := ioutil.WriteFile("./output2.txt", d1, 0666)  //写入文件(字节数组)
	//check(err2)
	
	//err2 := ioutil.WriteFile(fn, d1, 0666);  //写入文件(字节数组)
	err2 := AppendFile(fn, d1, 0666);  //写入文件(字节数组)
	
	if err2 != nil {
		//panic(e)
		return false;
	}

	return true;
}//

func LoadFromFile(fn string) ([]byte) {
	data, err := ioutil.ReadFile(fn);
	if err != nil {
		fmt.Println("LoadFromFile() read error:", err)
		//os.Exit(1)
		
		return nil;
	}
	
	return data;
}//

//将文件内容拆分成字符串数组，类似于 delphi
//fn 是相对路径的文件名
//参考 LoadConfig 函数
func LoadFromFile_StringList(fn string) ([]string, error){
	
	defer PrintError("LoadFromFile_StringList()");
	
    //logger.Infof("get file content as lines: %v", filePath) 
	fmt.Println("读取文件:", fn);
	
	//var result = make([0]string);//map[string]string;
	//var result = [...]string{};
	var result = make([]string, 0); //这样居然可以
	
    b, err := ioutil.ReadFile(fn)  
    if err != nil {  
        //logger.Errorf("read file: %v error: %v", filePath, err)  
		fmt.Println("读取文件失败:", fn, err);
        return result, err  
    }  
    s := string(b); 
	lines := strings.Split(s, "\n");  
    //for _, lineStr := range strings.Split(s, "\n") { 
	i := 0;
    for _, lineStr := range lines { 
	 
        lineStr = strings.TrimSpace(lineStr);  
		
		lines[i] = lineStr;
		i++;
	}//for
	
	return lines, nil;
		
}//

//2023 不要 trim 的版本
func LoadFromFile_StringList_noTrim(fn string) ([]string, error){
	
	defer PrintError("LoadFromFile_StringList_noTrim()");
	
    //logger.Infof("get file content as lines: %v", filePath) 
	fmt.Println("读取文件:", fn);
	
	//var result = make([0]string);//map[string]string;
	//var result = [...]string{};
	var result = make([]string, 0); //这样居然可以
	
    b, err := ioutil.ReadFile(fn)  
    if err != nil {  
        //logger.Errorf("read file: %v error: %v", filePath, err)  
		fmt.Println("读取文件失败:", fn, err);
        return result, err  
    }  
    s := string(b); 
	lines := strings.Split(s, "\n");  
    //for _, lineStr := range strings.Split(s, "\n") { 
	i := 0;
    for _, lineStr := range lines { 
	 
        //lineStr = strings.TrimSpace(lineStr);  //2023 不要 trim

		//假如最后一个字符是 "\r" 则去掉
		if (len(lineStr)>0)&&('\r' == lineStr[len(lineStr)-1]){
			lineStr = string(lineStr[0:len(lineStr)-1]);
		}
		
		lines[i] = lineStr;
		i++;
	}//for
	
	return lines, nil;
		
}//

//数组是不能改变大小的，要换成切片//感觉切片就是 delphi 里的动态数组
//https://www.cnblogs.com/liuzhongchao/p/9159896.html
func StringList_Add(list []string, line string) ([]string){
	return append(list, line);
	
}//

