
# <center>生成 lua 代码时要注意</center>

[map_main.ts 有原始信息]

目前的生成参数为

```
npx typescript-to-lua map_main.ts --allowJs  --noImplicitSelf true
```

如果是 
npx typescript-to-lua map_main.ts --allowJs 
的话，会生成很多带 "G," 的代码。
通过 npx typescript-to-lua --help 命令查看帮助，可知

```
 --noImplicitSelf <boolean>                      If "this" is implicitly considered an any type, do not generate a self parameter.

```

不过具体可能还是要看更详细的帮助。
官网 https://typescripttolua.github.io/docs/the-self-parameter 有一点

https://blog.51cto.com/rongfengliang/5269356
@noResolution 参数
使用此参数可以让TypeScriptToLua 不去解析引用的模块 

//----
typescript-to-lua 会象 tsc 一样将关联的 ts 文件一并编译。
暂时未找到关闭的方法，所以这样就是原 lua 文件被覆盖的风险，所以还是不要在运行环境中生成。
到单独目录中生成好后再复制过去好了。

//----
另外一个很重要的一点: 成员函数中不用加 self 参数，生成 lua 时会自动加上的。
实际上所有的 this 都会转换成 self

//----
//tstl ui_page_car_main_ts.ts --allowJs  --noImplicitSelf true  //这个会说 "true" 有问题
// rem cmd /k "set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH% "
// rem D:\no_install\node-v16.15.1-win-x64\node_modules\.bin   //tstl 也要设置目录。因为用 npx typescript-to-lua 的话会先去网络更新！！！
// cmd /k "set PATH=D:\no_install\node-v16.15.1-win-x64;D:\no_install\node-v16.15.1-win-x64\node_modules\.bin;%PATH% "
//以后再研究 tstl 怎么传递 true boolean 参数进去，现在先用以下命令，似乎也差不多
//tstl ui_page_car_main_ts.ts --allowJs  --noImplicitSelf