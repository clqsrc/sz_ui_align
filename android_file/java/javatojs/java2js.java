package javatojs;

import androidx.appcompat.app.AppCompatActivity;
import hpublic.Functions;

import android.content.Intent;
import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.*;
import android.net.*;


//import javax.script.*;
//import javax.script.ScriptEngineManager;
//import javax.script.ScriptException; //????js??
//rhino
//https://github.com/mozilla/rhino
//https://www.jianshu.com/p/048229e8d59b

import com.faendir.rhino_android.RhinoAndroidHelper;

import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.ImporterTopLevel;
import org.mozilla.javascript.Scriptable;

import javax.lang.model.SourceVersion;

//对应的 rhino-engine-1.7.14.jar  rhino-runtime-1.7.14.jar 在安卓下是用不了的
//import org.mozilla.javascript.engine.RhinoScriptEngine;
//import org.mozilla.javascript.engine.RhinoScriptEngineFactory;
//import org.mozilla.javascript.engine.*;

//import javax.script.ScriptEngine;
//import javax.script.ScriptEngineFactory;
import org.mozilla.javascript.Context;

import java.io.InputStream;
import java.io.InputStreamReader;

//import org.mozilla.javascript.*;

/*

当前版本 rhino-1.7.14.jar 其实是不能直接在 andrid 直接使用的。表现了当 js 脚本中有 try catch 异常时会造成整个 app 崩溃。
造成这种现象主要有两个原因: 1 是 javax.lang.modelSourceVersion 在安卓环境下不存在; 2 是 android 的多语言包位置和普通 java 的并不同。
同时修改过程中还要修改一些其他的代码，不过修改内容都是很小及整体很清晰的，所以风险基本上没有。大家也可以自己动手修改。

1.
自己新建一个假的
javax.lang.modelSourceVersion
类就可以了。原理是返回一个低版本值，rhino 就不会理会它了。

2.
语言包查找不到也会崩溃，其实 java 语言包必定会调用 ResourceBundle.getBundle 所以找到它修改成合适的路径就可以了。
原理是 android 的 properties 文件必须放到 assets 目录下，否则它是不会编译到 apk 文件中去的。
参考 https://cloud.tencent.com/developer/ask/sof/100160004
“
您需要确保属性文件可以写入.apk文件。您最好的选择可能是res/raw/或assets/。另请参阅PropertyResourceBundle的构造函数(因为很容易获得InputStream)。
”
有些人说要弄一个 messages_zh_CN.properties ，这样专门的中文包，一想就不对，难道别的语言环境也要自己弄？
查阅资料后可以知道只要有 messages.properties 至少就能出现英文提示了。其实我个人觉得 rhino 这里过度设计了，没有必要使用 java 的言语包机制。

3.
网上流传的那个 https://github.com/F43nd1r/rhino-android 版本也解决不了这个 js 中 try catch 异常的问题，而且它还得下载
https://android.googlesource.com/platform/dalvik
https://github.com/aosp-mirror/platform_dalvik
处的代码。非常夸张。不过可以保留作为参考

4.最后的解决办法是下载 rhino 的源码，自己编译。

编译过程中 engine 目录下的文件是编译不了的，对比了 rhino-1.7.14.jar 解压后的文件后可以知道实际上也没有包含它们，
所以可以放心地先删除掉。（它们应该是在 rhino-engine-1.7.14.jar  rhino-runtime-1.7.14.jar 文件中。这两个文件
项目组单独列出来的目的是什么？不太清楚，项目文档太多，有兴趣的朋友可以自己研究。）

5.
messages.properties 文件记得放到安卓的 assets 根目录下。
其实相关的 ResourceBundle.getBundle 处的代码很简单，可以自己修改一下，不依赖这个语言包。

6.
assets\html 下的 main_error.js 是一个触发这种情况的简单 js 脚本。可以用来验证一下是否修改成功。

其他代码，整个复制到 android 中的 src 目录下就可以了。建议不用编译成 jar ，因为整个引擎代码不过几百 k ，还是比较小巧的。

*/

//public class java2js extends AppCompatActivity
public class java2js //extends AppCompatActivity
{

//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//
//
//
//    }//

    static public class JSVM
    {
        public Context context;
        public Scriptable scriptable;

    }//

    static public RhinoAndroidHelper rhinoAndroidHelper;

