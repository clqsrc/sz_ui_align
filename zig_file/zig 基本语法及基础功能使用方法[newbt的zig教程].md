zig 基本语法及基础功能使用方法[newbt的zig教程]

这其实是一个 md 文档，在 github 中同步更新。地址为

https://github.com/clqsrc/sz_ui_align/blob/main/zig_file/zig%20%E5%9F%BA%E6%9C%AC%E8%AF%AD%E6%B3%95%E5%8F%8A%E5%9F%BA%E7%A1%80%E5%8A%9F%E8%83%BD%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95%5Bnewbt%E7%9A%84zig%E6%95%99%E7%A8%8B%5D.md

本文章原始地址为
http://newbt.net/ms/vdisk/show_bbs.php?id=9573656170788494499256AB77F15A09&pid=164

--------------------------------------------------------

zig 很小众。主要是语法变化目前还比较大（其实感觉上是标准库的位置和函数变化比较大）。
但用于某些环境中平替 c 语言是非常好的，特别是像 c++ 一下可以直接内嵌纯 c 语言，而不需要配置一个 c/c++ 编译器是我最接受它的原因。

不过目前已知的它并不能取代所有的 c/c++ 编译器，我所知道的 msys2 环境下编译 gtk 无法平替，其他的环境下基本上是完美。

1.
平替 c 语言编译器的方法

很简单，将编译脚本中的 gcc 替换成 zig cc 即可。同理 g++ 替换成 zig c++ 即可。
这一知识的获取方法是 zig 的命令行帮助，命令好像是 zig -h 不太记得了。

1.1
推荐的其他中文 zig 教程：

1.1.1
https://zhuanlan.zhihu.com/p/459281215

yalight
&#8203;
东北大学秦皇岛分校 教师

这上面有的内容我基本上只转发到其他区，就不贴在这里了。


1.2
官方文档

https://ziglang.org/documentation/0.10.0/

目前实际上好像就是一个页面而已，其实内容也比较全。




2.
其他语言中常用功能、特性在 zig 中的实现方式。比较强制类型转换，还是很特别的。

2.1
强制类型转换

https://ziglang.org/documentation/0.10.0/#intCast

2.1.1
转换成 c 语言的整数类型。这在调用 c 函数时经常用，居然没看到有人说到。

```
    var b4 = args.len;
    //var b5 = @as(c_int, b4);  //这个其实不是强制类型转换

    //var i2 = @as(c_int, args.len); //奇怪，这句就不对

    //https://ziglang.org/documentation/0.10.0/#Casting
    //test "cast *[1][*]const u8 to [*]const ?[*]const u8" {
    //     const window_name = [1][*]const u8{"window name"};
    //     const x: [*]const ?[*]const u8 = &window_name;
    //     try expect(mem.eql(u8, std.mem.sliceTo(@ptrCast([*:0]const u8, x[0].?), 0), "window name"));
    // }

    //var b5 = @ptrCast(c_int, b4);
    var b5 = @intCast(c_int, b4);
```

我是看到了有 @ptrCast 的代码，然后自己试了好几种写法，果然有 @intCast 。
这在 zig 中并不是关键字，而是
https://ziglang.org/documentation/0.10.0/#Builtin-Functions

不知道有没有更好的更直接的强制转换方式。

```
Builtin Functions
Builtin functions are provided by the compiler and are prefixed with @. The comptime keyword on a parameter means that the parameter must be known at compile time.



内置功能

内置函数由编译器提供，前缀为@。参数上的comptime关键字表示该参数必须在编译时已知。 
```

2.1.2
怎样转换出 c 语言常见的指针的指针。或者说是字符串数组。

```
void function(const char **pNames) {
    *pNames = NULL;
}

```

官方文档也是没有的，感觉 zig 既然是以兼容 c 为最大卖点，接口的写法却着实难过。

最后的的解决办法直接来自于它的创始人作者。
https://github.com/ziglang/zig/issues/9479

```
test "example" {
    var c_ptr: [*c][*c]const u8 = undefined;
    var zig_ptr: [*][*]const u8 = undefined;

    c_ptr = zig_ptr;
}
```

另外构建这样的字符串数组，还要借助 zig 中的数组类。

