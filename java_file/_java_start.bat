

rem 编译和运行 java

set JAVA_HOME=D:\Java\jdk9.0.4
set PATH=D:\Java\jdk9.0.4\bin

cd /d D:\gs_d\lg\test2\java_exe_t3\java_src

rem -d 是指明生成的路径
rem javac -d D:\gs_d\lg\test2\java_exe_t3\java_src\out

rem javac -encoding utf-8 -d D:\gs_d\lg\test2\java_exe_t3\java_src\out
rem javac -encoding utf-8 -d D:\gs_d\lg\test2\java_exe_t3\java_src\out http_main.java

rem javac -encoding utf-8 -d D:\gs_d\lg\test2\java_exe_t3\java_src\out http_main.java -cp D:\gs_d\lg\test2\java_exe_t3\java_src\lib\gson-2.8.5.jar

rem javac -cp ./lib/skija-windows-0.93.6.jar  -cp ./lib/skija-shared-0.93.6.jar --source-path D:\gs_d\lg\test2\java_exe_t3\java_src -encoding utf-8 -d D:\gs_d\lg\test2\java_exe_t3\java_src\out http_main.java -cp D:\gs_d\lg\test2\java_exe_t3\java_src\lib\gson-2.8.5.jar

rem 多个.jar包或者目录Mac用:分开，Wind用;分开；

rem //ok
rem javac -cp ./lib/skija-windows-0.93.6.jar;./lib/skija-shared-0.93.6.jar;D:\gs_d\lg\test2\java_exe_t3\java_src\lib\gson-2.8.5.jar --source-path D:\gs_d\lg\test2\java_exe_t3\java_src -encoding utf-8 -d D:\gs_d\lg\test2\java_exe_t3\java_src\out http_main.java

rem //下面的也是 ok 的
javac -cp ./lib/skija-windows-0.93.6.jar;./lib/skija-shared-0.93.6.jar;./lib/gson-2.8.5.jar --source-path ./ -encoding utf-8 -d ./out http_main.java

rem //下面是运行
rem java -cp ./lib/skija-windows-0.93.6.jar;./lib/skija-shared-0.93.6.jar;./lib/gson-2.8.5.jar --source-path ./ -encoding utf-8 -d ./out http_main

rem java --class-path ./out -cp ./lib/skija-windows-0.93.6.jar;./lib/skija-shared-0.93.6.jar;./lib/gson-2.8.5.jar http_main
java -cp ./out;./lib/skija-windows-0.93.6.jar;./lib/skija-shared-0.93.6.jar;./lib/gson-2.8.5.jar http_main
