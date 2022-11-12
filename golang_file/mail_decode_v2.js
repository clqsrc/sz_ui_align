
//otto 不支持 const 也不支持 {mail_error, MailMime_errorMail_forTest} 这种语法.
//也不支持 let
/*
区别：
1、es6新增了箭头函数，es5没有；
2、ES6中新增了块级作用域，es5没有；
3、ES6引入Class概念，不再像ES5一样使用原型链实现继承；
4、ES6中可以设置默认函数参数，es5不行；
5、ES6中新增了promise特性。

另外 IE 11 以后才支持的 let

反引号也是 es6 才支持的。
ES6 模板字符串(Template String)是增强版的字符串，用反引号(`)标识，它可以当作普通字符串使用，也可以用来定义多行字符串，或者在字符串中嵌入变量。

*/
//--------------------------------------------------------

//纯 js 的 functions 。设计上要求兼容 nodejs 和纯 web 环境  //v2 使用函数的版本

//解码一段字符串的邮件  //传递变量的方式


if (false){
//var v = {mail_error, MailMime_errorMail_forTest};

//var {mail_error, MailMime_errorMail_forTest} = require('./mime_functions_error')
var functions  = require("./functions");
var ShowMessage = require("./node_functions");
}

//var functions = require("./t2");
var functions = require("./mail_functions");

//ShowMessage("functions:" + functions);
//ShowMessage("functions:" + JSON.stringify(functions));

//----------------------------------------------------------------

