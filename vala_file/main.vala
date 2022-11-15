

//https://zhuanlan.zhihu.com/p/471057010

//  在GNOME3平台上，需要以下命令来编译这一示例:
//  valac --pkg gtk+-3.0 hellogtk.vala


//--------------------------------------------------------
/*

当前版本可以这样编译出 c 代码
valac -ccode main.vala

通过 valac --help>help.txt 可知。

当前版本的安装命令如下：

1.
运行 msys2 的 ucrt64.exe

2.
在上面正常的环境中输入
pacman -S mingw-w64-x86_64-vala-language-server

来自 https://github.com/vala-lang/vala-language-server

3.
在上面正常的环境中输入
pacman -S mingw-w64-x86_64-gcc 
pacman -S mingw-w64-x86_64-pkg-config
pacman -S mingw-w64-x86_64-vala

来自 https://wiki.gnome.org/Projects/Vala/ValaOnWindows

4.不过奇怪的是这时候不能在 msys2 的环境中使用到 valac 命令。
查找到发现安装在 
D:\new\msys64\mingw64\bin
中。倒是可以直接在 windows 的命令行中进入并成功编译。

rem cmd /k "set PATH=D:\no_install\vala\vala-0.12.0\bin;%PATH% "

cmd /k "set PATH=D:\new\msys64\mingw64\bin;%PATH% "

*/

//  F:\new\Meld\libgtk-3-0.dll  应该是可以用的，它有 gtk_window_new 函数
//  https://blog.csdn.net/weixin_48842132/article/details/121249430

//--------------------------------------------------------


//  简陋的Vala

void main () {
    print ("Hello World\n");
}

//  使用面向对象特性的Vala

class Sample : Object {
    void run () {
        stdout.printf ("Hello World\n");
    }


    static void main2 (string[] args) {
        var sample = new Sample ();
        sample.run ();
    }
}

/*

使用GTK创建图形用户界面

using Gtk;
​
int main (string[] args) {
    Gtk.init (ref args);
​
    var window = new Window ();
    window.title = "Hello, World!";
    window.border_width = 10;
    window.window_position = WindowPosition.CENTER;
    window.set_default_size (350, 70);
    window.destroy.connect (Gtk.main_quit);
​
    var label = new Label ("Hello, World!");
​
    window.add (label);
    window.show_all ();
​
    Gtk.main ();
    return 0;
}

*/