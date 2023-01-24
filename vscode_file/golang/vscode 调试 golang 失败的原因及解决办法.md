
http://newbt.net/ms/vdisk/show_bbs.php?id=BDF78AB3B80C08CFC72C6A175ABCAFCA&pid=164

vscode 调试 golang 失败的原因及解决办法

vscode 调试 golang 经常失效，实在让人头痛。这也是很才段时间里我一直使用 vscode 开发了好多其他语言的程序，但 golang 的始终用 liteide 的原因。

最近升级了 vscode 和 golang 插件后又调试失败了 ...

原因是它不能再共用之前版本的 go 的 gopath ，也就是说如果你的 go 版本升级了，得给它一个新的 gopath ，这其实也没什么，因为对于 dlv.exe 这样的程序来说，如果编译到 gopath 下，本身就是不能版本通用的。
必须一个 go 版本一个。但这对于 gopath/src 就麻烦了。因为我要经常在多个编译环境下编译同一份代码。

之前的解决办法是只在老版本的 gopath/src 中存一份代码，在新版本中时临时修改环境变量中的 gopath 。这在之前的工作中一直很好用，但最近升级后发现只能编译不能调试，折腾好久才发现换一个全新 gopath 可以调试。
所以目录的办法是将 src 在 windows 下用 mklink 软链接过去。linux 下暂时还没测试。

对于我的项目来说是要修改 launch.json 中的 env 节点中的 GOPATH 。
说实在的，我觉得这只能说 dlv 这个调试程序太蠢，而且调试的断点失败应该提示原因。唉，开源自己搞的就是这样了，难用，但又确实能用，折腾死人。

```
//"GOPROXY":"https://goproxy.io,direct", //这里不行，要写到环境变量中 //https://goproxy.io/zh/
//现在是这个命令 $env:GOPROXY = "https://proxy.golang.com.cn,direct"
"GOPATH":"D:/gopath1.18.3",

// "GOPATH":"D:/gopath1.10.8", //2023 如果和 "go.gopath" 的配置不同，有可能会导致 dlv.exe 找不到源码而调试失败
//目前要调试的源码最好在这个配置的 src 之下
//dlv.exe 在 gopath/bin 中还是 go/bin 中也有可能对是否可以调试产生影响
//在目前[2023] 的 vscode 插件中，会把 dlv.exe 编译到 gopath/bin 中，对原有的项目影响比较大的
//这对于以前直接在 1.18.3 中直接使用 1.10.8 的源码造成了比较大的困难。目前的解决办法是 windows mklink
//对 src 进行软链接。这样做之后，还要通过在 vscode 中直接打开 "D:/gopath1.18.3/..." 完整路径，如果在
//打开对话框中一个个路径进去的话会导致仍然是 "D:/gopath1.10.8" 的目录，导致调试失败

"GOROOT": "D:/go1.18.3.windows-amd64/go",
// "GOROOT": "D:/go1.10.8.windows-amd64/go",

//其实，如果在 setting.json 中设置了 "go.goroot" "go.gopath" 那么这两个最好不要设置

//----

```

