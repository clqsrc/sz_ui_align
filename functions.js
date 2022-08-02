
/// <reference path="jquery-1.11.3.min.js" />

//https://www.cnblogs.com/kismet82/p/5479390.html
//js 中显示 jquery 提示的方法


//就算有 jquery 仍然有不的情况，比如 chrome 下产能直接写颜色的 alpha 值

//透明色
function Functions_colorTransparent()
{
    /*
    var form = new Alert("alert_id");
    form.Create();
    //form.set_background_color("#00252525", "#00353535");
    form.FullScreen();
    //form.set_background_color("#00ff0000", "#00ff0000"); //chrome 下无法用 alpha 颜色值设置透明
    //form.set_background_color("transparent", "transparent"); //ok
    //form.set_background_color("#05ff0000", "#05ff0000"); //chrome 下无法用 alpha 颜色值设置透明//半透明也不行
    //在 layer ui 中用的是 rgba(0,0,0,.7)
    //也可参考 https://www.cnblogs.com/chuaWeb/p/5092087.html

    form.set_background_color("rgba(0,0,0,.7)", "rgba(0,0,0,.7)");
    */
    return "transparent"; 
  
}//

//半透明//注意参数是小数
function Functions_colorTransparent_half(alpha)
{
    /*
    var form = new Alert("alert_id");
    form.Create();
    //form.set_background_color("#00252525", "#00353535");
    form.FullScreen();
    //form.set_background_color("#00ff0000", "#00ff0000"); //chrome 下无法用 alpha 颜色值设置透明
    //form.set_background_color("transparent", "transparent"); //ok
    //form.set_background_color("#05ff0000", "#05ff0000"); //chrome 下无法用 alpha 颜色值设置透明//半透明也不行
    //在 layer ui 中用的是 rgba(0,0,0,.7)
    //也可参考 https://www.cnblogs.com/chuaWeb/p/5092087.html

    form.set_background_color("rgba(0,0,0,.7)", "rgba(0,0,0,.7)");
    */
    return "rgba(0,0,0," + alpha +  ")"; //rgba(0,0,0,.7)
  
}//

//这种 ajax 请求 webview 是取不到的
function Functions_httpGet(_url) {

    //$.ajax({url:"/jquery/test1.txt",async:false}); //同步

    //异步
    $.ajax({url: _url, async:true});

}//

//这种 ajax 请求 webview 是取不到的//这个可以
function Functions_httpGet_webview(_url) {

    //$.ajax({url:"/jquery/test1.txt",async:false}); //同步

    //异步
    window.location = _url;

}//

//返回上一页面
function Functions_httpGet_webview_return() {

    //history.back(-1); //直接返回当前页的上一页，数据全部消息，是个新页面
    history.go(-1); //也是返回当前页的上一页，不过表单里的数据全部还在

}//


//返回上一页面
function Functions_return() {

    //history.back(-1); //直接返回当前页的上一页，数据全部消息，是个新页面
    history.go(-1); //也是返回当前页的上一页，不过表单里的数据全部还在

}//


//encodeURI 和 decodeURI 函数操作的是完整的 URI；这俩函数假定 URI 中的任何保留字符都有特殊意义，所有不会编码它们。
//encodeURIComponent 和 decodeURIComponent 函数操作的是组成 URI 的个别组件；这俩函数假定任何保留字符都代表普通文本，所以必须编码它们，所以它们（保留字符）出现在一个完整 URI 的组件里面时不会被解释成保留字符了。
//以上说明摘自
//参考 https://www.w3.org/html/ig/zh/wiki/ES5/%E6%A0%87%E5%87%86_ECMAScript_%E5%86%85%E7%BD%AE%E5%AF%B9%E8%B1%A1#.E5.A4.84.E7.90.86_URI_.E7.9A.84.E5.87.BD.E6.95.B0.E5.B1.9E.E6.80.A7
function function_httpEncode(s){
    return encodeURIComponent(s);
}//

function function_httpDecode(s){
    return decodeURIComponent(s);
}//

