package javatojs;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.*;
import android.net.*;

//安卓不能用 ScriptEngineManager

//import javax.script.*;
//import javax.script.ScriptEngineManager;
//import javax.script.ScriptException; //直接调用js代码
//rhino
//https://github.com/mozilla/rhino
//https://www.jianshu.com/p/048229e8d59b
//安卓中目前是无法使用 ScriptEngineManager 的。所以要用第三方库。
import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.Scriptable;

//不过，如果只是单 webview 的项目根本不需要用 js 引擎

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

    //android 中运行一段脚本
    static public void RunJS(String js_src)
    {
        //https://blog.csdn.net/kael_wyh/article/details/125918352

        try{

            org.mozilla.javascript.Context _rhino = org.mozilla.javascript.Context.enter();
            Context vm = _rhino;

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

            _rhino.setOptimizationLevel(-1);
            Scriptable _scope = _rhino.initStandardObjects(); //是什么？

            //--------

            //--------

            vm.evaluateString(_scope, " alert('');", null, 1, null);

            /* 这里是测试执行代码
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




    }//func

    //--------------------------------------------------------
    //应该是调用一个 js 函数
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