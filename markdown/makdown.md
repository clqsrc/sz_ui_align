

# MD 文件在 vscode 中的简单教程


在 VSCode 下用 Markdown Preview Enhanced 愉快地写文档
//https://zhuanlan.zhihu.com/p/56699805


这些功能不是 MPE 的功能吧，是 Markdown-it 的功能，只要是 Markdown-it 作解析器，全都可以装这些语法扩展。


你说的对呀，mpe底层就是markdown-it。


draw.io
https://app.diagrams.net/

PlantUML 支持开发人员使用的各种图表，例如序列图、对象和类图等。
如果您已经熟悉 Draw.io，您会喜欢另一个扩展Draw.io Integration。

--------------------------------------------------------
https://github.com/stathissideris/ditaa

纯文本的图片。还可以圆角，可以加背景色

```ditaa{ cmd=true args=["-E"] hide=true }

    /--------+
    | c900   |
    |        |
    ++++++++++
    |        |
    |        |
    +--------/


    +--------+   +-------+    +-------+
    |        | --+ ditaa +--> |       |
    |  Text  |   +-------+    |diagram|
    |Document|   |!magic!|    |       |
    |     {d}|   |       |    |       |
    +---+----+   +-------+    +-------+
        :                         ^
        |       Lots of work      |
        +-------------------------+


```

--------------------------------------------------------

导出文件比导入文件还好理解：

首先在预览窗口里右键，选择 Open in Browser
在浏览器里打印为 PDF……
……完成