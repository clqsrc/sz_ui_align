#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* windows only */
#include <Windows.h>
#include <conio.h>  // _kbhit


int __stdcall addS(int a,int b)
{
	return a + b;

}//
	

int __cdecl addC(int a,int b)
{
	return a + b;

}//

int input = 0;
int result = 0;
extern     int     input,result;

int main(int argc, const char* argv[])
{
	//input= 1;
  	//__asm__ __volatile__ ("movl %1,%0":"=r" (result) : "r" (input));
	
	input = 0;
	
	__asm__ __volatile__("nop");   //空语句
	
	__asm__ __volatile__("movl $1, %eax");
	
	//__asm__ __volatile__("movl input, %eax");
	//__asm__ __volatile__ ("movl %1, %%eax":"=r" (result) : "r" (input));
	//__asm__ __volatile__ ("movl %%eax, %1":"=r" (result) : "r" (input));
	__asm__ __volatile__ ("movl $11, %0":"=r" (result) : "r" (input));  //把 1 赋值给 result 
	
	//GCC编译器默认的汇编风格为：AT&T格式，但很多人其实只习惯 Intel格式。 
	//所以：如果我们在GCC上使用内联汇编就需要配置编译器的 MASM 汇编器选项来支持 intel格式汇编
	//lc-3 似乎也是左边为目的地的，和 intel 一样 
	
	//__asm__ __volatile__ ("movl $1, %%ebx":"=r" (result) : "r" (input));  
	__asm__ __volatile__("movl $100, %ebx");   //好像是有参数的时候，ebx 这些才用两个 % 号 //右边的是接收者，这似乎是 at 风格 
	__asm__ __volatile__ ("movl %%ebx, %0":"=r" (result) : "r" (input));  
	
	//__asm__ __volatile__("movl %0, %%eax":"=r" (input));
	
	//input= 1;
	//__asm__ __volatile__ ("movl %1,%0" :
	//"=r" (result) : "r" (input));
	
	printf("%d\r\n", result);
	
	return 0;
	
}//



/*

https://zhuanlan.zhihu.com/p/527208939
“X86汇编语言风格比较: AT&T 和 Intel 风格” 
//编译 
gcc -c main.c

//反汇编为 AT&T风格 
objdump -S main.o

//反汇编为 Intel/微软 风格 
objdump -d -mi386:x86-64:intel main.o


*/

//--------------------------------------------------------
/*
函数调用的解释 

0000000000000068 <addC>:
  68:	55                   	push   %rbp
  69:	48 89 e5             	mov    %rsp,%rbp
  6c:	89 4d 10             	mov    %ecx,0x10(%rbp)
  6f:	89 55 18             	mov    %edx,0x18(%rbp)
  72:	8b 55 10             	mov    0x10(%rbp),%edx
  75:	8b 45 18             	mov    0x18(%rbp),%eax
  78:	01 d0                	add    %edx,%eax
  7a:	5d                   	pop    %rbp
  7b:	c3                   	ret

这里很显然，不用看手册 %rbp 肯定是参数栈。这在 lc3 里是没有的，不过可以直接用一个地址来模拟

第二行把 rsp 的数据给参数栈，然后取参数给 ecx 和 edx 

https://blog.csdn.net/lyndon_li/article/details/128605884
"在最新的 x86_x64 架构中，通常用 rbp、rsp 这两个寄存器来保存进程栈的状态（需要硬件支持）。
其中 rbp 保存的是栈中当前执行函数的基本地址，当前执行函数所有存储在栈上的数据都要靠 rbp 指针加上偏移量来读取。
而 rsp 就是常说的栈指针，它永远指向一个进程的栈顶。"

2.
%eax 一般是用来放返回值的，所以最后的结果要放在里面。

3.
ret 是返回调用处，这里扩展知识应该是 ret 其实是 pop 加 jmp 的功能，即调用前返回地址是放到某个地方的。如果过程中修改了它就会返回
到别的地方的，也就是溢出攻击。

不同风格这个还不同。在 stdcall 里是要用 ret 8 的，具体看 https://www.cnblogs.com/hanford/p/6177872.html

4.
以上其实是 64 位的， 32 位其实是
esp,ebp 

寄存器esp保存着程序堆栈的栈顶地址
esp 大概是 lc3 中的 pc 位置吧，即当前代码的位置。 

5.
参数是放在 ecx 和 edx 中的，似乎参数多于 6 个就要用堆栈来传递参数了。

6.
可以看到 ebp/rbp(64位) 不过是为了方便在函数中计算地址的临时寄存器。 sp(esp/rsp[64位]) 才是程序中总体的栈位置。
参考 
“
http://blog.csdn.net/yeruby/article/details/39780943
esp是栈指针，是cpu机制决定的，push、pop指令会自动调整esp的值；
ebp只是存取某时刻的esp，这个时刻就是进入一个函数内后，cpu会将esp的值赋给ebp，此时就可以通过ebp对栈进行操作，比如获取函数参数，局部变量等，实际上使用esp也可以；
既然使用esp也可以，那么为什么要设定ebp呢？
答案是为了方便程序员
因为esp在函数运行时会不断的变化，所以保存一个一进入某个函数的esp到ebp中 会方便程序员访问参数和局部变量，而且还方便调试器分析函数调用过程中的堆栈情况。前面说了，这个ebp不是必须要有的，你非要使用esp来访问函数参数 和局部变量也是可行的，只不过这样会麻烦一些。
” 
这也是为什么 lc-3 只有那么点指令和寄存器也是可以实现 c 语言的，可以看到完成的 x86/64 汇编其实也有很多语法糖。 


7.
用 call 来代替 jmp 调用函数也是为了方便。参考 https://blog.csdn.net/u014029795/article/details/118076698
“
CALL指令调用函数
这里再使用CALL指令来调用函数，由于CALL指令会将当前指令的下一行存储在堆栈中，所以直接在函数的最下面进行ret就可以回到之前执行函数的地方了。
” 

8.
x86架构
通用寄存器(32bit)
通用寄存器是程序执行代码最最常用，也最最基础的寄存器，程序执行过程中，绝大部分时间都是在操作这些寄存器来实现指令功能。
eax：通常用来执行加法，函数调用的返回值一般也放在这里面
ebx：数据存取
ecx：通常用作计数器，比如for循环
edx：暂不清楚
esp：栈顶指针，指向栈的顶部
ebp：栈底指针，指向栈的底部，通常用ebp+偏移量的形式来定位函数存放在栈中的局部变量
edi：字符串操作时，用于存放数据源的地址
esi：字符串操作时，用于存放目的地址的，和edi两个经常搭配一起使用，执行字符串的复制等操作 

9.
上面有个地方说错了。lc-3 中的 pc 寄存器指向下一条指令，在 x86 中对应的是 eip/rip(64位)/ip(8086架构(16bit)) 。
称为 “控制寄存器”，好像是不能直接修改的。 

10.
Verilog HDL
"
LC3指令流水线CPU
根据LC3指令集，用verilog HDL语言写的流水线CPU，已经通过仿真、烧片测试。
"

*/ 


