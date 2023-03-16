
//typescript.ts

//如何初始化一个 any 的成员变量，或者是 "类型|null" 的变量 //只能先声明一个变量吗

var t = {

    s:"",

    uijson:null,  //这样在赋值时会报错的

    popup_mini_uijson: null as unknown as ui_json,  //这样就不会报错了

}//