//my_ajax.js 要用到的计算字符串所占字节数的函数
//参考 http://www.cnblogs.com/1175429393wljblog/p/4562736.html

/**
 * 计算字符串所占的内存字节数，默认使用UTF-8的编码方式计算，也可制定为UTF-16
 * UTF-8 是一种可变长度的 Unicode 编码格式，使用一至四个字节为每个字符编码
 *
 * 000000 - 00007F(128个代码)      0zzzzzzz(00-7F)                             一个字节
 * 000080 - 0007FF(1920个代码)     110yyyyy(C0-DF) 10zzzzzz(80-BF)             两个字节
 * 000800 - 00D7FF
 * 00E000 - 00FFFF(61440个代码)    1110xxxx(E0-EF) 10yyyyyy 10zzzzzz           三个字节
 * 010000 - 10FFFF(1048576个代码)  11110www(F0-F7) 10xxxxxx 10yyyyyy 10zzzzzz  四个字节
 *
 * 注: Unicode在范围 D800-DFFF 中不存在任何字符
 * href="http://zh.wikipedia.org/wiki/UTF-8">http://zh.wikipedia.org/wiki/UTF-8
    *
    * UTF-16 大部分使用两个字节编码，编码超出 65535 的使用四个字节
    * 000000 - 00FFFF  两个字节
    * 010000 - 10FFFF  四个字节
    *
    * href="http://zh.wikipedia.org/wiki/UTF-16">http://zh.wikipedia.org/wiki/UTF-16
    * @param  {String} str
    * @param  {String} charset utf-8, utf-16
    * @return {Number}
    */
////var sizeof = function(str, charset){   
function function_sizeof(str, charset){
    var total = 0,
        charCode,
        i,
        len;
    charset = charset ? charset.toLowerCase() : '';
    if(charset === 'utf-16' || charset === 'utf16'){//utf16 字符串的计算方法
        for(i = 0, len = str.length; i < len; i++){
            charCode = str.charCodeAt(i);
            if(charCode <= 0xffff){
                total += 2;
            }else{
                total += 4;
            }
        }
    }else{//utf8 字符串的计算方法
        for(i = 0, len = str.length; i < len; i++){
            charCode = str.charCodeAt(i);
            if(charCode <= 0x007f) {
                total += 1;
            }else if(charCode <= 0x07ff){
                total += 2;
            }else if(charCode <= 0xffff){
                total += 3;
            }else{
                total += 4;
            }
        }
    }
    return total;
}//



//字符串转换成整数//会去掉 px
function function_StrToInt(s){
    //注意：parseInt方法在format'00'开头的数字时会当作2进制转10进制的方法进行转换；

    //所以建议string转int最好用Number方法；

    //s = replace(regexp/substr,replacement)
    //s = s.replace("px", ""); //ok 但不通用

    var news = "";
    for(i=0; i<s.length;i++){

        var c = s.charAt(i);

        //alert(s.charAt(i));

        if (c == '.') //小数点暂时也算吧
        news = news + s.charAt(i);

        if (c == '-') //负数应该也算 //2021.9
        news = news + s.charAt(i);

        if (c >= '0' && c <= '9') 
        news = news + s.charAt(i);

    }//for

    s = news;

    return Number(s);

}//

//分解出 hex 格式颜色值的 rgb
//substr(start,length)表示从start位置开始，截取length长度的字符串
function ColorHex2Rgb(s){

    var r = s.substr(1,2);
    var g = s.substr(3,2);
    var b = s.substr(5,2);

    //alert("rgb(" + r + "," + g + "," + b + ")");

    r = parseInt("0x" + r);
    g = parseInt("0x" + g);
    b = parseInt("0x" + b);

    return [r,g,b];

}//

