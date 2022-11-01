package javatojs;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.*;
import android.net.*;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;


//使用 webview 的情况下，给 webview 中的 js 调用的函数

public class java2js_webview {

    //webview调用js
    static public void RunJS(WebView web, String js_src)
    {
        ////mWebView.loadUrl("javascript:do()");

        web.loadUrl("javascript:" + js_src);

        //这个应该更强
        //web.evaluateJavascript();

    }//

    static public void RunJS_GetResult(WebView web, String js_src)
    {
        ////mWebView.loadUrl("javascript:do()");

        //web.loadUrl("javascript:" + js_src);


        //这个应该更强
        web.evaluateJavascript(js_src, new ValueCallback<String>() {
            @Override
            public void onReceiveValue(String value) {
                //Log.i(“datass”,value);
            }
        });

    }//

    //注入到 js 中的变量所在的类
    static public class JSObject {// sdk17版本以上加上注解

        //直接这样写在 js 中并不能被调用
        public String namestring="namestring";

        public WebView web = null;  //所属的 WebView

        public JSObject(WebView webview)
        {
            super();
            this.web = webview;
        }//

        //可供 js 调用的函数
        @JavascriptInterface
        public String getData(String txt) {

            return "12345678";

        }

        // sdk17版本以上加上注解
        @JavascriptInterface
        public void getClose() {

            //Toast.makeText(MainActivity.this, "dododo", Toast.LENGTH_SHORT)
            //        .show();

            // finish();

        }//

        //主要使用 json_param，而 bin_param 是用在传输大的二进制或者非常大的字符串时
        //据说一定要用 final 函数
        // sdk17版本以上加上注解
        @JavascriptInterface
        public String RunJavaFunc(final String func_name, final String json_param, final String bin_param)
        {
            if ("ShowMessage".equals(func_name)){

                Context this_ = web.getContext();

                //自动消息的圣诞框
                //Toast.makeText(web.getContext(), bin_param, Toast.LENGTH_SHORT).show();

                new AlertDialog.Builder(this_)
                        .setTitle("")
                        .setMessage(bin_param)
                        .setPositiveButton("OK", null)
                        .show();


                return "";
            }

            return "RunJava() result";
        }//

    }//class

    //js 调用 java //使用类似于 php 的 c 语言接口，用一个函数来完成全部的功能，由这个接口函数再去调用各自具体的函数
    static public void InitJSFunctions(WebView web, Activity this_)
    {
//        web.addJavascriptInterface(
//                new Object()
//                {
//
//                    //据说一定要用 final 函数
//                    public void RunJava(final String json_param, final String bin_param)
//                    {
//
//
//                    }
//
//                }
//                ,"RunJava1");

        web.addJavascriptInterface(new JSObject(web),"RunJava");
        //web.addJavascriptInterface(new JSObject(),"obj");

    }//

}//


