

rem //this file nodejs.bat

rem cmd /k "set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH% "


rem D:\no_install\node-v16.15.1-win-x64\node_modules\.bin   //tstl 也要设置目录。因为用 npx typescript-to-lua 的话会先去网络更新！！！


cmd /k "set PATH=D:\no_install\node-v16.15.1-win-x64;D:\no_install\node-v16.15.1-win-x64\node_modules\.bin;%PATH% "


