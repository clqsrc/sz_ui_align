

//#include "stdio.h"

int main()
{

    //printf("hello.\r\n");

    return 0;

}//



//https://baijiahao.baidu.com/s?id=1739050938692492659&wfr=spider&for=pc
//GCC编译过程
// 首先我们来看GCC的编译过程，hello.c的源代码如下。
// 在编译时添加“-save-temps”和“--verbose”编译选项，前者用于将编译过程中生成的中间文件保存下来，
// 后者用于查看GCC编译的详细工作流程，下面是几条最关键的输出。

//所以可以使用命令 zig cc -save-temps zig1.c