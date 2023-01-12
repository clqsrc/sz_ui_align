zig 基本语法及基础功能使用方法[newbt的zig教程]

这其实是一个 md 文档，在 github 中同步更新。

本文章原始地址为
http://newbt.net/ms/vdisk/show_bbs.php?id=9573656170788494499256AB77F15A09&pid=164

--------------------------------------------------------

zig 很小众。主要是语法变化目前还比较大（其实感觉上是标准库的位置和函数变化比较大）。
但用于某些环境中平替 c 语言是非常好的，特别是像 c++ 一下可以直接内嵌纯 c 语言，而不需要配置一个 c/c++ 编译器是我最接受它的原因。

不过目前已知的它并不能取代所有的 c/c++ 编译器，我所知道的 msys2 环境下编译 gtk 无法平替，其他的环境下基本上是完美。

1.
平替 c 语言编译器的方法

很简单，将编译脚本中的 gcc 替换成 zig cc 即可。同理 g++ 替换成 zig c++ 即可。
这一知道的获取方法是 zig 的命令行帮助，命令好像是 zig -h 不太记得了。

2. 
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











