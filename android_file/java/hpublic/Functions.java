package hpublic;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.view.animation.*;
//import android.view.animation.*;

import android.graphics.drawable.BitmapDrawable;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Outline;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Bundle;
import android.view.*;
import android.view.Window;
import android.view.animation.TranslateAnimation;
import android.view.animation.*;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import ui_json.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import java.util.Arrays.*;
import java.util.*;

import static android.view.View.TEXT_ALIGNMENT_TEXT_START;
import static android.widget.Toast.LENGTH_SHORT;
import static androidx.core.content.ContextCompat.startActivity;



//html, ios, android 下的统一 ui . 实际上只建立基本的 ui 位置控件和对齐方式，具体的效果仍然由各自的原生代码实现。
//但这样至少实现了 ui 的位置的统一。
//项目中最关键的就是对齐方式极为简单，因为经过多年的 ui 设计发现使用几种简单的对齐方式就能实现绝大多数的 ui 布局。
//完成没有必要在各个 ui 体系中使用各自复杂的布局器，特别是 ios 下的各种约束版本升级后几乎每次升级后都是灾难，使得实际上很多项目都是全硬编码的 ui 。

//另外也集成了各平台下的工具函数集可供参考.

public class Functions {

    //不给默认 Activity 很难和其他框架统一接口，还是放一个吧
    //static public Context g_this = null;
    static public Activity Functions_cur_this = null;  //Activity 才有 startActivityForResult

    static public void ShowMessage(String s, Context _this) {

        if (null == _this)
        {
            _this = Functions_cur_this;
        }

        Toast.makeText(_this, s, LENGTH_SHORT).show();

    }//

    //2022 暂时
    static public void ShowMessage(String s) {

        //Context _this = Functions.LastActivity; //
        try {
            Functions.ShowMessage(s, null);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }//

    //放这里方便 ndk 调用而已 //2022.09
    static public String RunJson(String str_json)
    {
        return HttpDLL.RunJson(str_json);
    }//

    //需要要确认按钮的
    static public void ShowMessage_ok(String s, Context _this)
    {

        if (null == _this)
        {
            _this = Functions_cur_this;
        }

        new AlertDialog.Builder(_this) .setTitle("消息").setMessage(s)
//                .setNegativeButton("确认2",
//                        new DialogInterface.OnClickListener() {
//                            @Override
//                            public void onClick(DialogInterface dialog, int which) {
//                                // TODO Auto-generated method stub
//                                //这里是可以加回调函数的
//                            }
//                        })
                .setPositiveButton("确认",
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                // TODO Auto-generated method stub
                                //这里是可以加回调函数的
                            }
                        }).

