
vscode 下调试 lua

原来有一个的，在 macos 下，还挺好用。不过已经忘记了，再做一个 windows 下的吧。


https://www.jianshu.com/p/61152eeee4cc/

这里有个比较复杂的，其实这是在 xlua slua 中调试内置 lua 的方法。

如果只是本机的 lua 其实直接装一个插件就可以了，它自带了 lua 的 exe 。

```
“
Lua Debug
v1.61.0
actboy168
347,640
(9)
VSCode debugger extension for Lua
”
```

不过叫 "Lua Debug" 比较多，要看清楚了。另外一个插件 emmylua 安装后就可以有语法高亮这些了，不过似乎没那么方便调试。

原始地址为

https://marketplace.visualstudio.com/items?itemName=actboy168.lua-debug

它调试时产生的命令行，类似如下：

================================================================
```
Windows PowerShell
版权所有 (C) Microsoft Corporation。保留所有权利。


PS F:\test1\lua1\teal_t1>  & 'c:/Users/ccc/.vscode/extensions/actboy168.lua-debug-1.61.0-win32-x64/runtime/win32-x64/lua54/lua.exe' '-e' 'dofile[[c:/Users/ccc/.vscode/extensions/actboy168.lua-debug-1.61.0-win32-x64/script/launch.lua]];DBG[[17240-ansi]]' 'F:\test1\lua1\teal_t1/t1.lua'
aaa
PS F:\test1\lua1\teal_t1>  f:; cd 'f:\test1\lua1\teal_t1'; & 'c:/Users/ccc/.vscode/extensions/actboy168.lua-debug-1.61.0-win32-x64/runtime/win32-x64/lua54/lua.exe' '-e' 'dofile[[c:/Users/ccc/.vscode/extensions/actboy168.lua-debug-1.61.0-win32-x64/script/launch.lua]];DBG[[4372-ansi]]' 'F:\test1\lua1\teal_t1/t1.lua' 
aaattt
C:\Users\ccc\.vscode\extensions\actboy168.lua-debug-1.61.0-win32-x64\runtime\win32-x64\lua54\lua.exe: F:\test1\lua1\teal_t1/t1.lua:5: interrupted!
stack traceback:
        [C]: in function 'print'
        F:\test1\lua1\teal_t1/t1.lua:5: in main chunk
        [C]: in ?
PS F:\test1\lua1\teal_t1>

```

================================================================




