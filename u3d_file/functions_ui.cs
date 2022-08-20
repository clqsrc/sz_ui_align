using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//UI 相关函数
//目前主要只是一些图片按钮而已

/*
https://blog.csdn.net/weixin_45565630/article/details/115674610

public Texture textureButton;

    void OnGUI()
    {
        if (!textureButton)
        {
            Debug.LogError("请在inspector窗口给textureButton一张图片");
            return;
        }

        if (GUI.Button(new Rect(10, 10, 500, 500), textureButton))//图片做按键
            Debug.Log("Clicked the button with an image");

        if (GUI.Button(new Rect(10, 670, 200, 30), "Click"))//文字按键
            Debug.Log("Clicked the button with text");
    }


*/


public class functions_ui : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

        
    }//


    //后两个参数应该是宽高
    static public Rect new_Rect(float x, float y, float w, float h)
    {
        return new Rect(x, y, w, h);
    }//

    //应该是画出一个按钮，并且判断是事业按下
    static public bool UI_Button(string txt)
    {

        // if (GUI.Button(new Rect(100, 100, 200, 30), txt))//文字按键
        // {
        //     //Debug.Log("Clicked the button with text");
        //     return true;
        // }//

        if (GUI.Button(new_Rect(100, 100, 80, 50), txt))//文字按键
        {
            //Debug.Log("Clicked the button with text");
            return true;
        }//        

        return false;
    }//

    static public bool UI_Button(string txt, float x, float y, float w, float h)
    {


        if (GUI.Button(new_Rect(x, y, w, h), txt))//文字按键
        {
            //Debug.Log("Clicked the button with text");
            return true;
        }//        

        return false;
    }//

}//


