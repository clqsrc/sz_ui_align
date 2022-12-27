

没时间细写，大致是这样，其中 launch.json 其实要根据自己的情况更换为实际的类型，比如我的实际上要换成直接调试 html 的 chrome 类型。
这种情况下只要生成有对应的 fn.js.map 文件就可以在 chrome 调试 ts 了。原理未知：比如浏览器岂不是要去检查每个 js 都是否有 .map ?
不清楚，以后再研究，至少本地 html 这样就是可以了。所以没有专门的 ts 类型工程也是可以的。

1.

launch.json


```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "launch",
            "type": "node",
            "request": "launch",
            "program": "${workspaceRoot}/dist/main.js",
            "args": [],
            "cwd": "${workspaceRoot}",
            "protocol": "inspector",

            //"preLaunchTask": "echo",
            //"preLaunchTask": "tsc2",
            //"preLaunchTask": "tsc4",
            "preLaunchTask": "tsc3",
        }
    ]
}

```



2.

tasks.json

```
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "echo",
            "type": "shell",
            "command": "echo Hello"
        },

        {
            "label": "tsc3",
            "type": "shell",
            //"command": "echo Hello"

            //这样可以运行 bat ，不过如何传递参数呢
            //"command": "./1.bat"
            //这样传递参数
            //"command": "./1.bat ${workspaceRoot}"
            //${fileBasename}:当前文件的文件名
            "command": "./1.bat ${fileBasename} ${workspaceRoot}"
        },

        //--------------------------------------------------------
        {
            "label": "tsc4",
            "type": "cppbuild",  //没有装 gcc 扩展的话，这个类型会导致 本任务无效。通用的话最好还是用 "shell" 任务为好
            //"label": "tsc2", //"tsc 生成活动文件",
            "command": "gcc",


        },


    ]
}

```


3.
bat


```



echo 1.bat in run ...


set PATH=D:\no_install\node-v16.15.1-win-x64;D:\no_install\node-v16.15.1-win-x64\node_modules\.bin;%PATH%

rem %0 应该是文件自身的全路径
set p0=%0
set p1=%1
set p2=%2

echo %0
echo %p0%
echo %p1%
echo %p2%


rem tsc %p1% --allowJs


tsc %p1% --allowJs --sourceMap


```



