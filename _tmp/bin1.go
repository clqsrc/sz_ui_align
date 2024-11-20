
package main

import (
	"fmt"
	"io"
	"os"
)

func main() {

	fmt.Println("aaa");

	// 打开文件
	//file, err := os.Open("example.bin");
	// 打开或创建文件
	file, err := os.OpenFile("example.bin", os.O_RDWR|os.O_CREATE, 0644);

	if err != nil {
		fmt.Println("打开文件失败:", err);
		return
	}
	defer file.Close()

	// 设置偏移量
	offset := int64(1024); // 偏移量，例如从文件的第1024字节开始
	// 设置读取长度
	length := 256 // 读取长度，例如读取256字节

	// 定位到文件偏移量
	_, err = file.Seek(offset, io.SeekStart);
	if err != nil {
		fmt.Println("定位文件失败:", err);
		return
	}

	// 创建一个缓冲区来存储读取的数据
	buffer := make([]byte, length);

	// 从文件中读取数据到缓冲区
	n, err := file.Read(buffer)
	if err != nil && err != io.EOF {
		fmt.Println("读取文件失败:", err);
		return
	}

	fmt.Printf("读取了 %d 字节的数据\n", n);
	fmt.Printf("读取的数据: %v\n", buffer[:n]);

	//---------------------------------------------------
	// 要写入的数据
	data := []byte{0x01, 0x02, 0x03, 0x04, 0x05};

	// 写入数据到文件
	n, err = file.Write(data);
	if err != nil {
		fmt.Println("写入文件失败:", err)
		return;
	}

	fmt.Printf("写入 %d 字节的数据\n", n);

}//