//颜色按 rgb 255 改变明暗//越大越明,越小越暗//这是个简单的方法,数值大了是不对的
function Functions_ColorHighlight(colorHex, color256_add){
    var rgb = ColorHex2Rgb(colorHex);
    
    //alert(rgb);

    r = rgb[0];
    g = rgb[1];
    b = rgb[2];

    //alert("rgb(" + r + "," + g + "," + b + ")");

    r = Math.min(255, r + color256_add);
    g = Math.min(255, g + color256_add);
    b = Math.min(255, b + color256_add);

    //alert("rgb(" + r + "," + g + "," + b + ")");

    return "rgb(" + r + "," + g + "," + b + ")";
}

//以下为更正规的算法
//------------------------------------------------------
//js如何实现rgb与hsl的转换
//http://www.php.cn/js-tutorial-411745.html

/**
 * RGB 颜色值转换为 HSL.
 * 转换公式参考自 http://en.wikipedia.org/wiki/HSL_color_space.
 * r, g, 和 b 需要在 [0, 255] 范围内
 * 返回的 h, s, 和 l 在 [0, 1] 之间
 *
 * @param   Number  r       红色色值
 * @param   Number  g       绿色色值
 * @param   Number  b       蓝色色值
 * @return  Array           HSL各值数组
 */

function rgbToHsl(r, g, b){
    r /= 255, g /= 255, b /= 255;
    var max = Math.max(r, g, b), min = Math.min(r, g, b);
    var h, s, l = (max + min) / 2;
 
    if(max == min){
        h = s = 0; // achromatic
    }else{
        var d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch(max){
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }

        h /= 6;

    }

    //clq 需要小数,所以不取整了//return [Math.floor(h*100), Math.round(s*100)+"%", Math.round(l*100)+"%"];
    return [h, s, l]; //clq 需要小数,所以不取整了
}//

/**
 * HSL颜色值转换为RGB. 
 * 换算公式改编自 http://en.wikipedia.org/wiki/HSL_color_space.
 * h, s, 和 l 设定在 [0, 1] 之间
 * 返回的 r, g, 和 b 在 [0, 255]之间
 *
 * @param   Number  h       色相
 * @param   Number  s       饱和度
 * @param   Number  l       亮度
 * @return  Array           RGB色值数值
 */
function hslToRgb(h, s, l) {
    var r, g, b;

    if(s == 0) {
        r = g = b = l; // achromatic
    } else {
        var hue2rgb = function hue2rgb(p, q, t) {
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;

            return p;
        }

        var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        var p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

    return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}//

//根据以上的算法就可以得到更严格的依据 hsl 模型的亮度调整函数
function Functions_ColorHighlight_v2(colorHex, color256_add){
    var rgb = ColorHex2Rgb(colorHex);
    
    //alert(rgb);

    r = rgb[0];
    g = rgb[1];
    b = rgb[2];

    var hsl = rgbToHsl(r,g,b);

    h = hsl[0];
    s = hsl[1];
    l = hsl[2];

    //增加 l 就是加亮度了
    color256_add = Math.min(255, color256_add); //不能大于 255

    l = l + (color256_add) / 255;

    rgb = hslToRgb(h,s,l);
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];

    //alert("rgb(" + r + "," + g + "," + b + ")");

    r = Math.min(255, r);
    g = Math.min(255, g);
    b = Math.min(255, b);

    //alert("rgb(" + r + "," + g + "," + b + ")");

    return "rgb(" + r + "," + g + "," + b + ")";
}//


//https://www.cnblogs.com/lookforFree/p/4503656.html
//日期加一个月
//月是从 0 开始的,日是从1开始的
function Functions_addMonth(date, num) {
    num = parseInt(num);
    //var sDate = dateToDate(date);
    var sDate = date;

    var sYear = sDate.getFullYear();
    var sMonth = sDate.getMonth() + 1;
    var sDay = sDate.getDate();

    var eYear = sYear;
    var eMonth = sMonth + num;
    var eDay = sDay;
    while (eMonth > 12) {
        eYear++;
        eMonth -= 12;
    }

    var eDate = new Date(eYear, eMonth - 1, eDay);

    while (eDate.getMonth() != eMonth - 1) {
        eDay--;
        eDate = new Date(eYear, eMonth - 1, eDay);
    }

    return eDate;
}//

