
package main
 
import (
	// "bufio"
	// "fmt"
	"io"
	"os"
) 

// functions_file_bin.go   //二进制操作的函数，或者示例

func function_FileOpen(fn string) (* os.File, error) {

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	//f, err := os.OpenFile(fn, os.O_RDWR, 0);
	f, err := os.OpenFile(fn, os.O_RDWR|os.O_CREATE, 0666);  //0666 参数未测试，以后可能会更改

	//参考
	// func Create(name string) (*File, error) {
	// 	return OpenFile(name, O_RDWR|O_CREATE|O_TRUNC, 0666)
	// }


	return f, err;
}//

//seek
func function_FileSeek(f * os.File, offset int64) (ret int64, err error) {

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	ret, err = f.Seek(offset, io.SeekStart);


	return ret, err;
}//

//写入一块内存
//返回值是实际的写入字节数
func function_FileWriteBin(f * os.File, buf []byte) (ret int, err error) {

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	//ret, err = f.Seek(offset, io.SeekStart);
	ret, err = f.Write(buf);


	return ret, err;
}//


//读取一块内存
//返回值是实际读取的字节数
func function_FileReadBin(f * os.File, buf_len int64) (ret int, err error) {

	buf := make([]byte, buf_len);

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	//ret, err = f.Seek(offset, io.SeekStart);
	//f.Write(buf);
	ret, err = f.Read(buf);


	return ret, err;
}//

func function_FileReadBuf(f * os.File, buf_len int64) (buf []byte, ret int, err error) {

	//buf := make([]byte, buf_len);
	buf = make([]byte, buf_len);

	//读写模式，并且可创建
	//f, err := os.OpenFile(fn, os.O_RDONLY, 0);
	//ret, err = f.Seek(offset, io.SeekStart);
	//f.Write(buf);
	ret, err = f.Read(buf);


	return buf, ret, err;
}//



