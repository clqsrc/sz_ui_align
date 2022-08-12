
#ifndef _C_UI_JSON_WINDOWS_H_
#define _C_UI_JSON_WINDOWS_H_

//按道理不应该有这个文件，不过确实需要一些平台特有的 ui 操作子函数

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------

//#include <jni.h>

#include "base_c.h"

chandle create_view(chandle parentView); //实际上在 c_ui_json.h 中声明的 //有点乱，以后再整理

chandle ui_json_CreateView(const char * _ui_class, chandle parentView);

//---------------------------------------------------------------

#ifdef __cplusplus
}
#endif



#endif
