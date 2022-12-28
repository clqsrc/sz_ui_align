use std::fmt;

fn main() {
    println!("Hello, world!");

    f1();
}


//安装两个插件后即可调试，在 rs 文件中会出现 run 和 debug 按钮
//安装了 rust-analyzer 后就可以运行了，而 CodeLLDB 安装后才可调试。
//CodeLLDB 插件安装后还要下载一些东西，有可能失败。这时可手动下载
//https://github.com/vadimcn/vscode-lldb/releases/download/v1.8.1/codelldb-x86_64-windows.vsix
//这个是 vscode 的离线插件格式，可以直接在插件中选择手工安装就可以了

fn f1()
{
    let i = 10;

    println!("Hello, world!{}", i);
    
}


//编译命令
//cargo run --package rt1 --bin rt1 

