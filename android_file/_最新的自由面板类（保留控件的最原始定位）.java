

package ui_json;

//其实就是实现一个 ios 的 uiview 。只是 android 的 view 不能加入子控件而已

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

//---------------------------------------------------------------
//ui_control_panel_base 满足布局，ui_control_panel 扩展绘画功能

//太啰嗦，还是要精简。只要能自由布局就可以了。考虑到 TextView Edit 这样的默认控件关联的东西太多，还是用 child.layout() 来修改具体的控件位置。
//虽然这个不是最底层的实现，不过没有太多时间去花在这上面。而且目前的实现也已经非常的小了。

//参考 https://blog.csdn.net/weixin_51065489/article/details/126465781 做一个最原始的版本

//---------------------------------------------------------------




//--------
//两个不可少的：重载 onLayout， 构造函数

//超简化的 AbsoluteLayout 就可以
public class ui_control_panel_base_t2023 extends ViewGroup {

    //------------------------------------------------

    //参考 view 的 OnClickListener ，目前只能用 接口不能用类
    public interface OnSizeListener
    {

        //void onClick(View v);
        void onSize(View v);
    }


    public int ON_SIZE_TYPE_AsAndroid12PadV1 = 0;  //2023 兼容而已，以后不要用

    public OnSizeListener onSizeListener = null;

    //从其他自定义布局的文章来看，修改控件的位置一般还是用的 child.layout()

    //位置改变时的事件，一般是要也改变子视图的位置
    //https://blog.csdn.net/weixin_51065489/article/details/126465781
    //只需要做一件事，那就是调用每一个子View的onLayout()把之前在onMeasure()中保存下来的尺寸和位置作为参数传进去，让它们把自己的位置和尺寸保存下来，并进行自我布局
    //测量子View的过程就是一个排版过程，在测量过程中能够拿到每一个子View的位置然后保存就好
    //对于尺寸，绝大多数情况来说每一个子View它所测量的尺寸就是它最后的尺寸，用的时候调用getMeasureWidth()和getMeasureHeight()即可
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b)
    //public void onLayout(boolean changed, int l, int t, int r, int b) //最好还是不要公开，再加一个函数好了
    {
        //super.onLayout(changed, l, t, r, b);  //似乎没有作用 //也不能直接访问

        //------------------------------------------------

        //其实这个时候不一定有值
        int w = this.getWidth();
        int h = this.getHeight();



        //if (0 == ON_SIZE_TYPE_AsAndroid12PadV1)
        ////DoForOnSize(w, h);  //什么都不做其实子控件也是可以布局的

    }//


    //默认应该是带有两个参数的。不过我们需要它表现得象一个普通控件，所以换一个空参数的版本
    //放到设计器中，必须要有这个函数
    public ui_control_panel_base_t2023(Context context, AttributeSet attrs) {
        super(context, attrs);


    }//

//    public ui_control_panel() {
//        super(null, null);
//
//
//    }//

    public ui_control_panel_base_t2023(Context context) {
        super(context);


    }//

    //---------------------------------------------------------------
    //下面是非必须的函数

    //onMeasure方法的作用就是计算出自定义View的宽度和高度。这个计算的过程参照父布局给出的大小，以及自己特点算出结果

    //从 https://blog.csdn.net/weixin_51065489/article/details/126465781  看这只是限制子控件的大小，可以直接给不限制。
    //实测不给限制还是不行的

    //据说 onMeasur 是可选的，所以不实现也是可以的 //不行，对于 TextView 这些默认控件来说，这里的代码不可少，否则就无法上下居中对齐 //但是可以左右对齐
    //大小要改变时的事件
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);


        int count = getChildCount();

        for (int i = 0; i < count; i++) {
            View child = getChildAt(i);
            //if (child.getVisibility() != GONE) {

                //对于 TextView 这些默认控件来说，这里的代码不可少，否则就无法上下居中对齐 //但是可以左右对齐
                measureChild(child, widthMeasureSpec, heightMeasureSpec);  //这样对吗？ //应该是告诉子控件最大能是多少
                //measureChild(child, -1, -1);  //这样对吗？ 这个是不行的

                //if (true) return;
                //2023 理论上说 LayoutParams 是这个类里才有用的，因为每个布局都有自己扩展的子类
                int h = child.getMeasuredHeight();
                int w = child.getMeasuredWidth();
                ViewGroup.LayoutParams lp = new ViewGroup.LayoutParams(w, h);
                //ViewGroup.LayoutParams lp = new ViewGroup.LayoutParams(0, 0);  //对于 TextView 来说，这样也是无法上下居中对齐的
                child.setLayoutParams(lp);

            //}
        }//for


    }//




}//
