
1.
安装的目录似乎是 
'/usr/local/lib/node_modules/electron'

2.[未证实]
ubuntu 22.04 下似乎无法安装 cnpm
根据 https://blog.csdn.net/weixin_43671362/article/details/106197732
换用直接指定 npm 下载地址

查看npm配置
npm config list

设置淘宝镜像
npm config set registry https://registry.npm.taobao.org

重置registry
npm config set registry https://registry.npmjs.org
或
npm config delete registry （未验证）
npm config edit
    用此命令打开 ".npmrc"文件
    添加或删除
registry=https://registry.npm.taobao.org/

    cnpm的用法同npm，且registry为国内淘宝镜像
    全局安装

npm i -g cnpm

    npm 安装依赖包尽量在电脑硬盘，在U盘巨慢。
    移动硬盘未知，待完善。