//解码一段字符串的邮件  //传递变量的方式
// func main_js_nodejs_DecodeMail_forString(mail_src string) string  //golang ver
function DecodeMail_forString(mail_src) {

    ShowMessage("mail_src:" + mail_src);

//----------------------------------------------------------------


var result = "";  //传递给引擎的返回值

//----------------------------------------------------------------
//纯 c 风格函数，方便移植

//有折行的 line 变成无折行的 //第一个字符是空格或者 tab 的就是 上一行的内容折下来的
function MailMime_srcLines2lines(srcArr)
{
    var r = new Array();

    var cur_line = ""; //当前行重置为空行
    for (var i = 0; i < srcArr.length; i++) {
        var line = srcArr[i];

        //是否是上一行的内容
        var is_half = 0;

        if (line.length<1) { continue; }  //按道理不应该有空行

        if (' ' == line[0] || '\t' == line[0]) { is_half = 1; }

        if (1 == is_half){

            line = line.substring(1);  //跳过第1个字符就可以了
            cur_line = cur_line + line;

        }else{
            cur_line = cur_line + line;

            cur_line = cur_line.trim();   //这个时候才能进行 trim() 操作
            r.push(cur_line);

            cur_line = "";  //当前行重置为空行
        }//if 1
        
    }//for 1

    //还有没加上去的也要补上去
    if (cur_line.length>0) { r.push(cur_line); }
    

    return r;
}//


//简单解码为 map //一般只解码 head
function MailMime_DecodeLines2Map(srcArr)
{
    var r = new Array();

    try {


    //简单点的话，用 var lines = s.split(": "); 分隔第一行就可以了。不过从协议上来说应该是只处理第一个分隔符的

    //var cur_line = ""; //当前行重置为空行
    for (var i = 0; i < srcArr.length; i++) {
        var line = srcArr[i];

        //注意分隔符号是英文 ":" 加一个空格
        var kv = line.split(": "); 

        if (line.length<2) { continue; }  //按道理至少有两列

        var key = kv[0];

        var left_len = key.length + 2;  //分隔符左边的字符串长度

        var value = line.substring(left_len);

        r[key] = value;        


        
    }//for 1

    //----------------------------------------------------------------
            
    } catch (e) {

        console.log(e);
            
    }

    return r;
}//


//解码 quoted-printable 这是一个很关键的代码，一定不能出错
/*
Quoted-printable编码方法
    任何一个8位的字节值可编码为3个字符：一个等号”=”后跟随两个十六进制数字(0–9或A–F)表示该字节的数值.
    例如，ASCII码换页符（十进制值为12）可以表示为”=0C”, 等号”=”（十进制值为61）必须表示为”=3D”. 
    除了可打印ASCII字符与换行符以外，所有字符必须表示为这种格式.
    所有可打印ASCII字符(十进制值的范围为33到126)可用ASCII字符编码来直接表示, 
    但是等号”=”(十进制值为61)不可以这样直接表示.ASCII的水平制表符(tab)与空格符, 十进制为9和32, 
    如果不出现在行尾则可以用其ASCII字符编码直接表示。如果这两个字符出现在行尾，必须QP编码表示为”=09″ (tab)或”=20″ (space).
    如果数据中包含有意义的行结束标志，必须转换为ASCII回车(CR)换行(LF)序列，既不能用原来的ASCII字符也不能用QP编码的”=”转义字符序列。 
    相反，如果字节值13与10有其它的不是行结束的含义，它们必须QP编码为=0D与=0A.
    quoted-printable编码的数据的每行长度不能超过76个字符. 
    为满足此要求又不改变被编码文本，在QP编码结果的每行末尾加上软换行(soft line break). 即在每行末尾加上一个”=”, 但并不会出现在解码得到的文本中.
*/
//这里有个问题，因为 js 只支持 utf8 ，所以有可能会得到乱码
function MailMime_Decode_quoted_printable(srcArr)
{
    // var count = str.length;

    var r = "";

    for (var i = 0; i < srcArr.length; i++) {
        var line = srcArr[i];

        line = line.trim();

        //如果最后一个字符是 "=" 则要先删除，因为这是 qp 编码的软回车
        if ("=" == line[line.length-1]) {

            //line[line.length-1] = " ";  //应该是不能写入的，lua 的字符串也是不能写入单个字符的. 参考 lib.es5.d.ts 中的 readonly [index: number]: string;
            //line[1] = "\20";

            line = line.substring(0, line.length-1);

            //line = MailMime_Decode_quoted_printable_one_line(line); //不能单独解码一行，因为有可能切断一个 utf8 字符串。所以要合成了整个字符串后再转换

            //ShowMessage(line);

            r = r + line;

        }else{
            r = r + line + "\r\n";   //据 lua 官方教程书，这种情形下应该使用 stringbuilder 在 lua 中是先放到 table 中，最后整体输出字符串
        }


    }//

    r = MailMime_Decode_quoted_printable_one_line(r); //不能单独解码一行，因为有可能切断一个 utf8 字符串。所以要合成了整个字符串后再转换

    //ShowMessage(r);

    return r;

}//

//利用 json 转换16进制为 utf8 ,JSON.stringify("\x00 \x0a") //直接利用 utf8 我也是可以的，不过太啰嗦，而且处理不了乱码的情况
//很啰嗦，直接换用 httpencode/decode 好了
function MailMime_Decode_StrinFromHex_json_(str_json)
{
    var s_json = '{"r":"' + str_json+ '"}';

    //var obj_json = JSON.stringify(s_json);
    ////var obj_json = JSON.parse(s_json);

    var s = "\x0d\x0a\r\n"; 

    //var s2 = JSON.parse('{"r":"\\r\\n\\x0d"}');

    //js中escape和unescape方法

    // unescape
    s = decodeURIComponent(str_json);


    return s;
    return "aaa";
    return String.raw("str_json");
    return String.raw(str_json);

    return obj_json.r;
}
    
//处理单行，注意，一定要与处理多行的相配合才正确
function MailMime_Decode_quoted_printable_one_line(str)
{

    var r = "";

    var cur_str = "";  //这个比较特殊，因为 js 的字符串是 utf8 的，单个字符的长度实际上超过1个字节大于 255 。所以只能取一整片字节流后整体处理成一段字符串。

    var count = str.length;
    var i = 0;
    // for (var i = 0; i < str.length; i++) 
    for (; i < count; ) 
    {
        var c = str[i];

        //js 里的一个字符应该是可以大于 255 的，因为它是 utf8 。参考 https://blog.csdn.net/ole_triangle_java/article/details/78227329

        // charAt(num) //获取字符串的num位置的字符
        // charCodeAt(num)//获取字符串的num位置的字符的unicode编码　
        // fromCharCode(num)//获取unicode编码对应的字符

        //更权威的，可以看 lib.es5.d.ts 中的 interface String 部分
        // * Returns the character at the specified index.
        // * @param pos The zero-based index of the desired character.
        // charAt(pos: number): string;

        //  * Returns the Unicode value of the character at the specified location.
        //  * @param index The zero-based index of the desired character. If there is no character at the specified index, NaN is returned.
        // charCodeAt(index: number): number;

        var c_int = str.charCodeAt(i);  //这时候是 number int 。大于 255 的话应该就是双字节了，实际上根据 utf8 的特点还有可能是 3字节


        if ("=" == c) {
            //"=" 号后的两个字符串组成了实际的字符
            var c_int2 = str.charCodeAt(i+1);  //int
            var c_int3 = str.charCodeAt(i+2); 

            var c2 = str.charAt(i+1);   //int
            var c3 = str.charAt(i+2); 

            i = i + 2;  //这里多处理了两个字符，所以索引下移两位

            //cur_str = cur_str + "\\x" + c2 + c3;  //("\x00 \x0a")
            cur_str = cur_str + "%" + c2 + c3;  //("\x00 \x0a")  //算了，用 http 编码好了

        }else{
            

            if (cur_str.length>0) { 
                r = r + MailMime_Decode_StrinFromHex_json_(cur_str); 
                cur_str =""; 
            }  //如果有未翻译的片段，加上去

            r = r + c;

        }
        //


        //----
        i++;
        
    }//for 

    //----
    //最后还要再处理一次片段
    if (cur_str.length>0) { r = r + MailMime_Decode_StrinFromHex_json_(cur_str); cur_str =""; }  //如果有未翻译的片段，加上去


/*
js 中的字符和字符串，以及字节
由于javascript是unicode编码的,所有的字符对于它来说一个就是一个，但是后台程序不是，通常在后台程序中一个中文是占两个字节的，
这就导致了前后端校验长度不一致，这个问题可以通过正则来解决。

function getRealLen( str ) {  
    return str.replace(/[^\x00-\xff]/g, '__').length; //这个把所有双字节的都给匹配进去了  
} 

http://t.zoukankan.com/wdlhao-p-5247652.html

js获取字符串字节数方法小结

js获取字符串字节数的方法。分享给大家供大家参考。具体如下：

大家都知道，获取字符串的长度可用length来获取，
那么获取这段字符串的字节数呢？

英文字母肯定lenght和字节数都一样：都是１
而中文lenght=1,字节数=2
因此，需要作的就是把中文字符的字节数计算出来。

方法一：

alert('a'.replace(/[^u0000-u00ff]/g,"aaa").length); 
//原理：把中文字符替换成2个英文字母，那么字节数就是2，
//示例中改成替换成3个英文字母了。
//因此弹出的字节数是3，如果要正确的，当然是替换成2个字母了
//u0000这个表示的是unicode编码

方法二：

var str='我我我';
var bytesCount;
for (var i = 0; i < str.length; i++)
{
  var c = str.charAt(i);
  if (/^[u0000-u00ff]$/.test(c)) //匹配双字节
  {
  bytesCount += 1;
  }
  else
  {
  bytesCount += 2;
  }
}
alert(bytesCount);
//结果是6
//原理也很简单，用正则判断是不是中文，如果是的话，字节数就加1。

匹配中文字符的正则表达式： [u4e00-u9fa5]

匹配双字节字符(包括汉字在内)：[^x0000-x00ff]

可以用来计算字符串的长度（一个双字节字符长度计2，ASCII字符计1）

JS中的几个函数：

charAt(num) //获取字符串的num位置的字符
charCodeAt(num)//获取字符串的num位置的字符的unicode编码　
fromCharCode(num)//获取unicode编码对应的字符


*/

    return r;
}//


//----------------------------------------------------------------


//这是邮件专用的 mime 模块，和 golang 主要用于 http 是不同的
var MailMime = {

    //新建立一个解码类
    create_new_decode : function(){

        return {

            head_lines:new Array(),      //这是一个数组
            head_map:new Array(),  //这是一个 map ，并不等同于 head 。head 是字符串的顺序数组简单列表，而这个是 key/value 映射表。实际上可能是有多个相同的 key 的，不过
            //我们这里只取最后一个就可以了
            head_src_lines:new Array(),   //一行字符在保存时是有可能折成多行的，所以这个是未折行时的原始内容。head 中的是已经合并过的

            //--------
            //头信息以外的就是 body 了，它与头信息是由一个空行相分隔的
            body_src_lines:new Array(),   //同上，是未折行前的字符串行集

            body_message:"",  //邮件的消息体，不一定有这个，因为 body 可能是一个 tree 结构

        };

    },//

    //新建一个解码结果子 item
    create_new_item : function(){

    },//

    //加载字符串形式的邮件源码并解码
    LoadMail_String : function(filePath){

    
        return "";
    
    },//

    test1:function() {

        //var s = mail_error.err1;

        var s = mail_src;  //从引擎中传入的
        s = String(mail_src);     //传入的是 golang 的 string 所以要转换成 js 的 string 后才有  s.split 函数可以调用。否则会报 "TypeError: 'split' is not a function"
        ShowMessage("403:" + s);
        ShowMessage("404:" + typeof(s));  //可以看到 js 自身的会显示 "string" 而 golang otto 传递过来的会显示 "object"

        //----

        var lines = s.split("\n");

        ShowMessage("409:" + s);

        var head = new Array();
        var body = new Array();
        var is_body = 0;  //是否已经到 body 行了,head 与 body 之间由一个空行分隔

        for (var i = 0; i < lines.length; i++) {
            var line = lines[i];
            
            //ShowMessage(line);

            if ((0 == is_body)&&(line.trim().length<1)) { is_body = 1; continue; }; //空行特殊处理,说明是 head 与 body 的分隔行 //要注意的是只处理第一个空行，后面的并不是分隔行

            if (0 == is_body) {

                head.push(line);
            }else{
                body.push(line);
            }//if 1

        }//for 1


        //将可能折行过的文本行再恢复
        head = MailMime_srcLines2lines(head);
        //body = MailMime_srcLines2lines(body);  //body 目前是不用这样处理的

        //如果是 quoted-printable 的 body 它的折行规则是不同的，不是前面为空格，而是最后一个字符为 "=" 号

        var mail = this.create_new_decode();

        mail.head_lines = head;
        mail.body_src_lines = body;

        //head 简单解码为 map
        mail.head_map = MailMime_DecodeLines2Map(head);

        //判断 Content-Transfer-Encoding: quoted-printable 编码
        var ContentTransferEncoding = mail.head_map["Content-Transfer-Encoding"];
        if (ContentTransferEncoding == "quoted-printable") {

            ShowMessage("quoted-printable 编码!");

            mail.body_message = MailMime_Decode_quoted_printable(mail.body_src_lines);

        }//if
        
        ShowMessage("mail.body_message: \r\n" + mail.body_message);


        //----
        result = mail_src + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" + mail.body_message;  //ok
        result = mail.body_message;

    },//
    
    
    //替换字符串
    str_replace:function (s, old_s, new_s) {
    
        //s = replace(regexp/substr,replacement)
        //s = s.replace("px", ""); //ok 但不通用
    
        //"".replace(old_s, new_s)
    
        return s.replace(old_s, new_s);
    
    }//
    
    
    
};

MailMime.test1();

//----------------------------------------------------------------

    return result;
    // return "aaa";

}//function DecodeMail_forString
//----------------------------------------------------------------




    
////var mime = MailMime;


//module.exports = { mime, MailMime }; //{hello,hello2}

//return mime;
var module = {};
////module.exports = { mime:mime, MailMime:MailMime };
module.exports = {DecodeMail_forString:DecodeMail_forString };