    //android ???????
    static public JSVM RunJS(String js_src)
    {
        JSVM jsvm = new JSVM();

        //https://blog.csdn.net/kael_wyh/article/details/125918352

        try{

            //--------------------------------------------------------
            //安卓下用不了
            //RhinoScriptEngineFactory engineManager = new RhinoScriptEngineFactory();

            //engineManager.registerEngineExtension("js", new RhinoScriptEngineFactory());

            //RhinoScriptEngine engine = engineManager.getEngineByName("js");
            //RhinoScriptEngine engine = (RhinoScriptEngine)engineManager.getScriptEngine();
            //engineManager.getScriptEngine();

            //InputStream ins = getClass().getResourceAsStream("/safeauth.js");
            //InputStreamReader insReader = new InputStreamReader(ins);
            //engine.eval(insReader);

            //engine.eval("");

            //--------------------------------------------------------
            /*
            rhinoAndroidHelper = new RhinoAndroidHelper(Functions.Functions_cur_this);
            Context context = rhinoAndroidHelper.enterContext();
            context.setOptimizationLevel(1);
            Scriptable scope = new ImporterTopLevel(context);

            //--
            org.mozilla.javascript.Context _rhino = context;
            Context vm = _rhino;
            Scriptable _scope = scope;

            jsvm.scriptable = _scope;
            */
            //--------------------------------------------------------

            ///*
            // android 下会触发 java.lang.ClassNotFoundException: Didn't find class "javax.lang.model.SourceVersion"
            org.mozilla.javascript.Context _rhino = org.mozilla.javascript.Context.enter();
            //new RhinoAndroidHelper(...).enterContext()；
            new RhinoAndroidHelper(Functions.Functions_cur_this).enterContext();
            Context vm = _rhino;

            jsvm.context = _rhino;
            //*/


            /*
            if( _rhino==null )
            {
                synchronized (_lock)
                {
                    if( _rhino==null )
                    {
                        _rhino = Context.enter();
                        _rhino.setOptimizationLevel(-1);
                        _scope = _rhino.initStandardObjects();
                    }
                }
            }
            */

            //据说是禁止优化，因为会有问题
            ///*
            _rhino.setOptimizationLevel(-1);
            Scriptable _scope = _rhino.initStandardObjects(); //据说是使 String 这些能用
            //Scriptable _scope = _rhino.initSafeStandardObjects(); //换这个也不行

            jsvm.scriptable = _scope;
            //*/

            //--------

            //--------

            ////vm.evaluateString(_scope, " alert('');", null, 1, null);
            vm.evaluateString(_scope, js_src, null, 1, null);
            //vm.evaluateString(null, js_src, null, 1, null);

            /* ?????????
            String js = "function aa(){return 'sdfsdfsdf';}";
            initScript(js);

            js = "function bb(){return '11111';}";
            initScript(js);

            Object obj = callFunc("aa");
            System.out.println(obj);

            obj = callFunc("bb");
            System.out.println(obj);*/


        }catch(Exception e){
            e.printStackTrace();
        }//


        return jsvm;

    }//func

