
源码下载自 https://github.com/electron/electron-quick-start  [2022]
从更新记录看，它时常会变动。所以以下说法不一定全部一致。


官方说明的运行命令有两行
```
# Install dependencies
npm install
# Run the app
npm start
```

实际上只要一行

```
npm start
```

就可以了，在国内环境下 npm install 反而不会成功。

所以目前的手工安装 electron 环境命令如下。


//--------------------------------------------------------


参考 https://blog.csdn.net/weixin_45195200/article/details/116452259

1.
安装好 nodejs 环境后。

2.[我的 ubuntu 22.04 装不上，请参考 《_linux_下安装的一些问题.md》]
使用 npm 得速度速度超慢，推荐可以把 npm 的仓库切换到国内淘宝仓库，注册 cnpm 命令：

npm install -g cnpm --registry=https://registry.npm.taobao.org
npm install -g cnpm --registry=http://registry.npmmirror.com

3.
然后使用 cnpm 进行安装：

cnpm install electron -g

4.
安装好后，我们可以通过 -v 命令检测 electron 是否安装成功，命令如下所示：

electron -v

5.
安装打包输出工具

为了方便最终成果输出，建议安装 electron-packager 工具，安装也很简单，建议以下面的命令全局安装：

npm install -g electron-packager

-------------------------------------------------------------
原淘宝npm域名即将停止解析，请切换至新域名 npmmirror.com

http://npm.taobao.org和 http://registry.npm.taobao.org 将在 2022.06.30 号正式下线和停止 DNS 解析。

新域名为 npmmirror.com, 相关服务域名切换规则请参考：

http://npm.taobao.org => http://npmmirror.com
http://registry.npm.taobao.org => http://registry.npmmirror.com