//日期减去1个月
function Functions_decMonth(y,m,d, num) {
    //var now = new Date(); //var week = new Date().getDay(); //取得周几,从 0 到 6
    //now.setFullYear(y, m-1, d); //月是从 0 开始的,日是从1开始的//这个会带有时间,所以要不时间部分还是用 new Date 的构造函数才对
    var now = new Date(y, m-1, d);

    num = parseInt(num);
    //var sDate = dateToDate(date);
    var sDate = now;

    var sYear = sDate.getFullYear();
    var sMonth = sDate.getMonth() + 1;
    var sDay = sDate.getDate(); //取日是比较特别的


    //----
    //先算出要减去多少年
    var numYear = Math.floor(num / 12); //Math.floor 相当于 delphi 的 trunc
    //再算减去多少月
    var num = num % 12; //月太大就取余数就行了
    //----    

    var eYear = sYear - numYear;
    var eMonth = sMonth - num;
    var eDay = sDay;


    var lastDay = Functions_getLastDayOfMonth(y, m); //应该是从 1 开始的//月中的哪一天

    if (eDay >= lastDay){ //如果 "日" 不同,说明新日期中的月份的最后一天比指定的日期小

        return eDate = new Date(eYear, eMonth-1, lastDay);
    }

    //eDate = new Date(eYear, eMonth, eDay);
    eDate = new Date(eYear, eMonth-1, eDay); //还是要减去1的//月是从 0 开始的

    return eDate;
}//

//到得一个月的最后一天
function Functions_getLastDayOfMonth(y,m) {
    //var now = new Date(); //var week = new Date().getDay(); //取得周几,从 0 到 6
    //now.setFullYear(y, m-1, d); //月是从 0 开始的,日是从1开始的//这个会带有时间,所以要不时间部分还是用 new Date 的构造函数才对
    //var now = new Date(y, m-1, d);

    //now = new Date(eYear, eMonth, 0); //构造函数和 setFullYear 不一样,这里的月是从 1 开始的
    //var lastDay = now.getDate(); //应该是从 1 开始的//月中的哪一天

    var last = 1;

    for (i=1; i<=31;i++){

        var d = i;
        var now = new Date(y, m-1, d); //月是从 0 开始的

        var sMonth = now.getMonth() + 1; //新时间的月//月是从 0 开始的

        if (sMonth > m) break; //如果 "日" 不同,说明新日期中的月份的最后一天比指定的日期小

        last = i;
    }


    return last;
}//

//日期加减去1天
//https://www.cnblogs.com/gmq-sh/p/5194706.html
function Functions_addDays(y,m,d, days){

    var now = new Date(y, m-1, d); //月是从 0 开始的

    var d = now; 
    d.setDate(d.getDate()+days); 
    //var m=d.getMonth()+1; 
    //return d.getFullYear()+'-'+m+'-'+d.getDate();     

    //其中，date参数是要进行加减的日期，days参数是要加减的天数，如果往前算就传入负数，往后算就传入正数，如果是要进行月份的加减，就调用setMonth()和getMonth（）就可以了，需要注意的是返回的月份是从0开始计算的，也就是说返回的月份要比实际月份少一个月，因此要相应的加上1。


    return d;
}//

function Functions_addMonths(y,m,d, months){

    ////这个函数当减去 3月29 的一个月时会显示为 3 月 1 日
    //以后再改了

    var now = new Date(y, m-1, d); //月是从 0 开始的

    var d = now; 
    //d.setDate(d.getDate()+days); 
    d.setMonth(d.getMonth()+months); 
    //var m=d.getMonth()+1; 
    //return d.getFullYear()+'-'+m+'-'+d.getDate();     

    //其中，date参数是要进行加减的日期，days参数是要加减的天数，如果往前算就传入负数，往后算就传入正数，如果是要进行月份的加减，就调用 setMonth()和getMonth（）就可以了，需要注意的是返回的月份是从0开始计算的，也就是说返回的月份要比实际月份少一个月，因此要相应的加上1。


    return d;
}//


