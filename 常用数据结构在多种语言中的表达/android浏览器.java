
//象如何在浏览器中实现 alert 这样的


import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.*;
import android.net.*;
// import javatojs.*;

//单实例的 webview 程序

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //--------------------------------------------------------
        web1 = findViewById(R.id.web1);

        this.InitWebViewForAndroid(web1);

        //应该是要放到打开页面之前
        java2js_webview.InitJSFunctions(web1, this);
        //java2js.RunJS("");

        //web1.loadUrl("https://www.baidu.com");

        //1.
        //当使用webview加载网页时我们会遇到网页加载出错，出现NET::ERR_CACHE_MISS的错误，
        // 解决方法只需要在你工程的AndroidManifest.xml文件中加入联网的权限

        //2.
        //net:ERR_CLEARTEXT_NOT_PERMITTED错误四种解决办法

        //--------------------------------------------------------
        //加载 assets 目录下的网页(待补) //对应源码的 main/assets 目录，新建立的项目可能没有这个目录，手工创建就可以了，至少当前版本不用写什么配置就可以用了
        web1.loadUrl("file:///android_asset/html/index.html");

        //应该是要放到打开页面之前
        //java2js_webview.InitJSFunctions(web1, this);
        //java2js.RunJS("");

    }//

    public WebView web1 = null;

    //初始化浏览器
    public void InitWebViewForAndroid(WebView webView)
    {
        webView.setWebViewClient(new WebViewClient() {

            //--------------------------------------------------------
            //试图解决 W/cr_AwContentsClient: Denied starting an intent without a user gesture, URI http://网址.html。
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                return false;
            }//

            //--------------------------------------------------------
            //试图解决 net:ERR_UNKNOWN_URL_SCHEME ，比如打开 baidu 时会想打开 app
            //我们这里一律禁止跳转好了
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
                String url = request.getUrl().toString();

                if(url.startsWith("http:") || url.startsWith("https:") ) {
                    view.loadUrl(url);
                    return false;
                }else{
                    //Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
                    //startActivity(intent);
                    return true;
                }
            }//

            //--------------------------------------------------------

        });

        //--------------------------------------------------------
        //--------------------------------------------------------
        //访问的页面中有Javascript，则webview必须设置支持Javascript
        WebSettings mWebSettings = webView.getSettings();
        mWebSettings.setJavaScriptEnabled(true);

        //--------------------------------------------------------
        //前提条件：
        //Android4.4+
        //1.在APP中启用 WebView 调试，开启调试后，Chrome DevTools才能对WebView进行远程调试；
        //https://www.cnblogs.com/wmhuang/p/7396150.html
        //1.在APP中启用 WebView 调试，开启调试后，Chrome DevTools才能对WebView进行远程调试；
        //1
        //WebView.setWebContentsDebuggingEnabled(true);　
        //2.通过访问chrome://inspect/#devices访问已启用调试的 WebView 列表；
        //那正式发布的时候应该取消
        webView.setWebContentsDebuggingEnabled(true);


        //--------------------------------------------------------

    }//

}//


//--------------------------------------------------------
/*
跨域的问题
https://blog.csdn.net/qq_28026283/article/details/108277210
https://blog.csdn.net/qq_32176125/article/details/75393783?


android——webview开发：跨域请求问题的解决方式之一
二分之一人生
于 2017-07-19 15:47:00 发布 13793
收藏 2
分类专栏： android 文章标签： webview 跨域请求 android开发
版权
android 专栏收录该内容
3 篇文章 0 订阅
订阅专栏

webview是android开发中常用的一个控件，它可以加载本地的html文件，也可以直接加载一个已存在的有效网络连接，但是当我们加载本地的html时，通过本地的html去加载不同域名的网页是就可能会发生跨域请求问题，出现这个问题可以用js的方式解决也可以用java的方式解决：

提示错误：No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'null' is therefore not allowed access.

下面在提供的解决方式：

在loadurl（“有效链接”）方法之前，添加这几行代码即可解决：代码如下

try {
    if (Build.VERSION.SDK_INT >= 16) {
        Class<?> clazz = webView.getSettings().getClass();
        Method method = clazz.getMethod(
                "setAllowUniversalAccessFromFileURLs", boolean.class);//利用反射机制去修改设置对象
        if (method != null) {
            method.invoke(webView.getSettings(), true);//修改设置
        }
    }
} catch (IllegalArgumentException e) {
    e.printStackTrace();
} catch (NoSuchMethodException e) {
    e.printStackTrace();
} catch (IllegalAccessException e) {
    e.printStackTrace();
} catch (InvocationTargetException e) {
    e.printStackTrace();
}

ok，问题解决了

--------------------------------------------------------
--------------------------------------------------------

Android webview跨域问题解决小记
大白大白大白
于 2020-08-28 12:10:42 发布 4750
收藏 4
分类专栏： android 文章标签： webview android跨域
版权
android 专栏收录该内容
11 篇文章 0 订阅
订阅专栏

android开发中，webview常用于显示网页或h5页面，一个遇到跨域方面的坑。

但是在android上在api 23之前，是可以跨域读取cookie的，比如A域写入一个userId的cookie，B域可以读取该值。但是在23时，系统将该值设置成了false，不再让跨域读取了。如果你的应用有跨域读取需求，怎么办？可以采用如下方式进行开启：

//设置跨域cookie读取

    public final void setAcceptThirdPartyCookies() {

//target 23 default false, so manual set true

        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) { CookieManager.getInstance().setAcceptThirdPartyCookies(webView,true);

        }

    }

2.http/https混合加载

        在现阶段，很多网站都改成了https进行访问，https可以提升访问网站的安全性，防止信息被窃取，如果所有的网页都是https且网页内的链接也是都是https，那就没有混合加载(文本区域https，图片文件http加载)的问题了。但是很多资源现阶段还没有改变成https访问，往往页面都嵌入了http的链接。这种混合网页如果不进行处理，直接加载是会出现错误的。怎么解决这个问题？

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {

        webSettings.setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);

        }

        这也是一个分版本的函数，在api23之前，默认是可以混合加载的，但是在23时，默认值改成了MIXED_CONTENT_NEVER_ALLOW，因此如果你有混合加载的需求，设置setMixedContentMode为MIXED_CONTENT_ALWAYS_ALLOW。

        3.无法解决跨域访问问题，可以尝试给webview设置如下配置，已解决该问题；

        if (Build.VERSION.SDK_INT >= 16) {

        Class clazz =webView.getSettings().getClass();

        Method method = clazz.getMethod(“setAllowUniversalAccessFromFileURLs”, boolean.class);

        if (method != null) {

        method.invoke(webView.getSettings(), true);

        }

        }

        } catch (IllegalArgumentExceptione) {

        e.printStackTrace();

        } catch (NoSuchMethodException e) {

        e.printStackTrace();

        } catch (IllegalAccessException e) {

        e.printStackTrace();

        } catch (InvocationTargetException e) {

        e.printStackTrace();

        }









*/




