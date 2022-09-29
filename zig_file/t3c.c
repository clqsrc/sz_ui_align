

#include <stdio.h>

#include "t3c-2.h"
//#include "t3c-2.c"

static int f1()
{

    f2();
    return 1;
}

struct c1{

    int t1;
    int t2;

};

struct c1 t1 = { 0, 1 };

int main()
{

    f2();

    t1.t1;

    printf("hello.\r\n");

    return 0;

}//

//zig cc -save-temps t3c.c  //生成中间代码

//zig cc  t3c.c t3c-2.c -o t3.exe  //生成 exe ，可以看到确实是限制了 static 函数的访问

//据说早期 cfront 可将 c++ 翻译为 c 代码



//zig 在使用 musl 的情况下可以生成不依赖任何库的纯静态文件，类似于 golang 这样大大有利于部署
//这也可以给 rust 用，参考 https://zhuanlan.zhihu.com/p/376471249
/*
“
Rust 在 x86_64-unknown-linux-gnu 目标下默认会动态链接到系统 C 运行时[1]，而不同发行版之间的 libc 可能会有兼容性问题。
如果想要把一次编译好的可执行文件放到不同的 Linux 发行版上面去跑，最好采用 x86_64-unknown-linux-musl 目标进行静态编译。
”

据说 https://github.com/vlang 能将 vlang 转换为 c 语言 //https://github.com/vlang/vc
https://vlang.io/ 上直接有例子
v translate file.c

另外还有一个例子说到 zig 的 musl
https://www.zhihu.com/question/469214263/answer/1972506636?utm_id=0
“
====补充下其他回答不太涉及的C代码交叉编译的分割线====
从交叉编译的角度讲，比较常用的gcc/clang并不好用。这里推荐一个C/C++语言的编译器zig
zig本身是一个编程语言，但是它同时可以做为c/c++的编译器使用，设计上非常注重交叉编译的支持，
开箱自带各个平台/架构的标准库(libc)，不需要安装复杂的工具链

比如使用命令 zig cc -o hello hello.c -target riscv64-linux-musl 就可以交叉编译到riscv架构，并使用musl而不是glic (使用musl可以完全静态链接), 但是用gcc / clang是做不到的。。

zig的作者发过一个使用zig来轻松交叉编译LuaJIT项目的例子，可以参考
https://andrewkelley.me/post/zig-cc-powerful-drop-in-replacement-gcc-clang.html

交叉编译还有一个痛点是在非C的语言中通过C ABI调用C的代码，比如GO里面使用CGO / rust里面使用C代码。
使用clang / gcc的工具链通常交叉编译都很麻烦，使用zig cc通常能更容易的解决。
这里有个例子，有位老哥想把rust库wasmtime交叉编译到aarch64-macos  但他手上应该没有用M2的新macbook pro
rust的部分倒好说，rust本身的交叉编译支持非常棒。但是依赖的C代码就搞不定了，找不到现成的办法，他后来用zig cc做linker，竟然成功了，然后兴奋了发了下面的帖子
https://actually.fyi/posts/zig-makes-rust-cross-compilation-just-work/

”

*/

//--------------------------------------------------------
//c 语言尽量避免函数重名的方法，在 gcc 中是有一些的。
/*

1.
// https://blog.csdn.net/giveaname/article/details/103353828?
可以用gcc编译器的特性来设置动态库函数的导出控制。
可在函数前增加__attribute__ 前缀来控制

#define DLL_PUBLIC __attribute__((visibility("default")))
#define DLL_LOCAL  __attribute__((visibility("hidden")))


编译时，增加-fvisibility=hidden 参数，则未增加前缀的函数都不会导出

2.
应该和上面的一样
https://blog.csdn.net/qq_37887537/article/details/89472705


C 多个动态库存在同名函数问题处理方法：-fvisibility=hidden

在使用多个动态库时，两个动态库之间有可能存在相同名称的函数，由于动态链接器在程序加载时会对外部引用进行重定位，这样会出现只有第一个函数生效，即所有对该函数的调用都将指向第一个加载的动态库的同名函数中。这样就会很混乱，而且在想改名称也不是很简单的情况下就会很麻烦（例如同名函数太多，甚至就是模板）等等。
处理方法

这里只找到一种处理方法，当然你直接把名字改成不一样或者统一添加前缀（例如C++ namespace）也不错。方法如下：

-fvisibility=hidden

将以上的写入gcc/g++中去，从.o的生成到.so的生成的命令行都要填上。这个有什么作用呢？GUN规定动态库中的所有函数的全局变量默认是对外可见的，如果编译动态库时不添加以上参数，默认是可见的，添加之后，如果调用动态库的函数，由于无法定义到引用的实际实现，会报错：
undefined symbol: XXX
所以还没有结束，接着就是要把动态库部分需要对外显示的函数接口显示出来：例如模块的入口函数：
__attribute__((visibility("default"))) int app_main(void)
注
__attribute__作用:
__attribute__ 可以设置函数属性（Function Attribute）、变量属性（Variable Attribute）和类型属性（Type Attribute）
格式：__attribute__（（(visibility("default")，，，，，））
当然大家可以看明白，同名的不是可执行文件要调用的函数，而是动态库中其他函数要使用的。

类似案例：
内核态导出的parameter找不到，不添加hidden又不能按照模板进行加载多个类似的，只能在需要导出的parameter上添加default属性，让该参数可以刷新出来：


3.应该是比较通用的加 static 限制在本文件中

https://blog.csdn.net/weixin_42031299/article/details/115761184

1.在源文件中，不对外提供的函数，定义时都用static修饰，将该函数的链接属性限定在本文件；
2.函数命名时，在每个函数的名字前都加上库的名称，一般库的名字是唯一且不重合的；
3.全局变量如果只在本文件中使用，最好也使用static修饰；

不过假如是  include 的多个 c 文件情况也是没有用的，这时候似乎只能上 c++ ?

原理可以看下 http://blog.chinaunix.net/uid-28994163-id-4333705.html



*/


