
# <center>js 项目转换为 typescript 项目的方法</center>

环境 VSCode


## 1.安装 nodejs

* 主要是用来运行 tsc 编译器。
* windows 下可以安装为绿色版本，然后用 

```
set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH%
```
设置路径。

* typescript 目前似乎只能在 nodejs 环境中才能运行，似乎没有独立的运行环境。


## 2.安装 typescript

好像要先安装 npm ，忘记了。 npm 是用来方便安装 nodejs 的工具罢了。类似于 ubuntu 的 apt 这样。所以实际上手动装理论上也是可以的。安装时文件有可能放在 nodejs 的安装目录下，也可能放在项目的目录下，前者称之为全局安装。一般情况下用全局吧，不过有些情况下必须装在本项目下。其实就是下载些 js 文件之类的。

## 3.安装好 typescript 后其实不用转换整个项目，用 tsc 命令转换单个文件更好控制

* tsc 默认比较严格，所以一般加上 --allowJs

```
tsc dom1.ts --allowJs
```

* 据说以下代码也有帮助

```
//TS编译之后自动加上严格模式，当然这个也可以通过配置去掉！
"use strict";
```

# 4. ts 兼容 lua

ts 原始就是用来转换成 js 再使用的，所以它的语法树兼容性很好，可完美的支持其他语言，比如 lua 。
方法是安装一个包，github 上就有。

```
//--------------------------------------------------------
//可以参考在线版 https://typescripttolua.github.io/play
//在线版转换出来的更简单清晰

//set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH%
//npx typescript-to-lua class1.ts
//tsc dom1.ts --allowJs
//npx typescript-to-lua dom1.ts --allowJs

//或者是发布是只复制 local function __TS__Class(self) 这样的函数过去就可以了

//浏览器的话要用 webpack 打包

//--------------------------------------------------------
```

所以，实际上我们的运行逻辑脚本全部就用 ts 写就行了，因为我们的环境就是 c/lus/js 非常完美地解决了跨平台的问题。


# 5.多文件引用的方法

据说最新的 ts 使用了 nodejs 推荐的 import 来引用多个文件（或是互相引用）。但这样的话每个文件独立成了一个模块，实际上与我们的本意不符合，我们的功能是要拆分成多个文件来实现的。有可能多个文件来实现一个大的模块，不便全部模块化。因此我们用传统的 ts 想用方法，即引用文件路径（reference path）。

实际上后面要说的所谓 “TypeScript 声明文件” 也是这种方法，这是官方引用已有 js 文件的方法。所以是很可靠的。

例如：

```
/// <reference path = "js.d.ts" />
/// <reference path = "jquery.ts/jquery/index.d.ts" />

```


# 6.jquery 的引入方法，以及没有用 ts 实现的函数、变量等如何使用？

这两者其实本质上是一样的，即如何使用原有的 js 代码（或者是不便改写为  ts 的代码）。

解决方法很简单，声明就可以了，类似于声明 c 语言的函数体这样的。

例如函数：

```
declare function CreateHashMap(): any;
```

注意，其中的参数类型在 ts 是严格区分的，所以对于已有的 js 类型 ts 统一叫它们为 any 类型。

另外，声明变量的方法和函数是不一样的，可以参考 https://www.jianshu.com/p/150f2893d643

```

declare let ui_design_prop_form: any;   //tsc 编译需要而已

// 类型 接口
interface Person {name: string;}

// 类型 类型别名
type Fruit = {size: number};

// 值 变量
declare let a: number;

// 值 函数
declare function log(message: string): void;

// 值 类
declare class Person {name: string;}

// 值 枚举
declare enum Color {Red, Green}

// 值 命名空间
declare namespace person {let name: string;}

```

而这些声明可以写在一种后缀名为 d.ts 的 ts 专有声明文件中。编译时是不会真的编译它们的。这些文件也是用引用路径的文件来引用的。

```

/// <reference path = "js.d.ts" />
/// <reference path = "jquery.ts/jquery/index.d.ts" />

```


上面其实展现了 jquery 的引入方法，这个 "jquery.ts/jquery/index.d.ts" 文件来自

https://www.runoob.com/typescript/ts-ambient.html
https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/jquery/index.d.ts

不过直接下载单个文件是不行的，甚至下载整个 
https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/jquery/

目录下的文件也还是不行，因为它还引用了另外一个包（幸好是比较小的包）。
考虑到可能还有其他包以后要用到，我是下载了这整个的包，大概有 50m 。全部放到项目中显然不可行，用 winrar 打开，将 jquery 开头的包全部拉出来就行了（即不用全部解压，文件太多了）。然后再拉出一个名为 sizzle 的包。

这时候用 tdc 编译一下包含了 jquery 的包，全提示 jquery 包的 index.d.ts 在引用 sizzle 时出错了。

这是因为它使用了 nodejs 的 import 方法，修改为引用路径的方法就可以了。

```

///// <reference types="sizzle" />
//clq 2022 要改成下面这个，引入 sizzle 包
/// <reference path="../sizzle/index.d.ts" />

```

