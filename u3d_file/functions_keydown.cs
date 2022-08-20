using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//判断按下的是什么键，包括键盘和鼠标

//一般是在主窗体的 update 中调用

public class functions_keydown : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }//


    static public int OnKeyDown_Left()
    {

        // 获取按键输入
        // if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.W)) speedForward = tourCamera.forward;
        // if (Input.GetKey(KeyCode.DownArrow) || Input.GetKey(KeyCode.S)) speedBack = -tourCamera.forward;
        // if (Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) speedLeft = -tourCamera.right;
        // if (Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) speedRight = tourCamera.right;

        // //https://blog.csdn.net/qq_46649692/article/details/125005962

        if (Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) return 1;


        return 0;


    }//

    static public int OnKeyDown_Right()
    {
        if (Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) return 1;

        return 0;
    }//

    //用户是否按了“向上”键
    static public int OnKeyDown_Up()
    {
        if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.W)) return 1;

        return 0;
    }//

    //用户是否按了“向下”键
    static public int OnKeyDown_Down()
    {
        if (Input.GetKey(KeyCode.DownArrow) || Input.GetKey(KeyCode.S)) return 1;

        return 0;
    }//

}//