//读取cookies //来自 https://www.cnblogs.com/limeiky/p/6927305.html 不一定完善，不过本程序范围内可用
//function getCookie(name)
//2020.01.27 目前已知无法处理重复的 key 的问题,这个可以认为是取到了第一个
//2020.01.31 根据 https://www.cnblogs.com/r00tuser/p/7993509.html 的说法，只有 python 是取后者的，其他的基本上是取前者 
function Functions_getCookie_v1(name) //2019.09.19
{
    var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");

    if(arr=document.cookie.match(reg))

    return unescape(arr[2]);
    else
    return null;
} //

//还是取最后一个才好//不对，从测试的情况下看，还是应该取第一个,不过 firefox 会只显示最后一个，这和后台程序取到的是不同的
function Functions_getCookie(name) //2020
{
    //return Functions_getCookie_v1(name);
    //return Functions_getCookie_last(name);
    return Functions_getCookie_first(name); //
} //


//和上面的差不多，不过会取到最后一个
function Functions_getCookie_last(name) {

    var value = "";

    var strCookie = document.cookie;    // 获取cookie字符串

    var arrCookie = strCookie.split(";");        //从分号的位置 分割字符串strCookie为字符串数组

    //遍历cookie数组，处理每个cookie对
    for (var i = 0; i < arrCookie.length; i++) {

        var arr = arrCookie[i].split("=");           //从 = 的位置 分割每对cookie

        //if (arr[0] == "userId") {      //如果前面是 userId 就是找到了
        //if (arr[0] == name) {      //如果前面是 userId 就是找到了

        var key = (arr[0].trim()); //这样取前面可能会有空格，所以要再 trim 一下

        if (key == name) {      //如果前面是 userId 就是找到了

            value = arr[1];         //将后面的值赋给 userId ，跳出循环

            //break; //不跳出，就是找最后一个了

        }//if key
    } //for

    //alert(userId);

    return value;

} //

//和上面的差不多，不过会取到第一个
function Functions_getCookie_first(name) {

    var value = "";

    var strCookie = document.cookie;    // 获取cookie字符串

    var arrCookie = strCookie.split(";");        //从分号的位置 分割字符串strCookie为字符串数组

    //遍历cookie数组，处理每个cookie对
    for (var i = 0; i < arrCookie.length; i++) {

        var arr = arrCookie[i].split("=");           //从 = 的位置 分割每对cookie

        //if (arr[0] == "userId") {      //如果前面是 userId 就是找到了
        //if (arr[0] == name) {      //如果前面是 userId 就是找到了

        var key = (arr[0].trim()); //这样取前面可能会有空格，所以要再 trim 一下

        if (key == name) {      //如果前面是 userId 就是找到了

            value = arr[1];         //将后面的值赋给 userId ，跳出循环

            break; //不跳出，就是找最后一个了

        } //if key
    } //for

    //alert(userId);

    return value;

} //




//取全路径文件名中的无路径文件名
function Functions_ExtractFileName(filePath){
    //var filePath = $(this).val(); //文件名//全路径


    //var arr = filePath.split('\\');
    //js中以多个字符拆分字符串
    var arr = filePath.split(/[\\\/]/); //这句话的意思其实就是按字符 "\" 和 "/" 拆分为数组，只是加了转义符。另外 js 中是可以用多个分隔箱号的，所以就形成了这个怪异的式子

    if (arr.length >= 1) {
        var fileName = arr[arr.length-1];

        return fileName;
    }

    return "";

}//

//取全路径文件名中的无路径文件名//后缀
function Functions_ExtractFileExt(filePath){
    //var filePath = $(this).val(); //文件名//全路径


    //var arr = filePath.split('\\');
    //js中以多个字符拆分字符串
    var arr = filePath.split(/[.]/); //这句话的意思其实就是按字符 "\" 和 "/" 拆分为数组，只是加了转义符。另外 js 中是可以用多个分隔箱号的，所以就形成了这个怪异的式子

    if (arr.length >= 1) {
        var fileName = arr[arr.length-1];

        return fileName;
    }

    return "";

}//

