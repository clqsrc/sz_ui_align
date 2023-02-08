

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

rem chrome 调试 ts 和 vscode 直接调试 nodejs 的 ts 都可以直接用这个脚本编译 ts 。生成的 sourceMap 直接生效配合 ts 调试。


