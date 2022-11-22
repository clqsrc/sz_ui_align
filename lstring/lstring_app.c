//
// Created by ccc on 8/4/21.
//




//---------------------------------------------------------------


#include "base_c.h"
#include "lstring_app.h"

#ifdef _android_

    #include <stdio.h>
    #include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了
    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _ios_

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _windows_

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    #define byte unsigned  char

#endif

#ifdef _qt_  //直接用 ios 的就行

    #include <stdio.h>

    //ios 没有这个？
    //#include <malloc.h> //有些编译,如果 malloc.h 在后面的话会报 malloc 函数冲突,解决办法很简单,把含有 malloc 的头文件放前面,好让我们的 malloc 定义能覆盖它就可以了

    #include <stdlib.h>  //ios for malloc
    #include <ctype.h>  //ios for tolower

    #include <string.h>
    //#include <time.h>
    //#include <winsock.h>
    //#include <windows.h>
    #include <time.h>

    //#define byte unsigned  char
    typedef unsigned char byte;

#endif

#include "c_functions.h"

//---------------------------------------------------------------

//根据不同的情况，用户自己定义不同的函数
void _lstring_check_mem_ShowError_APP(const char * s)
{
    ShowMessage_c(s, NULL);

}//

