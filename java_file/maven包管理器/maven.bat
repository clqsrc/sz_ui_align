

set JAVA_HOME=D:\Java\jdk1.8.0_131

set PATH=D:\Java\jdk1.8.0_131\bin;F:\no_install\maven\apache-maven-3.8.6\bin;%PATH%



rem --------------------------------------------------------
rem 上面的 1.8 sdk 不行了，最新的 maven 要用 jdk 9  //maven 下载的 jar 文件这些到哪去了呢？ //C:/Users/ccc/.m2/repository/

set JAVA_HOME=D:\Java\jdk9.0.4

set PATH=D:\Java\jdk9.0.4\bin;F:\no_install\maven\apache-maven-3.8.6\bin;%PATH%

rem --------------------------------------------------------
rem jsweet 需要 jdk 11

set JAVA_HOME=D:\Java\jdk-11.0.15

set PATH=D:\Java\jdk-11.0.15\bin;F:\no_install\maven\apache-maven-3.8.6\bin;%PATH%

cd /d d:

rem cmd /k "dir"

rem 可以先在上面执行完要执行的内容，然后用 cmd /k 停留在命令行中而不退出

cmd /k "echo %PATH%"

