


rem go install fyne.io/fyne/v2/cmd/fyne_demo@latest
rem fyne_demo

rem --------------------------------------------------------

rem # native compiler windows amd64

rem #GOROOT=c:\go
rem #GOROOT=D:\go1.10.8.windows-amd64\go
set GOROOT=D:\go1.18.3.windows-amd64\go

rem # 这个参数很重要，不是跟 go 版本一致，而是和代码所在的路径一致！ 否则是会编译失败的
rem #GOPATH=D:\gopath1.10.8

# 这个是全部 go 18 特性都打开的版本，主要用来测试网上下载的代码，所以路径还是设置成标准的
set GOPATH=D:\gopath1.18.3
#GOBIN=
set GOARCH=amd64
set GOOS=windows
set CGO_ENABLED=1

rem #PATH=c:\mingw64\bin;%GOROOT%\bin;%PATH%
rem #PATH=D:\new\tdm-gcc-64-5.1.0-2\gdb64\bin;%GOROOT%\bin;%PATH% # gdb64 不行 
set PATH=D:\gopath1.18.3\bin;D:\new\mingw64-8.1.0-sjlj\mingw64\bin;%GOROOT%\bin;%PATH%

rem #//GO111MODULE 在 1.18.3 这样的版本中一定要配合正确的 GOPATH 才能正常工作
rem #go 1.18.3 下也还是可以用的. 有了这个变量定义 1.18.3 或者说 1.10.8 之后的版本才会去找项目自身下的 vendor 目录
rem #因为 gomod 目前已知是一定要找 gopath 然后在下面生成一个 pkg 目前的，不利于不同项目分别备份代码，所以还是用 vendor
rem #比较简单。要特别注意的是，开启 gomod 的情况下，是不推荐手改 vendor 下的代码的，因为 gomod 会下载全新代码覆盖它!
set GO111MODULE=on

rem 这一句如果有了的话就不要设置了
echo %GOPROXY%
rem set GOPROXY=https://proxy.golang.com.cn,direct

rem --------------------------------------------------------
rem 为了 fyne 的安卓环境还要有以下两个
set ANDROID_HOME=D:\asd_2019\sdk
set ANDROID_NDK_HOME=D:\asd_2019\sdk\ndk\23.0.7599858

start