                show();

    }//

    static public double StrToFloat(String s) {

        try {

            //ios 能自己去掉 px
            s = s.replaceAll("px", "");

            double r = Double.parseDouble(s);

            return r;
        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return  0;
    }//

    static public int StrToInt(String s) {

        try {
            if (null == s) return 0;
            if (s.length() < 1) return 0;

            int r = Integer.parseInt(s);

            return r;
        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return  0;
    }//

    //保存一个值在手机上
    static public void SetSaveValue(String key, String value)
    {

        try {

            Context _this = Functions_cur_this;
            SharedPreferences shared = _this.getSharedPreferences( "testContextSp", Context.MODE_PRIVATE );
            SharedPreferences.Editor editor = shared.edit();
            //editor.putInt( "age", 28);
            //editor.putBoolean( "isStudent", false );
            //editor.putString( "job", "it" );
            editor.putString(key, value);
            editor.commit();


        }catch (Exception e)
        {
            e.printStackTrace();
        }

    }//

    //取文件名 //即 [/] [\] 最后一节的内容 //中文未测试,以后再改进
    static public String ExtractFileName(String s)
    {
        //String[] r = new String[2]; r[0] = ""; r[1] = "";

        String r = "";

        //char sp
        try{
            char[] arr = s.toCharArray();
            boolean getLeft = false;
            for (int i=arr.length-1; i>=0; i--)
            {
                char c = arr[i];

                if (c == '/' || c == '\\')
                {
                    getLeft = true;
                    break;
                }

                //r = r + c;
                r = c + r;

            }//for


        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return r;

    }//


    //分一行为两部分
    static public String[] StrTo2(String s, char sp)
    {
        String[] r = new String[2]; r[0] = ""; r[1] = "";
        String sl = "";
        String sr = "";
        try{
            char[] arr = s.toCharArray();
            boolean getLeft = false;
            for (int i=0; i<arr.length; i++)
            {
                char c = arr[i];

                if (c == sp)
                {
                    getLeft = true;
                    continue;
                }

                if (false == getLeft) //得到一行了
                {
                    sl = sl + c;
                }else{
                    sr = sr + c;
                }
            }//for

            r[0] = sl;
            r[1] = sr;

        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return r;

    }//

    static public HashMap<String, String> LoadConfig(String fn)
    {
        HashMap<String, String> ls = new HashMap<String, String>();

        try{
            String s = LoadStringFromFile_assets(fn);

            boolean getLine = false;
            String sLine = "";

//            for (int i=0; i<s.length(); i++)
//            {
//                //char c = s.charAt(i); //据说这样是错误的，因为 java 的字符串是 unicode 的
//                s.codePointAt(i);
//            }

            char[] arr = s.toCharArray();
            for (int i=0; i<arr.length; i++)
            {
                char c = arr[i];

                if (c == '\r')
                {
                    continue;
                }

                if (c == '\n') //得到一行了
                {

                    String [] kv = StrTo2(sLine, '=');
                    String key = kv[0];
                    String value = kv[1];

                    if (null != key && key.trim().length() > 0){
                        ls.put(key, value);
                    }

                    //要重置
                    getLine = false;
                    sLine = "";
                    continue;
                }

                sLine = sLine + c;
            }//for

        }catch (Exception e)
        {
            e.printStackTrace();
        }



        return ls;
    }//

    //语言函数
    static public HashMap<String, String> gLg = null;
    static public String lg(String key, String cn_ref)
    {

        try {

            Context _this = Functions_cur_this;

            //----
            //先判断语言版本
            if (true != AppFunctions.GAppFunc.AppverIsEn) return cn_ref;

            //----

            if (null == gLg)
            {
                //gLg = [[LanguageConfig alloc] init];
                //[gLg LoadConfig];
                gLg = new HashMap<String, String>();

                gLg = LoadConfig("lg_en.txt");

            }

            //key = [Functions Trim:key];

            //---- 空值不翻译


            //----

            String v = gLg.get(key);

            //NSString * _v = [Functions Trim:v];
            //_v = [Functions Replace:_v s_old:@"\r" s_new:@""];// 奇怪，会有空回车的情况
            //_v = [Functions Replace:_v s_old:@"\n" s_new:@""];// 奇怪，会有空回车的情况

            if (null == v || v.length()<1)
            //if (nil == _v || _v.length<1)
            {
                //v = @"";
                v = key;

                //---- 空值不翻译
                String _cn_ref = cn_ref.trim();
                if (null == _cn_ref || _cn_ref.length()<1) return cn_ref; //说明这只是界面上的空元素，就不要翻译了
                //----


                ShowMessage("未翻译的语言包内容。key:" + key + " 参考中文为:" + cn_ref, Functions_cur_this);
            }//

            return v;

            //return value;

        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return "";
    }//

    //保存一个值在手机上
    static public String GetSaveValue(String key)
    {

        try {

            Context _this = Functions_cur_this;

            SharedPreferences shared = _this.getSharedPreferences( "testContextSp", Context.MODE_PRIVATE );
            SharedPreferences.Editor editor = shared.edit();
            //editor.putInt( "age", 28);
            //editor.putBoolean( "isStudent", false );
            //editor.putString( "job", "it" );
            //editor.putString(key, value);
            //editor.commit();

            String value = shared.getString(key, "");

            return value;

        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return "";
    }//

    //旋转视图 //参数为 视图、旋转角度、持续时间（毫秒）
    static public void View_RotateAni(View view, int angle_360, int millisecond)
    {
        try{

            //angle_360 = 15;

            //应该是旋转动画类
            //RotateAnimation animation = new RotateAnimation(0, angle_360);
//            RotateAnimation animation = new RotateAnimation(0, angle_360,
//                    view.getWidth() / 2, //100.5f, //表示从 x 轴中心旋转
//                    150.5f
//            );

            //用这个参数才会和 ios 的相同效果
            RotateAnimation animation = new RotateAnimation(angle_360, angle_360,
                    view.getWidth() / 2, //100.5f, //表示从 x 轴中心旋转
                    view.getHeight() / 2
            );

            //应该是平移动画类
            //TranslateAnimation animation = new TranslateAnimation(x_from, x_to, 0, 0);

            animation.setInterpolator(new OvershootInterpolator()); //?似乎并不是必要的.应该是表示匀速
            //animation.setDuration(durationMillis);
            ////animation.setDuration((long)(time_second * 1000));
            animation.setDuration((long)millisecond);
            ////on.setFillAfter(true);//动画执行完后是否停留在执行完的状态
            animation.setFillAfter(true);
            //animation.setStartOffset(delayMillis);
            animation.setStartOffset(0);
            view.startAnimation(animation);

            //scrollTo(x,y);
            //scrollBy(x,y);

            //view.scrollBy(1000, 0);


        }catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    //动画显示窗口 //向左移动  //比较简单的，暂时用
    //+ (void)AnimaWindow_toLeft:(UIView*) view
    static public void View_Anima_toLeft(View view, int width, float time_second)
    {
        try{
            float x_from = 0;
            float x_to = 1000;

            width = dp2px(Functions_cur_this, width);
            x_to = 0 - width;

            //https://cloud.tencent.com/developer/article/1623749?from=14588

            TranslateAnimation animation = new TranslateAnimation(x_from, x_to, 0, 0);
            animation.setInterpolator(new OvershootInterpolator()); //?似乎并不是必要的
            //animation.setDuration(durationMillis);
            animation.setDuration((long)(time_second * 1000));
            //animation.setStartOffset(delayMillis);
            animation.setStartOffset(0);
            view.startAnimation(animation);

            //scrollTo(x,y);
            //scrollBy(x,y);

            //view.scrollBy(1000, 0);


        }catch (Exception e)
        {
            e.printStackTrace();
        }

    }//

    //不恢复原状的版本 //单位是 dp
    static int last_View_Anima_Move = 0;
    static public void View_Anima_Move_v1(View view, int left, int top, float time_second)
    {
        try{
            if (1 == last_View_Anima_Move) return;

            last_View_Anima_Move = 1;

            float x_from = 0;
            float x_to = 1000;

            float y_from = 0;
            float y_to = 1000;

            left = dp2px(Functions_cur_this, left);
            top  = dp2px(Functions_cur_this,  top);

            x_to = left;
            y_to = top;

            x_from = view.getLeft();
            y_from = view.getTop();

            //left = dp2px(Functions_cur_this, left); //这个不用再转
            //top  = dp2px(Functions_cur_this,  top);

            //https://cloud.tencent.com/developer/article/1623749?from=14588

            //https://www.jianshu.com/p/b11edfb35445
            //float fromXDelta:这个参数表示动画开始的点离当前View X坐标上的差值；
            //float toXDelta,这个参数表示动画结束的点离当前View X坐标上的差值；
            //float fromYDelta,这个参数表示动画开始的点离当前View Y坐标上的差值；
            //float toYDelta)这个参数表示动画开始的点离当前View Y坐标上的差值；
            //如果view在A(x,y)点 那么动画就是从B点(x+fromXDelta, y+fromYDelta)点移动到C 点(x+toXDelta,y+toYDelta)点.

            //TranslateAnimation animation = new TranslateAnimation(x_from, x_to, y_from, y_to);
            //和 ios 不同，这个是用的差值
            //TranslateAnimation animation = new TranslateAnimation(0, x_from - x_to, 0, y_from - y_to);
            TranslateAnimation animation = new TranslateAnimation(0, x_to - x_from, 0, y_to - y_from);

            animation.setInterpolator(new OvershootInterpolator()); //?似乎并不是必要的
            //animation.setDuration(durationMillis);
            //animation.setDuration((long)(time_second * 1000));  //动画持续的时间
            animation.setDuration((long)(time_second * 500));  //动画持续的时间

            //动画后fillBefore是指动画结束时画面停留在此动画的第一帧; 默认值为true fillAfter是指动画结束是画面停留在此动画的最
            //但这并不会修改 view 的实际位置
            animation.setFillAfter(true);
            animation.setFillEnabled(true);

            final float off_x = x_from - x_to;
            final int _left = left;
            final int _top = top;

            run_delay(Functions_cur_this, 500, new Runnable() {
                @Override
                public void run() {
                    SetFrame(view,
                            _left, //view.getLeft() + (int)off_x, //view.getLeft() + (int)(x_from - x_to),
                            _top, //view.getTop(),
                            view.getWidth(),
                            view.getHeight());

//                    last_View_Anima_Move = 0;
                }
            });

            run_delay(Functions_cur_this, 900, new Runnable() {
                @Override
                public void run() {
//                    SetFrame(view,
//                            _left, //view.getLeft() + (int)off_x, //view.getLeft() + (int)(x_from - x_to),
//                            _top, //view.getTop(),
//                            view.getWidth(),
//                            view.getHeight());

                    last_View_Anima_Move = 0;
                }
            });

            /*
            animation.setAnimationListener(new Animation.AnimationListener()
            {



                @Override
                public void onAnimationEnd(Animation animation)
                {
                    animation.setFillAfter(false);
                    //FrameLayout.LayoutParams par=(LayoutParams)b.getLayoutParams();
                    //par.leftMargin=deltaXToMove;
                    //b.setLayoutParams(par);

                    SetFrame(view,
                            view.getLeft() + (int)off_x, //view.getLeft() + (int)(x_from - x_to),
                            view.getTop(),
                            view.getWidth(),
                            view.getHeight());
                }

                @Override
                public void onAnimationRepeat(Animation arg0) {
                    // TODO Auto-generated method stub

                }

                @Override
                public void onAnimationStart(Animation arg0) {
                    // TODO Auto-generated method stub

                }

            });//
            */


            //animation.setStartOffset(delayMillis);
            animation.setStartOffset(0);
            view.startAnimation(animation);


        }catch (Exception e)
        {
            e.printStackTrace();
        }

    }//


    //安卓的动画是不实际改变位置的，所以我们自己记录一下上次的移动到的位置 //临时
//    static int first = 1;
//    static int last_left = 0;
//    static int last_top = 0;
//    static long last_time = 0; //安卓下不能响应太快，否则两个动画间没有过渡，所以只能隔一段响应一次

    static class View_Anima_Move_Info{
        int first = 1;
        int last_left = 0;
        int last_top = 0;
        long last_time = 0; //安卓下不能响应太快，否则两个动画间没有过渡，所以只能隔一段响应一次

        public View_Anima_Move_Info(){

        }
    }//

    static HashMap<View, View_Anima_Move_Info> View_Anima_Move_List = new HashMap<View, View_Anima_Move_Info>();

    static public void View_Anima_Move(View view, int left, int top, float time_second)
    {
        try{

            //------------------------------------------------
            //取当前视图相关联的信息

            View_Anima_Move_Info info = View_Anima_Move_List.get(view);
            if (null == info)
            {
                info = new View_Anima_Move_Info();
                View_Anima_Move_List.put(view, info);
            }

            //------------------------------------------------
            //安卓版 两个动画之间间隔时间太短会很难看

            long now = System.currentTimeMillis();

            if ((now - info.last_time)<300) return;  //目前 300 到 500 之间是比较好的
            info.last_time = now;

            //------------------------------------------------

            float x_from = 0;
            float x_to = 1000;

            float y_from = 0;
            float y_to = 1000;

            left = dp2px(Functions_cur_this, left);
            top  = dp2px(Functions_cur_this,  top);

            x_to = left;
            y_to = top;

            int fix_left = view.getLeft();  //安卓中这两个值其实是不变的，所谓有动画其实只是影像在动
            int fix_top  = view.getTop();

            if (0 == info.first) //不是第一次就取上次的位置，否则取 view 自己的位置
            {
                x_from = info.last_left;
                y_from = info.last_top;

            }else{
                x_from = fix_left;
                y_from = fix_top;
            }

            info.first = 0;

            info.last_left = (int)x_to;
            info.last_top = (int)y_to;

            //left = dp2px(Functions_cur_this, left); //这个不用再转
            //top  = dp2px(Functions_cur_this,  top);

            //https://cloud.tencent.com/developer/article/1623749?from=14588

            //https://www.jianshu.com/p/b11edfb35445
            //float fromXDelta:这个参数表示动画开始的点离当前View X坐标上的差值；
            //float toXDelta,这个参数表示动画结束的点离当前View X坐标上的差值；
            //float fromYDelta,这个参数表示动画开始的点离当前View Y坐标上的差值；
            //float toYDelta)这个参数表示动画开始的点离当前View Y坐标上的差值；
            //如果view在A(x,y)点 那么动画就是从B点(x+fromXDelta, y+fromYDelta)点移动到C 点(x+toXDelta,y+toYDelta)点.

            //TranslateAnimation animation = new TranslateAnimation(x_from, x_to, y_from, y_to);
            //和 ios 不同，这个是用的差值
            //TranslateAnimation animation = new TranslateAnimation(0, x_from - x_to, 0, y_from - y_to);
            TranslateAnimation animation = new TranslateAnimation(
                    x_from - fix_left,               //起点 x left
                    left - fix_left,   //终点
                    y_from - fix_top,               //起点 y top
                    top - fix_top    //终点
            );

            animation.setInterpolator(new OvershootInterpolator()); //?似乎并不是必要的
            //animation.setDuration(durationMillis);
            animation.setDuration((long)(time_second * 1000));  //动画持续的时间
            //animation.setDuration((long)(time_second * 500));  //动画持续的时间

            //动画后fillBefore是指动画结束时画面停留在此动画的第一帧; 默认值为true fillAfter是指动画结束是画面停留在此动画的最
            //但这并不会修改 view 的实际位置
            animation.setFillAfter(true);
            animation.setFillEnabled(true);

            //animation.setStartOffset(delayMillis);
            animation.setStartOffset(0);
            view.startAnimation(animation);


        }catch (Exception e)
        {
            e.printStackTrace();
        }

    }//


    //参考 ios 的 go(), gomain() 函数对
    static void go(Runnable action)
    {
        new Thread(new Runnable() {
            @Override
            public void run() {
                action.run();
            }
        }).start();
    }//

    static void go_main(Activity _this, Runnable action)
    {
        _this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                action.run();

            }
        });
    }//

    //一秒钟后在 ui 线程中触发
    static void run_delay(Activity _this, int millisecond, Runnable action)
    {
        go(new Runnable() {
            @Override
            public void run() {

                try {
                    //Thread.sleep(1000);
                    Thread.sleep(millisecond);
                }catch (Exception e){
                    e.printStackTrace();
                }

                go_main(_this, new Runnable() {
                    @Override
                    public void run() {
                        action.run();
                    }
                });
            }
        });

    }//

    //参考 uijson 的同名函数
    static byte [] LoadFromFile_assets(String fn, Context context)
    {
        try {

            //fn = "file:///android_asset/web/1_pc_local.html";
            //fn = "/android_asset/web/1_pc_local.html";

            //File file = new File(fn);
            //FileInputStream inStream = new FileInputStream(file);

            //getAssets 的返回值可能也是要关闭的

            InputStream inStream = context.getAssets().open(fn);

            //Log.i("FileTest", readInStream(inStream));
            byte [] buf = new byte[4 * 1024 * 1024];
            int len = inStream.read(buf, 0, 4 * 1024 * 1024); //暂定最大 4 m

            inStream.close();

            if (len<1) return null;

            byte [] _buf = new byte[len];

            //Arrays.copyOfRange()
            //System.arraycopy();
            _buf = Arrays.copyOf(buf, len);

            return _buf;

        }catch (Exception e)
        {
            e.printStackTrace();
        }//try

        return null;
    }//

    static public String LoadStringFromFile_assets(String fn)
    {
        try {

            Context context = Functions_cur_this;

            byte [] buf = LoadFromFile_assets(fn, context);

            if (null == buf) return "";

            String s = new String(buf); //安卓下默认应该是 utf8, pc 应该是 unicode

            return s;

        }catch (Exception e)
        {
            e.printStackTrace();
        }//try

        return "";

    }//

    static public void HideActionBar(AppCompatActivity _this)
    {
        //-------- 隐藏 action bar
        //一定要在 activity 的 onCreate 中调用
        //获取ActionBar
        ActionBar actionBar = _this.getSupportActionBar();
        actionBar.hide();


        //--------
    }//

    //修改状态栏为全透明  //但其中的文字颜色变不了
    //https://www.jianshu.com/p/7d2ddc3b92a7
    @TargetApi(19)
    public static void StatusBarUtil_TransparencyBar(Activity activity)
    {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
        {
            Window window = activity.getWindow();
            window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            window.setStatusBarColor(Color.TRANSPARENT);

        }
        else
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
        {
            Window window =activity.getWindow();
            window.setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS,
                    WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);

        }//if


    }//

    //https://www.jianshu.com/p/b0e523c87ddf
    //【Android】Android R（API 30）中新的状态栏/导航栏隐藏方法
    //现在 real me 手机还用不了。不过可以写着兼容性
    //应该状态栏颜色那些以后也可以用这个修改
    public static void StatusBarUtil_HideNavigationBars_api30(AppCompatActivity activity)
    {
        //现在会报 No virtual method getInsetsController()Landroid/view/WindowInsetsController
        WindowInsetsController ic = activity.getWindow().getInsetsController();
        if (ic != null) {
            ic.hide(WindowInsets.Type.statusBars());
            ic.hide(WindowInsets.Type.navigationBars());
        }
    }//


    //修改状态栏颜色，支持4.4以上版本  //real me ok
    //public static void setStatusBarColor(AppCompatActivity activity,int colorId)
    public static void StatusBarUtil_SetStatusBarColor(AppCompatActivity activity,int color)
    {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            Window window = activity.getWindow();
            //window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            //window.getDecorView().setSystemUiVisibility( View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);

            //window.setStatusBarColor(activity.getResources().getColor(colorId));
            window.setStatusBarColor(color);

        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            //使用SystemBarTint库使4.4版本状态栏变色，需要先将状态栏设置为透明
//            transparencyBar(activity);
//            SystemBarTintManager tintManager = new SystemBarTintManager(activity);
//            tintManager.setStatusBarTintEnabled(true);
//            tintManager.setStatusBarTintResource(colorId);
        }
    }//


    //目前 real me 来说，先用决定是否用 StatusBarUtil_SetBlackFonColor 修改为黑色字体（无法修改为黑色）
    //再用 StatusBarUtil_SetStatusBarColor 修改背景色.
    //低版本下直接修改 view 的背景色为好，状态栏的颜色会自动匹配的



    //设置为黑色字体，默认是白色？//似乎默认并不一定，默认应该是设计器中的反色
    static public void StatusBarUtil_SetBlackFonColor(AppCompatActivity activity)
    {
        //在目前的 real me 版本中只能修改字体颜色为黑色
        //修改状态栏背景
        //状态栏中的文字颜色和图标颜色，需要android系统6.0以上，而且目前只有一种可以修改（一种是深色，一种是浅色即白色）
        if (android.os.Build.VERSION.SDK_INT > Build.VERSION_CODES.M) {

            //好象过时了
            activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);

            activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_VISIBLE);

        }//if

        //this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        //this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);

        //No virtual method getInsetsController()Landroid/view/WindowInsetsController
        //WindowInsetsController ic = getWindow().getInsetsController();
//            WindowInsetsController ic = getWindow().getInsetsController(); //有版本要求，现在不行
//            if (ic != null) {
//                ic.hide(WindowInsets.Type.statusBars());
//                ic.hide(WindowInsets.Type.navigationBars());
//            }
    }//

    //神奇，这样字是白色
    static public void StatusBarUtil_SetWhiteFonColor(AppCompatActivity activity) {
        //在目前的 real me 版本中只能修改字体颜色为黑色
        //修改状态栏背景
        //状态栏中的文字颜色和图标颜色，需要android系统6.0以上，而且目前只有一种可以修改（一种是深色，一种是浅色即白色）
        if (android.os.Build.VERSION.SDK_INT > Build.VERSION_CODES.M) {

            //好象过时了
            //activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);

            activity.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_VISIBLE);

        }//if

    }//


    //取一个 ac 的根 view
    public static View GetRootView(AppCompatActivity activity)
    {
        //DecorView为整个Window界面的最顶层View
        //this.getWindow().getDecorView().setBackgroundColor(Color.BLUE); //test //ok

        View view = activity.getWindow().getDecorView();

        return view;
    }//

    //文字左对齐
    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    public static void UILabel_TextAlignLeft(TextView label)
    {
        //label.textAlignment = NSTextAlignmentLeft;
        //label.backgroundColor = [UIColor redColor];
        //label.superview.backgroundColor = [UIColor blueColor];

        ////UILabel_TextAlignLeft(label);  //应该还要改对齐标题的地方
        label.setTextAlignment(TEXT_ALIGNMENT_TEXT_START);  //这个有效果，但有版本要求
        //label.setGravity(Gravity.CENTER);
        //label.setGravity(Gravity.LEFT); //无效

        //---- 充满父控件
        View p = (ViewGroup)label.getParent();
        int height = p.getHeight();
        int width = p.getWidth();

        ViewGroup.LayoutParams lp = label.getLayoutParams();  //默认控件还要要用默认的 lp 来设置宽高的
        lp.width = width;//300;
        lp.height = height;
        label.setLayoutParams(lp);
        label.setGravity(Gravity.CENTER); //CENTER);
        label.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL); //不行? 好象可以
        //label.setGravity(Gravity.START);
        label.layout(0, 0, width, height);
    }//


    //圆角
    //https://www.it610.com/article/1288765025120493568.htm
    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public static void UIView_SetBorderRadius(View view, float v)
    {
        CircleOutlineProvider mOutlineProviderCircle = new CircleOutlineProvider();
        mOutlineProviderCircle.Radius = v;
        view.setOutlineProvider(mOutlineProviderCircle);
        view.setClipToOutline(true);
    }

    //https://www.it610.com/article/1288765025120493568.htm
    //圆角的实现,确实可以
    //应该是 android 5
    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private static class CircleOutlineProvider extends ViewOutlineProvider
    {
        public float Radius;

        @Override
        public void getOutline(View view, Outline outline)
        {
            int margin = Math.min(view.getWidth(), view.getHeight())/10;
            //view是match_parent的
            //ok//outline.setRoundRect(margin, margin, view.getWidth() - margin, view.getHeight() , margin/2);
            //outline.setOval(0, 0, 9, 9);

            ////outline.setRoundRect(margin, margin, view.getWidth(), view.getHeight() , Radius);
            //2021.11.03 删除掉间隔
            outline.setRoundRect(0, 0, view.getWidth(), view.getHeight() , Radius);
        }//
    }//

    //---------------------------------------------------------------

    //设置一个图标
    //void SetIcon(String uijson_id, String as_fn)
    static public void SetIcon_t1(ViewGroup view, String as_fn, Context context)
    {
        //ViewGroup view = (ViewGroup) uiJson.GetView(uijson_id);

        if (null == view) return;

        ImageView img = new ImageView(context);
        view.addView(img);
        //img.setImageURI("ass");
        //Bitmap bm = BitmapFactory.decodeFile("home.png");
        //Bitmap bm = getImageFromAssetsFile("home.png");
        Bitmap bm = getImageFromAssetsFile(as_fn, context);
        img.setImageBitmap(bm);

        //view.addView(img);

        Rect frame = new Rect(0, 0, view.getWidth(), view.getHeight());

        //Functions.Functions_cur_this

        Functions.SetFrame_dp(img, 0, 0, 50, 50, context);
        //Functions.SetFrame_dp(img, 0, 0, 50, 50);
        Functions.SetFrame(img, 0, 0, 50, 50);
        Functions.SetFrame(img, frame.left, frame.top, frame.width(), frame.height());

    }//

    //从 assets 目录中取一个文件设置为图标 //ll  尝试缓存图片
    static public int SetIcon(ImageView img, String as_fn, Context context)
    {
        int r = 0;
        try {
            //ViewGroup view = (ViewGroup) uiJson.GetView(uijson_id);

            //if (null == view) return;

            //ImageView img = new ImageView(context);
            //view.addView(img);
            //img.setImageURI("ass");
            //Bitmap bm = BitmapFactory.decodeFile("home.png");
            //Bitmap bm = getImageFromAssetsFile("home.png");
            Bitmap bm = getImageFromAssetsFile(as_fn, context);
            img.setImageBitmap(bm);

            //Rect frame = new Rect(0, 0, view.getWidth(), view.getHeight());

            //Functions.SetFrame_dp(img, 0, 0, 50, 50, context);
            //Functions.SetFrame(img, 0, 0, 50, 50);
            //Functions.SetFrame(img, frame.left, frame.top, frame.width(), frame.height());

            if (null != bm) r = 1; //bm 是有可能为 null 的

        }catch (Exception e){
            e.printStackTrace();
            ShowMessage_ok("图片加载失败。SetIcon() " + as_fn, Functions_cur_this);
        }//try

        //----------------------------------------------------------------
        //---- 2022 如果从 as 中取失败则使用全路径
//        if (0 == r)
//        {
//            //这个是 http 下载图片的，也是安卓 APP 的内部存储路径，会随着 APP 的删除而删除。相对的就还有一个外部路径
//            //fn = HttpFunctions.GetPAth_write(image);
//            Functions.SetIcon_FullPath(img, as_fn, context);
//        }

        //----------------------------------------------------------------

        return r;
    }//


    //全路径
    static public void SetIcon_FullPath(ImageView img, String full_fn, Context context)
    {
        try {

            //Bitmap bm = getImageFromAssetsFile(as_fn, context);
            Bitmap bm = getImageFromFullPathFile(full_fn, context);

            img.setImageBitmap(bm);

            //Rect frame = new Rect(0, 0, view.getWidth(), view.getHeight());

            //Functions.SetFrame_dp(img, 0, 0, 50, 50, context);
            //Functions.SetFrame(img, 0, 0, 50, 50);
            //Functions.SetFrame(img, frame.left, frame.top, frame.width(), frame.height());
        }catch (Exception e){
            e.printStackTrace();
            ShowMessage_ok("图片加载失败。SetIcon_FullPath() " + full_fn, Functions_cur_this);
        }//try

    }//

    //变更高宽比显示全部图片内容 //2021.10.25
    static public void SetIcon_ShowAll_Scale(ImageView img)
    {

        img.setScaleType(ImageView.ScaleType.FIT_XY);


    }//

    // //2021.10.25
    static public void ShowView(View view, int show)
    {
        //v1.setVisibility(View.VISIBLE);   //正常显示
        //v1.setVisibility(View.INVISIBLE); //隐藏参与布局（还占着地方）
        //v1.setVisibility(View.GONE);      //隐藏不参与布局（不占地方）

        if (1 == show)
        {
            view.setVisibility(View.VISIBLE);
        }else{
            view.setVisibility(View.INVISIBLE);
        }


    }//


    //从Assets中读取图片
    //Bitmap bitmapimg = getImageFromAssetsFile("Cat_Blink/cat_blink0000.png");

    static public Bitmap getImageFromAssetsFile(String fileName, Context context)
    {
        Bitmap image =null;

        AssetManager am = context.getResources().getAssets();

        try
        {
            InputStream is = am.open(fileName);
            image = BitmapFactory.decodeStream(is);
            is.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }

        return image;
    }//

    //从全路径加载图片
    static public Bitmap getImageFromFullPathFile(String fileName, Context context)
    {
        Bitmap image =null;

        //AssetManager am = context.getResources().getAssets();
        File file = null;

        try
        {
            file = new File(fileName);

            //InputStream is = am.open(fileName);
            InputStream is = new FileInputStream(file);

            image = BitmapFactory.decodeStream(is);
            is.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }

        return image;
    }//

    //回收图片，避免 oom 。未测试
    //https://blog.csdn.net/u012301841/article/details/50493214
    public static void CleanImageView (ImageView imageView)
    {
        if (!(imageView.getDrawable() instanceof BitmapDrawable))
            return;

        // clean uo the view's image for the sake of memory
        BitmapDrawable b = (BitmapDrawable) imageView.getDrawable();
        b.getBitmap().recycle();
        imageView.setImageDrawable(null);

    }//


    //---------------------------------------------------------------



    static public int Color(String s_hex) {


        return hexStringToColor(s_hex);
    }//


    static public int hexStringToColor(String s_hex) {

        try {

            if (null == s_hex || s_hex.trim().length()<1) return 0;

            s_hex = s_hex.replace("#", "");
            //int borderColor = Color.parseColor("#D8FE54");
            int r = Color.parseColor("#" + s_hex);

            return r;
        }catch (Exception e)
        {
            e.printStackTrace();
        }

        return 0;
    }//

    //动画显示一个 view //test
    //最终效果要类似于 ios 的 [Functions AnimaWindow_toLeft:frmPageConfig.view];
    static public void ShowToLeft(View view)
    {
        float distanceY = 50;
        float distanceX = 50;
        float nextY = view.getY() - distanceY;
        float nextX = view.getX() - distanceX;

        // 属性动画移动
        ObjectAnimator y = ObjectAnimator.ofFloat(view, "y", view.getY(), nextY);
        ObjectAnimator x = ObjectAnimator.ofFloat(view, "x", view.getX(), nextX);


        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(x, y);
        //clq//animatorSet.setDuration(0);
        animatorSet.setDuration(1000);  //应该是毫秒，持续时间，为 0 的话就是立即变化了
        animatorSet.start();


        //https://blog.csdn.net/sinat_28238111/article/details/52289050
    }//

    //显示一个窗口
    static public void ShowWindow(Context _this,  Class<?> cls)
    {

        //startActivity(new Intent(_this, ActivityLoading.class));
        //new Intent(_this, cls);
        startActivity(_this, new Intent(_this, cls), null);
    }//

    //关闭当前窗口，返回上一个//注意 Activity 才有这个函数
    static public void HideWindow(Activity _this)
    {

        _this.finish();
    }//

    //设置点击事件不要再传递
    static public void SetDivIs_EventTouchEnd(ui_control_panel div)
    {
        div.EventTouch_End = true;

    }//

    //--------------------
    //https://blog.csdn.net/u010983881/article/details/51993157
    //据说代码里一般是 px 那么就要转换成 dp 后再赋值。要不会与设计界面上的 dp 为主的发生冲突
    //根据手机的分辨率从 dp 的单位 转成为 px(像素)
    public static int dip2px(Context context, float dpValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }//

    //根据手机的分辨率从 px(像素) 的单位 转成为 dp
    public static int px2dip(Context context, float pxValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }//

    public static int dp2px(Context context, float dpValue) {

        return dip2px(context, dpValue);
    }//

    //根据手机的分辨率从 px(像素) 的单位 转成为 dp
    public static int px2dp(Context context, float pxValue) {

        return px2dip(context, pxValue);
    }//

    //代码控制按钮的图标大小
    public static void SetButton_IconSize_dp(Context context, Button btn, float width, float height) {

        //代码控制按钮的图标大小，但是感觉太啰嗦了，而且还要计算对齐
        //Button btn1 = findViewById(R.id.btnT1);
        Drawable d1 = btn.getCompoundDrawables()[1]; //好像不能直接写 0 ，这个值应该是其在 setCompoundDrawables 中的位置，0 的话应该表示左边的按钮
        //Rect r1 = d1.getBounds();

        //Rect r2 = new Rect(0, 0, d1.getMinimumWidth() * 2 / 3, d1.getMinimumHeight() * 2 / 3);
        //d1.setBounds(0,0, 50, 50);
        //d1.setBounds(r1);
        //d1.setBounds(0, 0, 150, 250); //单位是 px 还是 dp
        d1.setBounds(0, 0, Functions.dp2px(context, width), Functions.dp2px(context, height)); //单位是 px 还是 dp

        //--------------------
        //https://blog.csdn.net/u010983881/article/details/51993157
        //据说代码里一般是 px 那么就要转换成 dp 后再赋值。

        btn.setCompoundDrawables(null, d1, null, null);//将改变了属性的 drawable 再重新设置回去//这一步非常重要,这样就可以不计算对齐了

    }//

    //安卓的位置应当是原始的 px 像素，而 ios 的是映射过的. 这个函数的目的就是使用界面设计器的 dp 为单位
    static public void SetFrame_dp(View view, int left, int top, int w, int h, Context ui_context)
    //static public void SetFrame_dp(View view, int left, int top, int w, int h)
    {
        //安卓中设置位置的原始方法应该是 setX,setY . layout 应该还是考虑了布局属性的
        //另外还有 setLeftTopRightBottom(int left, int top, int right, int bottom)
        //它等于内部的  setFrame()
        //view.layout(StrToInt(left), StrToInt(top), StrToInt(w), StrToInt(h));
        ////view.setLeftTopRightBottom(StrToInt(left), StrToInt(top), StrToInt(left) + StrToInt(w), StrToInt(top) + StrToInt(h));

        //left = Functions.px2dp(ui_context, left);
        //top = Functions.px2dp(ui_context, top);
        //w = Functions.px2dp(ui_context, w);
        //h = Functions.px2dp(ui_context, h);

        //Context ui_context = Functions_cur_this;


        left = Functions.dp2px(ui_context, left);
        top = Functions.dp2px(ui_context, top);
        w = Functions.dp2px(ui_context, w);
        h = Functions.dp2px(ui_context, h);

        ////view.setLeftTopRightBottom(left, top, left + w, top + h);
        Functions.SetFrame(view, left, top, w, h); //2022 这种方式更正规
    }//

    static public void SetFrame_dp_c(View view, int left, int top, int w, int h)
    {
        //安卓中设置位置的原始方法应该是 setX,setY . layout 应该还是考虑了布局属性的
        //另外还有 setLeftTopRightBottom(int left, int top, int right, int bottom)
        //它等于内部的  setFrame()
        //view.layout(StrToInt(left), StrToInt(top), StrToInt(w), StrToInt(h));
        ////view.setLeftTopRightBottom(StrToInt(left), StrToInt(top), StrToInt(left) + StrToInt(w), StrToInt(top) + StrToInt(h));

        //left = Functions.px2dp(ui_context, left);
        //top = Functions.px2dp(ui_context, top);
        //w = Functions.px2dp(ui_context, w);
        //h = Functions.px2dp(ui_context, h);

        Context ui_context = Functions_cur_this;


        left = Functions.dp2px(ui_context, left);
        top = Functions.dp2px(ui_context, top);
        w = Functions.dp2px(ui_context, w);
        h = Functions.dp2px(ui_context, h);

        ////view.setLeftTopRightBottom(left, top, left + w, top + h);
        Functions.SetFrame(view, left, top, w, h); //2022 这种方式更正规
    }//

    static public void SetFrame(View view, int left, int top, int w, int h)
    {

        //left = Functions.dp2px(ui_context, left);
        //top = Functions.dp2px(ui_context, top);
        //w = Functions.dp2px(ui_context, w);
        //h = Functions.dp2px(ui_context, h);

        view.setLeftTopRightBottom(left, top, left + w, top + h);
        //view.layout(left, top, left + w, top + h); //test 2022.01.11 //按道理还是应该有这个，否则有些版本会不触发 onlayout
        //不过，两者分开也更好控制控件的表现

        //在工业机中，如果 setLeftTopRightBottom 和 layout 设置的参数相同就不会触发 onlayout
        //但普通手机中又不能这样用,因为 layout 会死循环

        //layout() 一般会触发（似乎不是一定） onLayout
        //setLeftTopRightBottom() 一般会触发 onSizeChanged ，但从安卓源码看，当大小不变时是不会触发的
        //所以有时候还是要手工调用布局的自定义函数的。
    }//

    //ios 兼容
    //UIScrollView_ShowAllChild(pnlSC, 100.0f); //拉伸 contentSize 以显示所有子控件
    //拉伸 contentSize 以显示所有子控件
    //static public void UIScrollView_ShowAllChild(ViewGroup pnlSC, int sp)
    static public void UIScrollView_ShowAllChild(ui_control_panel_ScrollView pnlSC, int sp)
    {
        ui_control_panel_ScrollView.UIScrollView_ShowAllChild(pnlSC, sp);

    }//


    //取父 view ，注意，据说某些容器没实现具体的 getParent() 接口
    static public ViewGroup GetParentView(View view)
    {
        //--- 测试取父 view //ok
        //ViewGroup parentview = (ViewGroup)this.uiJson.root_view.getParent();
        //parentview.setBackgroundColor(Functions.Color("#ff0000"));

        ViewGroup parentview = (ViewGroup) view.getParent();
        //parentview.setBackgroundColor(Functions.Color("#ff0000"));

        return parentview;
    }//

}//

