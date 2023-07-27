
package main
 
import (
	// "bufio"
	"fmt"
	// "io"
	"os"
) 

//二进制操作的函数，或者示例
// functions_file_bin.go >> functions_file_bin_app.go   

//functions_file_bin.go 的示例 

func FileBin_WriteTest1(fn string) (* os.File, error) {

	

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	//f, err := os.OpenFile(fn, os.O_RDWR, 0);
	// f, err := os.OpenFile(fn, os.O_RDWR|os.O_CREATE, 0666);  //0666 参数未测试，以后可能会更改
	f, err := function_FileOpen(fn);  //0666 参数未测试，以后可能会更改

	defer f.Close();

	//buf, err := function_FileReadBin(f, 1024 * 1024);
	buf, n, err := function_FileReadBuf(f, 1024 * 1024);

	fmt.Println("FileBin_WriteTest1()", n);

	function_FileSeek(f, 1000);

	f.WriteString("aaaaaa");
	function_FileWriteBin(f, buf);
	// function_FileWriteBin(f, buf);




	//参考
	// func Create(name string) (*File, error) {
	// 	return OpenFile(name, O_RDWR|O_CREATE|O_TRUNC, 0666)
	// }


	return f, err;
}//