//txt 转换为 json//ie6,7,8 是没有 json 对象的
function Functions_TxtToJson(str){

    //var obj = $.parseJSON( jsonstr ); //jQuery.parseJSON(jsonstr),可以将json字符串转换成json对象 
    var obj = $.parseJSON(str);

    return obj;
}//

//js 数组的方便性函数而已
//加一个元素到动态数组中
function Functions_AddToList(list, item)
{
    if (list == null) list = new Array();

    list.push(item);

    return list;
}//

function AddToList(list, item)
{

    return Functions_AddToList(list, item);

}//

//整数转换为字符串
function IntToStr(v)
{
    return String(v);
}

function StrToInt(v)
{
	return function_StrToInt(v); //2021.06.09 第一次修复，原因参考 function_StrToInt
    //return parseInt(v);
}

function StrToFloat(v)
{
	return Number(v); //其实 function_StrToInt() 也转了浮点数
}//

function StrToBool(v)
{
	v = String(v);
	if (undefined == v || null == v) return v;
	
	v = v.toLocaleLowerCase().trim();
	
	return "true" == v;
}//

//同 golang 版本
var _g_id_Now_id = 0; //Now_id 用的全局变量，千万不要混用
function Now_id() 
{

    //不足两位的，前面加 0
    function _str2(v) {

        return StrToInt(v) < 10 ? "0" + v : IntToStr(v);
    }//

    var date = new Date();

    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    var _date = date.getDate();
    var hour = date.getHours();// < 10 ? "0" + date.getHours() : date.getHours();
    var minute = date.getMinutes();// < 10 ? "0" + date.getMinutes() : date.getMinutes();
    var second = date.getSeconds();// < 10 ? "0" + date.getSeconds() : date.getSeconds();
    var milliSeconds = date.getMilliseconds();

    var currentTime = year + _str2(month) + _str2(_date) + _str2(hour) + _str2(minute) + _str2(second) + '_' + milliSeconds + "_" + _g_id_Now_id;

    _g_id_Now_id = _g_id_Now_id + 1;

    return currentTime;
}//

//页面刷新
function Functions_HtmlRefresh()
{

	location.reload(true);
}//

//https://tool.chinaz.com/Tools/unixtime.aspx
//1970 的 unix 时间戳
function Functions_DateTime2Unix(_date_js)
{
    //return Math.round(new Date().getTime()/1000); //getTime()返回数值的单位是毫秒
    return Math.round(_date_js.getTime()/1000); //getTime()返回数值的单位是毫秒
}//

function Functions_Unix2DateTime(time_c)
{
    var unixTimestamp = new Date(time_c * 1000) 
    //然后 commonTime = unixTimestamp.toLocaleString()

    return unixTimestamp;
}//

function Functions_Unix2DateTime(_date_js)
{
    //return unixTimestamp.toLocaleString();
    return _date_js.toLocaleString();
}

function Functions_Unix2DateTime_String(time_c)
{
    var unixTimestamp = new Date(time_c * 1000) 
    //然后 commonTime = unixTimestamp.toLocaleString()

    return Functions_Unix2DateTime(unixTimestamp);
}//

//判断是否是 jquery 变量
function is_jquery(obj)
{
    //https://www.cnblogs.com/conis/archive/2010/03/20/1720385.html

    //var obj = $("body");
    if(obj instanceof jQuery){
        //alert("这是一个jQuery对象");
        return true;
    }else{
        //alert("这是一个其它对象")
        return false;
    }
}//

//取一个 dom 节点，代替 document.getElementById() //太常用了，简化操作而已
function dom_(id_or_jquery) {

    if (is_jquery(id_or_jquery)) {

        return id_or_jquery[0]; //据说 jquery 是一个数组，取其中一个就行 //也可以用 get(0), 例如 $("#aijquery").get(0);
    }

    //----
    var dom_node = document.getElementById(id_or_jquery);

    return dom_node;
}//


AddLog("load functions.js ok");
import_ok("functions.js");