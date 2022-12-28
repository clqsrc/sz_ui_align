

菜鸟教程和其他一些网上的教程都太过复杂了，目前（2022）是可以有个简单的方法的


//安装两个插件后即可调试，在 rs 文件中会出现 run 和 debug 按钮
//安装了 rust-analyzer 后就可以运行了，而 CodeLLDB 安装后才可调试。
//CodeLLDB 插件安装后还要下载一些东西，有可能失败。这时可手动下载
//https://github.com/vadimcn/vscode-lldb/releases/download/v1.8.1/codelldb-x86_64-windows.vsix
//这个是 vscode 的离线插件格式，可以直接在插件中选择手工安装就可以了

有可能要设置路径，比如我的 settings.json


```

{
    "workbench.colorCustomizations": {
    
    },

    "terminal.integrated.env.windows": {

        //"GOROOT":"D:/go1.18.3.windows-amd64/go",
        "PATH":"C:/Users/ccc/.cargo/bin",
    },

    "rust-analyzer.runnableEnv": [

        {
            //"mask":null,
            "env": {
            //     "APP_ID": "2", // overwrites only APP_ID
            "PATH":"C:/Users/ccc/.cargo/bin",
            }
        },

        {
            "mask": "test_name",
            "env": {
            //     "APP_ID": "2", // overwrites only APP_ID
            "PATH":"C:/Users/ccc/.cargo/bin",
            }
        }
    ]
}


```