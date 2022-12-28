

# 方法1

msys2 中

### 1.1

安装 git 。

```
pacman -S git
```

### 1.2

先克隆一个初始版本。

```
git clone https://github.com/clqsrc/sz_ui_align
```

### 1.3

更新

要先进入到代码的目录

```
cd sz_ui_align
git pull
```

# 方法2

安装有 windows 版本 git 的话


--------------------------------------------------------


1. 本地代码未修改，只有master分支，直接更新

git pull

但前提必须是本地的代码没更改过。比如，你提交了代码到 github 后，随后别人也提交代码到 github，然后你需要更新别人提交的代码到你本地，

就可以直接使用该命令。假如你提交代码后再修改过你本地的代码，就会产生冲突，直接使用该命令会失败的。

2. 本地代码有修改，多分支。

复制代码

//切换到master分支
git chekout master

//更新master分支
git pull

//切换到自己的分支isso
git checkout isso

//把master分支合并到自己分支
git merger master

复制代码

3. 本地代码有修改，只有master分支，直接覆盖本地代码

//重置索引和工作目录
git reset --hard

//更新代码
git pull

4.本地代码有修改，直接覆盖远程仓库代码

// 覆盖isso分支
git push --force origin isso

// 覆盖主分支
git push --force origin master