    //--------------------------------------------------------
    //应该是调用 js 函数
    //public static Object callFunc(String functionName, Object... functionParams)
    public static Object callFunc(org.mozilla.javascript.Context _rhino, Scriptable _scope, String functionName, Object... functionParams)
    {
        try
        {
            Function function = (Function) _scope.get(functionName, _scope);
            if( function != null )
                return function.call(_rhino, _scope, _scope, functionParams);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }//


    //--------------------------------------------------------

}//


//https://blog.csdn.net/weixin_26913055/article/details/114090861
/*


public class MainActivity extends Activity {

@Override

protected void onCreate(Bundle savedInstanceState) {

super.onCreate(savedInstanceState);

setContentView(R.layout.activity_main);

TextView text1 = (TextView) findViewById(android.R.id.text1);

TextView text2 = (TextView) findViewById(android.R.id.text2);

text1.setText(runScript(JAVA_CALL_JS_FUNCTION, "Test", new String[] {}));

text2.setText(runScript(JS_CALL_JAVA_FUNCTION, "Test", new String[] {}));

}

Java执行js的方法

    private static final String JAVA_CALL_JS_FUNCTION = "function Test(){ return '农民伯伯 java call js Rhino'; }";

    js调用Java中的方法

    private static final String JS_CALL_JAVA_FUNCTION = //

            "var ScriptAPI = java.lang.Class.forName(\"" + MainActivity.class.getName() + "\", true, javaLoader);" + //

                    "var methodRead = ScriptAPI.getMethod(\"jsCallJava\", [java.lang.String]);" + //

                    "function jsCallJava(url) {return methodRead.invoke(null, url);}" + //

                    "function Test(){ return jsCallJava(); }";


     // 执行JS
     //
     // @param js js代码
     // @param functionName js方法名称
     // @param functionParams js方法参数
     // @return


    public String runScript(String js, String functionName, Object[] functionParams) {

        Context rhino = Context.enter();

        rhino.setOptimizationLevel(-1);

        try {

            Scriptable scope = rhino.initStandardObjects();

            ScriptableObject.putProperty(scope, "javaContext", Context.javaToJS(MainActivity.this, scope));

            ScriptableObject.putProperty(scope, "javaLoader", Context.javaToJS(MainActivity.class.getClassLoader(), scope));

            rhino.evaluateString(scope, js, "MainActivity", 1, null);

            Function function = (Function) scope.get(functionName, scope);

            Object result = function.call(rhino, scope, scope, functionParams);

            if (result instanceof String) {

                return (String) result;

            } else if (result instanceof NativeJavaObject) {

                return (String) ((NativeJavaObject) result).getDefaultValue(String.class);

            } else if (result instanceof NativeObject) {

                return (String) ((NativeObject) result).getDefaultValue(String.class);

            }

            return result.toString();//(String) function.call(rhino, scope, scope, functionParams);

        } finally {

            Context.exit();

        }

    }

    public static String jsCallJava(String url) {

        return "农民伯伯 js call Java Rhino";

    }

}


*/

//--------------------------------------------------------
//安卓下不同
//http://www.cncsto.com/article/209971
//https://www.cnblogs.com/muphy/p/15253488.html

/*

 安卓(Android)或者Java使用rhino实现任意字符串表达式计算

安卓(Android)或者Java使用rhino实现任意字符串表达式计算

本文连接：https://www.cnblogs.com/muphy/p/15253488.html

Java使用rhino

引入依赖包
复制代码

<!-- https://mvnrepository.com/artifact/org.mozilla/rhino -->
<dependency>
    <groupId>org.mozilla</groupId>
    <artifactId>rhino</artifactId>
    <version>1.7.13</version>
</dependency>

复制代码

初始rhino的Context

Context.enter();

安卓系统使用rhino不太一样，安卓使用参考：https://github.com/F43nd1r/rhino-android

引入依赖包

implementation 'com.faendir.rhino:rhino-android:1.6.0'
//implementation group: 'org.mozilla', name: 'rhino', version: '1.7.13'

初始化rhino的Context

//这里参数context使用Android的Context
new RhinoAndroidHelper(context).enterContext()

AndroidContextHolder.java
复制代码

import android.content.Context;

public class AndroidContextHolder {
    private static Context context;

    public static Context getContext() {
        return context;
    }

    public static void setContext(Context context) {
        AndroidContextHolder.context = context;
    }
}

复制代码

CalculateUtils.java
复制代码

//package com.moefon.power.spring.util;

import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.Scriptable;
import org.mozilla.javascript.Undefined;

import java.util.Stack;


 * 将字符串转计算公式
 * Object r1 = evaluate("function f(){var x = 5; y = 455; var m = {'x':x,'y':y}; return x}; var i = f();"); // Undefined
 * Object r2 = evaluate("function f(){var x = 5; y = 455; var m = {'x':x,'y':y}; return x}; var i = f(); i;"); // Integer:5
 * Object r3 = evaluate("var f = function(){var x = 5; var m = {'x':x,'y':55}; return m}; var i = f();", "i"); // NativeObject:{'x':x,'y':y}
 * Object r4 = evaluate("var f = function(v){var x = 5; var m = {'x':x,'y':v}; return m;};", "f", 1); // NativeObject:{'x':x,'y':y}
 * Object r5 = evaluate("var f = function(v){var x = 5; var m = {'x':x,'y':v}; return x + v;};", "f", 1); // Double:5
 *
 * AndroidContextHolder.getContext() 表示安卓启动的时候记得设置activity或者application
 *
 * @author: 若非
 * @date: 2021/9/9 20:43

public class CalculateUtils {


     * 将字符串转计算公式 {@code evaluate(7+ +55 + +5 * (2-5)) }
     *
     * @param js
     * @return: 计算结果
     * @author: 若非
     * @date: 2021/9/9 20:43

    public static <T> T evaluate(String js) {
        Context ctx = new RhinoAndroidHelper(AndroidContextHolder.getContext).enterContext();
        Scriptable scope = ctx.initStandardObjects();    //初始化本地对象
        T result = null;
        try {
            Object o = ctx.evaluateString(scope, js, "", 1, null);
            if (o != Scriptable.NOT_FOUND && !(o instanceof Undefined)) {
                result = (T) o;//执行
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }


     * 执行JS代码
     *
     * @param js
     * @param resultName scope中获取返回参数名称
     * @return: js对象 计算结果
     * @author: 若非
     * @date: 2021/9/9 20:43

    public static <T> T evaluate(String js, String resultName) {
        Context ctx = new RhinoAndroidHelper(AndroidContextHolder.getContext()).enterContext();
        Scriptable scope = ctx.initStandardObjects();    //初始化本地对象
        T result = null;
        try {
            ctx.evaluateString(scope, js, "", 1, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
        Object o = scope.get(resultName, scope);
        if (o != Scriptable.NOT_FOUND && !(o instanceof Undefined)) {
            result = (T) o;//执行
        }
        return result;
    }


     * 执行JS代码
     *
     * @param js             js代码
     * @param functionName   js方法名称
     * @param functionParams js方法参数
     * @return js对象
     * @author: 若非
     * @date: 2021/9/9 20:43

    public static <T> T evaluate(String js, String functionName, Object... functionParams) {
        Context ctx = new RhinoAndroidHelper(AndroidContextHolder.getContext()).enterContext();
        ctx.setOptimizationLevel(-1);
        T result = null;
        try {
            Scriptable scope = ctx.initStandardObjects();
            ctx.evaluateString(scope, js, null, 1, null);
            Function function = (Function) scope.get(functionName, scope);
            Object o = function.call(ctx, scope, scope, functionParams);
            if (o != Scriptable.NOT_FOUND && !(o instanceof Undefined)) {
                result = (T) o;//执行
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }


     * 加减乘除计算 使用包装类方便取值
     *
     * @param expression
     * @return
     * @author: 若非
     * @date: 2021/9/9 20:43
     * @see this#evaluate
     * @deprecated {@code this.evaluate}

    @Remind(deprecated = true, notice = "请使用evaluate方法来进行表达式计算，这里只支持+-* / ")
    public static Double calculationResult(String expression) {
        char[] chars = expression.toCharArray();
        Stack<String> stack = new Stack<>();
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < chars.length; i++) {
            switch (chars[i]) {
                case '+':
                case '-':
                    stack.push(stringBuilder.toString());
                    stack.push(String.valueOf(calculationResult(stack)));
                    stack.push(chars[i] + "");
                    stringBuilder = new StringBuilder();
                    break;
                case '*':
                case '/':
                    stack.push(stringBuilder.toString());
                    stack.push(chars[i] + "");
                    stringBuilder = new StringBuilder();
                    break;
                default:
                    stringBuilder.append(chars[i]);
            }
        }
        stack.push(stringBuilder.toString());
        Double v = calculationResult(stack);
        return v;
    }


     * 加减乘除计算 使用包装类方便取值
     *
     * @param stack
     * @return 执行结果
     * @author: 若非
     * @date: 2021/9/9 20:43
     * @see this#evaluate
     * @deprecated {@code this.evaluate}

    private static Double calculationResult(Stack<String> stack) {
        double res = Double.parseDouble(stack.pop());
        while (!stack.empty()) {
            String opt = stack.pop();
            double second = Double.parseDouble(stack.pop());
            res = calculation(res, second, opt.charAt(0));
        }
        return res;
    }


     * 加减乘除计算 使用包装类方便取值
     *
     * @param res
     * @param x
     * @param opt
     * @return 执行结果
     * @author: 若非
     * @date: 2021/9/9 20:43
     * @see this#evaluate
     * @deprecated {@code this.evaluate}

    private static Double calculation(double res, double x, char opt) {
        switch (opt) {
            case '+':
                return res + x;
            case '-':
                return x - res;
            case '*':
                return res * x;
            case '/':
                if (x < 0.0000000001f) {
                    return Double.MAX_VALUE;
                }
                return x / res;
            default:
                return res;
        }
    }
}


*/



//--------------------------------------------------------
/*
//这个的方法差好远，不过参考
        https://www.cnblogs.com/liangblog/p/5956256.html

        package com.my.rhino;

        import java.io.FileReader;
        import java.util.List;

        import javax.script.Bindings;
        import javax.script.Compilable;
        import javax.script.CompiledScript;
        import javax.script.Invocable;
        import javax.script.ScriptContext;
        import javax.script.ScriptEngine;
        import javax.script.ScriptEngineFactory;
        import javax.script.ScriptEngineManager;
        import javax.script.ScriptException;

public class RhinoDemo {

    public static void main(String[] args) {
        // create ScriptEngineManager
        ScriptEngineManager manager = new ScriptEngineManager();

        ScriptEngine engine = manager.getEngineByName("js");
        String script = "print ('hello nm')";
        try {
            engine.eval(script);
            System.out.println();
        } catch (ScriptException e) {
            e.printStackTrace();
        }

        try {
            Double hour = (Double) engine.eval("var date = new Date(); date.getHours();");
            String msg;
            if (hour < 10) {
                msg = "Good morning";
            } else if (hour < 16) {
                msg = "Good afternoon";
            } else if (hour < 20) {
                msg = "Good evening";
            } else {
                msg = "Good night";
            }
            System.out.println(hour);
            System.out.println(msg);
        } catch (ScriptException e) {
            System.err.println(e);
        }

        try {
            FileReader reader = new FileReader("src/com/my/rhino/file.js");
            Double num = (Double) engine.eval(reader);
            System.out.println(num);
            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            engine.put("a", 1);
            engine.put("b", 5);

            Bindings bindings = engine.getBindings(ScriptContext.ENGINE_SCOPE);
            Object a = bindings.get("a");
            Object b = bindings.get("b");
            System.out.println("a = " + a);
            System.out.println("b = " + b);

            FileReader reader = new FileReader("src/com/my/rhino/vars.js");
            Double num = (Double) engine.eval(reader);
            System.out.println(num);
            reader.close();

            reader = new FileReader("src/com/my/rhino/fun.js");
            // engine.eval("function add (a, b) {c = a + b; return c; }");
            engine.eval(reader);
            Invocable jsInvoke = (Invocable) engine;

            Object result1 = jsInvoke.invokeFunction("add", new Object[] { 10, 5 });
            System.out.println(result1);

            Adder adder = jsInvoke.getInterface(Adder.class);
            int result2 = adder.add(10, 5);
            System.out.println(result2);

            Compilable jsCompile = (Compilable) engine;
            CompiledScript vscript = jsCompile.compile("function hi () {print ('ddddd !'); }; hi ();");

            for (int i = 0; i < 5; i++) {
                vscript.eval();
            }

        } catch (Exception e) {
            // TODO: handle exception
        }

        try {
            FileReader reader = new FileReader("src/com/my/rhino/java.js");
            @SuppressWarnings("unchecked")
            List<String> num = (List<String>) engine.eval(reader);
            System.out.println();
            for (String word : num) {
                System.out.print(word);
            }
            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        List<ScriptEngineFactory> factoryList = manager.getEngineFactories();
        for (ScriptEngineFactory factory : factoryList) {
            System.out.println("=====");
            System.out.println(factory.getEngineName());
            System.out.println(factory.getEngineVersion());
            System.out.println(factory.getLanguageName());
            System.out.println(factory.getLanguageVersion());
            System.out.println(factory.getExtensions());
            System.out.println(factory.getMimeTypes());
            System.out.println(factory.getNames());
        }
    }
}

Rhino.java
*/


//--------------------------------------------------------
/*
        https://github.com/F43nd1r/rhino-android
        How to use

        Import via gradle

        compile 'com.faendir.rhino:rhino-android:1.6.0'

        Then, instead of calling

        Context.enter()

        use

        new RhinoAndroidHelper(...).enterContext()

        An example project can be found here.
        dx

        This uses google's dx tool which can be found here - https://android.googlesource.com/platform/dalvik
*/
