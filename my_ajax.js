/// <reference path="jquery-1.11.3.min.js" />
/// <reference path="panel.js" />

//https://www.cnblogs.com/kismet82/p/5479390.html
//js 中显示 jquery 提示的方法

//jquery 的 ajax 其实有点啰嗦，其实可以很简单的定义出常用的功能
//不过微信小程序的和它语法也差不多，所以当做一个使用示例也可以

//取文件的最后修改时间
//直接参考 showbbs.html 
//function loadXMLDoc(url) {
function url_GetLastModified(url, cookie) {
    var xmlhttp;
    var last = null;
    if (window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function () {
        //alert("aaa");
        //alert(xmlhttp.getResponseHeader('Last-Modified'));
        //if (xmlhttp.readyState == 4 && xmlhttp.status == 200) 
        if (xmlhttp.readyState == 4 && xmlhttp.status != 404) //判断 404 就可以了,因为没变化的话是 304
        {
            //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified');
            //var last = xmlhttp.getResponseHeader('Last-Modified');

        }
    }
    //xmlhttp.open("GET", url, true);
    //xmlhttp.open("HEAD", url, true); //默认异步方式
    xmlhttp.open("HEAD", url, false); //默认异步方式//我们要同步方式

    if (true == cookie) {
        xmlhttp.withCredentials = true; //2020 //所说这样才可写入跨域的 cookie //参考 https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/withCredentials
        //似乎是打开后才可设置
        //有这个，服务器的 cookie 才会写入客户端，不过这要求服务器能动态设置 "Access-Control-Allow-Origin" ，所以并不适合所有的资源
    
    }//if 1 

    xmlhttp.send();

    //--------------------------------------------------
    //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified'); //同步的情况下不用判断 readyState,直接 send 后取就行了

    return xmlhttp.getResponseHeader('Last-Modified'); //oss 里有这个标志

} //

//----------------------------------------------
//原理可参考 http://www.w3school.com.cn/xmldom/dom_http.asp


//----------------------------------------------


//常用的 get 请求//非异步
function url_get(url, cookie) {
    var xmlhttp;
    var last = null;
    if (window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function () {
        //alert("aaa");
        //alert(xmlhttp.getResponseHeader('Last-Modified'));
        //if (xmlhttp.readyState == 4 && xmlhttp.status == 200) 
        if (xmlhttp.readyState == 4 && xmlhttp.status != 404) //判断 404 就可以了,因为没变化的话是 304
        {
            //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified');
            //var last = xmlhttp.getResponseHeader('Last-Modified');

        }
    }
    //xmlhttp.open("GET", url, true);
    //xmlhttp.open("HEAD", url, true); //默认异步方式
    //xmlhttp.open("HEAD", url, false); //默认异步方式//我们要同步方式
    xmlhttp.open("GET", url, false); //默认异步方式//我们要同步方式

    if (true == cookie) {
        xmlhttp.withCredentials = true; //2020 //所说这样才可写入跨域的 cookie //参考 https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/withCredentials
        //似乎是打开后才可设置
        //有这个，服务器的 cookie 才会写入客户端，不过这要求服务器能动态设置 "Access-Control-Allow-Origin" ，所以并不适合所有的资源
    
    }//if 1 

    xmlhttp.send();

    //--------------------------------------------------
    //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified'); //同步的情况下不用判断 readyState,直接 send 后取就行了

    //return xmlhttp.getResponseHeader('Last-Modified'); //oss 里有这个标志
    return xmlhttp.responseText;

} //

//常用的 get 请求//异步
//function url_get_async(url, func, cookie = false) { //es6 才支持默认参数
function url_get_async(url, func, cookie) {
    var xmlhttp;
    var last = null;
    if (window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function () {
        //alert("aaa");
        //alert(xmlhttp.getResponseHeader('Last-Modified'));
        //if (xmlhttp.readyState == 4 && xmlhttp.status == 200) 
        if (xmlhttp.readyState == 4 && xmlhttp.status != 404) //判断 404 就可以了,因为没变化的话是 304
        {
            //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified');
            //var last = xmlhttp.getResponseHeader('Last-Modified');
            var s = xmlhttp.responseText;
            func(s); //返回的是字符串,如果要 json 自己转换,如果要 xml 其实可以有 xmlhttp.responseXML ,不过自己转换更好一点,可以检测出服务回应中的错误
        }
    }
    //xmlhttp.open("GET", url, true);
    //xmlhttp.open("HEAD", url, true); //默认异步方式
    //xmlhttp.open("HEAD", url, false); //默认异步方式//我们要同步方式
    //xmlhttp.open("GET", url, false); //默认异步方式//我们要同步方式
    xmlhttp.open("GET", url, true); //默认异步方式//我们要异步方式

    if (true == cookie) {
        xmlhttp.withCredentials = true; //2020 //所说这样才可写入跨域的 cookie //参考 https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/withCredentials
        //似乎是打开后才可设置
        //有这个，服务器的 cookie 才会写入客户端，不过这要求服务器能动态设置 "Access-Control-Allow-Origin" ，所以并不适合所有的资源
    
    }//if 1 

    xmlhttp.send();

    //--------------------------------------------------
    //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified'); //同步的情况下不用判断 readyState,直接 send 后取就行了

    //return xmlhttp.getResponseHeader('Last-Modified'); //oss 里有这个标志
    //return xmlhttp.responseText;

} //

//常用的 post 请求//异步 //传递进来的参数要分开使用 function_httpEncode 进行编码
function url_post_async(url, postData, func, func_error, cookie) {
    var xmlhttp;
    var last = null;
    if (window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function () {
        //alert("aaa");
        //alert(xmlhttp.getResponseHeader('Last-Modified'));
        //if (xmlhttp.readyState == 4 && xmlhttp.status == 200) 
        if (xmlhttp.readyState == 4 && xmlhttp.status != 404) //判断 404 就可以了,因为没变化的话是 304
        {
            //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified');
            //var last = xmlhttp.getResponseHeader('Last-Modified');
            var s = xmlhttp.responseText;
            func(s); //返回的是字符串,如果要 json 自己转换,如果要 xml 其实可以有 xmlhttp.responseXML ,不过自己转换更好一点,可以检测出服务回应中的错误
        }
    }

    //-------------------------
    //有个 XMLHttpRequest.upload 属性可以绑定更多的事件来显示上传的进度

    //xhr.xmlhttp = uploadFailed; //请求失败

    //xmlhttp.onerror = function (evt) { } //这样写是可以的，访问超时时会响应
    //写成 xmlhttp.upload.onerror 也是可以的

    //xmlhttp.upload.onerror =  function (evt) {
    //    _alert("上传失败！");
    //}
    if (xmlhttp.upload) xmlhttp.upload.onerror = func_error;

    //-------------------------
    //xmlhttp.open("GET", url, true);
    //xmlhttp.open("HEAD", url, true); //默认异步方式
    //xmlhttp.open("HEAD", url, false); //默认异步方式//我们要同步方式
    //xmlhttp.open("GET", url, false); //默认异步方式//我们要同步方式
    xmlhttp.open("POST", url, true); //默认异步方式//我们要异步方式

    //-------------------------
    //post 的请求其实没有那么简单

    //有些代码里有这个,但是恐怕不一定对,因为 js 中很可能是 utf8 字符串,和真实的字节长度恐怕是不一样的
    //所以如果不会计算宁可不要这个参数//实在要用,可以参考 function_sizeof() 自定义函数
    //xmlhttp.setRequestHeader("Content-Length",arg.lenght);     
    //用POST的时候一定要有这句
    xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded;");  //open 后才能设置

    //-------------------------

    if (true == cookie) {
        xmlhttp.withCredentials = true; //2020 //据说这样才可写入跨域的 cookie //参考 https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/withCredentials
        //似乎是打开后才可设置
        //有这个，服务器的 cookie 才会写入客户端，不过这要求服务器能动态设置 "Access-Control-Allow-Origin" ，所以并不适合所有的资源
    
    }//if 1 

    xmlhttp.send(postData); //传递进来的参数要分开使用 function_httpEncode 进行编码

    //--------------------------------------------------
    //document.getElementById('p1').innerHTML = "Last modified: " + xmlhttp.getResponseHeader('Last-Modified'); //同步的情况下不用判断 readyState,直接 send 后取就行了

    //return xmlhttp.getResponseHeader('Last-Modified'); //oss 里有这个标志
    //return xmlhttp.responseText;

} //


/*
//上传文件方法
//参考 https://www.cnblogs.com/tianyuchen/p/5594641.html
function UpladFile() {
    var fileObj = document.getElementById("file").files[0]; // js 获取文件对象
    var url = "uploadFile"; // 接收上传文件的后台地址 

    var form = new FormData(); // FormData 对象
    form.append("mf", fileObj); // 文件对象

    xhr = new XMLHttpRequest();  // XMLHttpRequest 对象
    xhr.open("post", url, true); //post方式，url为服务器请求地址，true 该参数规定请求是否异步处理。
    xhr.onload = uploadComplete; //请求完成
    xhr.onerror = uploadFailed; //请求失败
    xhr.upload.onprogress = progressFunction;//【上传进度调用方法实现】
    xhr.upload.onloadstart = function () {//上传开始执行方法
        ot = new Date().getTime();   //设置上传开始时间
        oloaded = 0;//设置上传开始时，以上传的文件大小为0
    };
    xhr.send(form); //开始上传，发送form数据
}//
*/

/*
//可用的事件列表(浏览器支持不详，自己测试吧)，参考 https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/upload
XMLHttpRequest.upload 属性返回一个 XMLHttpRequestUpload对象，用来表示上传的进度。这个对象是不透明的，但是作为一个XMLHttpRequestEventTarget，可以通过对其绑定事件来追踪它的进度。

可以被绑定在upload对象上的事件监听器如下：
事件 	相应属性的信息类型
onloadstart 	获取开始
onprogress 	数据传输进行中
onabort 	获取操作终止
onerror 	获取失败
onload 	获取成功
ontimeout 	获取操作在用户规定的时间内未完成
onloadend 	获取完成（不论成功与否）
 
*/


//----------------------------------
//以下可当作 jquery 中的 ajax 调用的使用示例//jquery 的 ajax 其实没有原生的 XMLHttpRequest 丰富，例如进度什么的就没有，感觉还不如用原生的好了

