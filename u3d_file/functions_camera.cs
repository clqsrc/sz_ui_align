using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//摄像头相关函数


public class functions_camera : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

        
    }//


    //取主摄像头
    //实际上这还没有得到主摄像头，摄像头实际上是类 Camera 。这里得到的是包含了 Camera 的 GameObject
    //class in UnityEngine 继承自： Behaviour
    //static public Behaviour GetMainCamera()
    static public GameObject GetMainCamera()
    {

        //return Camera.main;
        // Camera camera = Camera.main;

        //mainCamera = GameObject.Find("MainCamera").GetComponent<Camera>(); 
        GameObject obj = GameObject.Find("Main Camera"); //用名称来找的，默认 u3d 生成的摄像头就是名就是 "Main Camera"，而 "MainCamera" 是 tag 名
        //两者的获取方法是不同的，要使用不同的两个函数。
        //而这时是不能用 Camera.main ，因为我们要移动的话一般是只能移动 GameObject 而 Camera 似乎是不能直接移动的

        // //GameObject obj = (GameObject)camera;
        // GameObject obj = camera as GameObject;

        return obj;
    }//

    static public Camera GetMainCamera_()
    {

        //return Camera.main;
        // Camera camera = Camera.main;

        //mainCamera = GameObject.Find("MainCamera").GetComponent<Camera>(); 
        GameObject obj = GameObject.Find("Main Camera"); //用名称来找的，默认 u3d 生成的摄像头就是名就是 "Main Camera"，而 "MainCamera" 是 tag 名
        //两者的获取方法是不同的，要使用不同的两个函数。
        //而这时是不能用 Camera.main ，因为我们要移动的话一般是只能移动 GameObject 而 Camera 似乎是不能直接移动的

        // //GameObject obj = (GameObject)camera;
        // GameObject obj = camera as GameObject;

        return obj.GetComponent<Camera>();
    }//


}//


