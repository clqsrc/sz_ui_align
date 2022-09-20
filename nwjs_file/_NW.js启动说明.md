

1.

https://nwjs.io/

下载 sdk 版本。
这个和 ele 是不同的，ele 是在 node.js 中使用 npm 安装。而 nwjs 自带 node.js


2.
可以直接运行目录中的 nw.exe

3.
可以把 html 拖放进去。

4.
这样也可以
nw D:\test1\nodejs1\nw1\nwt1

--------------------------------------------------------
http://t.zoukankan.com/lingnweb-p-9945027.html
Package.json可以理解为应用的配置文件，为必须；index.html作为应用的入口文件，内容和一般网页类似，名字可以按自己喜好起，但是一定要和配置中的“main”参数设置一致；如果要引用node模块，可以在目录结构中增加一个node_modules的目录，以存放APP所需的node模块。

    title : 字符串，设置默认 title。
    width/height : 主窗口的大小。
    toolbar : bool 值。是否显示导航栏。
    icon : 窗口的 icon。
    position :字符串。窗口打开时的位置，可以设置为“null”、“center”或者“mouse”。
    min_width/min_height : 窗口的最小值。
    max_width/max_height : 窗口显示的最大值。
    resizable : bool 值。是否允许调整窗口大小。
    always-on-top : bool 值。窗口置顶。
    fullscreen : bool 值。是否全屏显示。
    show_in_taskbar : 是否在任务栏显示图标。
    frame : bool 值。如果设置为 false，程序将无边框显示。
    "chromium-args" :"-allow-file-access-from-files" 相当于给谷歌浏览器添加启动参数一样，这行代码允许angularjs直接访问本地json文件。


--------------------------------------------------------

https://blog.csdn.net/qq_42376361/article/details/111591551
vscode 中增加 node.js 提示

npm install --save-dev @types/node 

是否成功要看当前目录是否下载了 node 的模块文件。有可能要换用以下命令。另外据说没有 jsconfig.json 文件的话一定要手工建立一个。
npm install --save-dev @types/node --location=local
npm install --save-dev @types/node --local





