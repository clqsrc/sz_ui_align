const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    const exe = b.addExecutable("zig_t2", "src/main.zig");
    exe.setTarget(target);
    exe.setBuildMode(mode);

    //--------------------------------------------------------
    //clq c 语言的还要加上各种库
    exe.linkLibC();  //链接标准库 libc ，这样才能使用 stdio.h 中的函数

    //加入自定义的 c 源代码
    exe.addIncludeDir("src");
    //exe.addCSourceFile("src/cfuncs.c", &[_][]const u8{"-Wall"});
    //exe.addCSourceFile("src/cfuncs.c", &[_][]const u8{"-g"});  //改成这个的话，可以在 lldb 和 cppvsdbg 类型的 vscode 配置中进行调试
    exe.addCSourceFile("src/cfuncs.c", &[_][]const u8{"-g", "-Wall"});  //改成这个的话，可以在 lldb 和 cppvsdbg 类型的 vscode 配置中进行调试


    //编译命令就是 zig build 。所以实际上就是可以将 zig 项目当做 c++ 项目来写

    //--------------------------------------------------------

    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const exe_tests = b.addTest("src/main.zig");
    exe_tests.setTarget(target);
    exe_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
}