```

    const allocator2 = std.heap.c_allocator;
    var extensions = std.ArrayList([*]const u8).init(allocator2);
    errdefer extensions.deinit();

    for (args) |arg, n| {
        //warn("arg{}: {}\n", n, arg);
        //std.log.info("arg{}: {}\n", n);
        //std.log.info("arg{}: {}\n", arg);

        std.log.info("numAddTen:{d}\n",.{n});
        std.log.info("numAddTen:{s}\n",.{arg});

        //var c_arg :[*]const u8 = arg;

        //try extensions.append(arg);  //expected type '[*]const u8', found '[:0]u8'
        //try extensions.append(c_arg);

        //这个强制转换也很关键
        try extensions.append(@ptrCast([*]const u8, &arg[0]));

        //extensions.appendSlice(arg);
        
    }//


    var c_argv = extensions.items.ptr;

    c_ptr = @ptrCast([*c][*c]u8, c_argv); 

    //最后这个 c_ptr 才是对应到的 char ** argv 参数类型
    _ = c._lua_main(c_argc, c_ptr);


```

2.1.3

根据上面的例子就有会经常用到的  '[:0]u8' 转换为 '[*]const u8' 的方法

```
@ptrCast([*]const u8, &arg[0]));

```

不知道正宗的 zig 写法是什么，我这里是直接用了 delphi 中常用的取一个字符串原始地址的方法。
即取出一个字符串中第一个字符的地址就是整个内存块的起始地址。  注意并不是直接取字符串的地址。

这个方法成功后我也松了一口气，这样看来 zig 的内存低级操作还是有迹可循的，并没有太过份的语法糖魔术。



3.
zig 自己的特性

和 c/c++/java 等传统语言不同的放这里介绍。其实有相当一部分新兴语言是有的，所以说是相当于是 zig 借鉴新兴语言的部分也可以。

3.1
defer

很好用的特性，和 golang 中的一模一样。就是在函数结束后要执行的清理工作代码。


3.2
与 c 语言字符串的接口。

3.2.1
导出函数给 c 语言用时。

直接看官方文档中的如下示例。这也是 zig 中访问 c 语言风格字符串的方法(c 语言字符串转换为 zig 的缓存片段类型)。


```
混合对象文件
您可以将 Zig 对象文件与遵循 C ABI 的任何其他对象文件混合使用。例：

base64.zig
const base64 = @import("std").base64;

export fn decode_base_64(
 dest_ptr: [*]u8,
 dest_len: usize,
 source_ptr: [*]const u8,
 source_len: usize,
) usize {
 const src = source_ptr[0..source_len];
 const dest = dest_ptr[0..dest_len];
 const base64_decoder = base64.standard.Decoder;
 const decoded_size = base64_decoder.calcSizeForSlice(src) catch unreachable;
 base64_decoder.decode(dest[0..decoded_size], src) catch unreachable;
 return decoded_size;
}

```


999. 
zig 中的内存分配器

其实如果用过我们的 lstring 库就知道为什么要有个内存分配器。其实说白了就是一个方便批量释放内存的调用，就是你操作分配了一大堆内存后不记得每个内存块要在哪释放，不用操心。
在某个要退出操作的时候调用一个函数一次性释放就完了。

国内的 zig 教程也还是比较多的，不过似乎都没说到这个。所以重点提一下。

内存分配器可以自己创建也可以使用系统默认带有的，全部方式我也不太懂，目前已知的方式如下。


```
//--------------------------------------------------------
    //zig 取命令行参数的方法 //好像 golang 也是这样取的
    //const args = try std.os.argsAlloc(allocator); 
    //defer std.os.argsFree(allocator, args);

    //似乎已经过时了，在代码中 D:\no_install\zig-x86_64-0.10\lib\std\os.zig 中提示
    // Populated by startup code before main().
    // Not available on WASI or Windows without libc. See `std.process.argsAlloc`
    // or `std.process.argsWithAllocator` for a cross-platform alternative.

    //下面这段内存分配器的代码来自 D:\no_install\zig-x86_64-0.10\lib\std\testing.zig
    //而实际上 zig 是有默认的内存分配器的，而且似乎不止一个
    //const allocator = std.mem.Allocator.allocator();
    var b = std.heap.GeneralPurposeAllocator(.{}){};
    var allocator_instance = b;
    const allocator1 = allocator_instance.allocator();

    std.log.info("allocator1:{}\n",.{allocator1});

    //这个默认的内存分配器示例来自 https://github.com/MasterQ32/zig-args/blob/master/demo.zig
    var allocator = std.heap.page_allocator;

    const args = try std.process.argsAlloc(allocator);
    //defer std.os.argsFree(allocator, args);

    defer allocator.free(args); //类似于 golang 的函数结束时的释放语句


    //warn("total args: {}\n", args.len);

    for (args) |arg, n| {
        //warn("arg{}: {}\n", n, arg);
        //std.log.info("arg{}: {}\n", n);
        //std.log.info("arg{}: {}\n", arg);

        std.log.info("numAddTen:{d}\n",.{n});
        std.log.info("numAddTen:{s}\n",.{arg});
        
    }//

    std.log.info("命令行参数个数:{d}\n",.{args.len});

    //--------------------------------------------------------
```











