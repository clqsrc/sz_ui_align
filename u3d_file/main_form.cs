using System;
using System.Globalization;
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//using UnityEngine.Debug;

using static functinos_move;
using static functions_object;
using static functions_keydown;
using static functions_camera;
using static functions_ui;

public class main_form : MonoBehaviour
{

    //主物体
    public GameObject main_ship = null;

    //反舰导弹
    public GameObject main_fjdd = null;


    //小流星群
    ArrayList sub1_list = new ArrayList();

    // Start is called before the first frame update
    void Start()
    {
        String s = "";


        //LoadModel("Prefabs/b3dt2v1");
        //main_ship = LoadModel("model3d/fj");
        main_ship = LoadModel("Prefabs/myship");   //主飞船
        main_fjdd = LoadModel("Prefabs/fj");     //反舰导弹

        //小流星
        GameObject sub1 = LoadModel("Prefabs/sub_obj1");
        //GameObject sub1 = LoadModel("Prefabs/myship");

        //GameObject sctript1 = GameObject.Find("sub1_control").GetComponent<ScriptName>();
        //GameObject sctript1 = new GameObject("sub1_control");

        //u3d 动态添加脚本，其实是给物体添加脚本中定义的那个类!!!
        sub1.AddComponent<sub1_control>();
        sub1.AddComponent<BoxCollider>();//添加碰撞器
        //UnityEngine.Debug.Log("Collide the sub");

        sub1_list.Add(sub1);

        MoveObject(sub1, 100, 0, 0);

        //主飞船的起始位置
        MoveObject(main_ship, 0, 0, 0);


        //RotateObject90(main_ship);
        RotateObject(main_ship, 270, -180, 0);
        RotateObject90(main_fjdd);        //反舰导弹 的模型方向也不太正确

        //远一点
        //MoveObject(GetMainCamera(), 0, 0, -20);      //居然是 z 方向控制远近
        //高一点
        MoveObject(GetMainCamera(), 0, 10, -20);      

        GetMainCamera_().transform.LookAt(main_ship.transform);
        
    }//

    // Update is called once per frame
    void Update()
    {

        if (null != main_fjdd)
        {
            MoveObject_atOnTime(main_fjdd, 10, 0, 0);
        }


        //移动小流星
        for (int i=0; i<sub1_list.Count; i++)
        {
            GameObject obj = (GameObject)sub1_list[i];

            MoveObject_atOnTime(obj, -10, 0, 0);
        }//for

        //移动摄像机  //实际上摄像头是附着在某个 GameObject 上的
        GameObject camera = GetMainCamera();
        //MoveObject_atOnTime(camera, 5, 5, 0);

        //摄像头向下移动
        if (1 == OnKeyDown_Down())
        {
            MoveObject_atOnTime(camera, 0, -5, 0);
        }//

        //摄像头向h移动
        if (1 == OnKeyDown_Up())
        {
            MoveObject_atOnTime(camera, 0, 5, 0);
        }//

        
    }//


    //u3d 的 ui 点击事件等
    void OnGUI()
    {
        // if (functions_ui.UI_Button("test1")) 
        // {
        //     UnityEngine.Debug.Log("ui click");
        // }

        if (functions_ui.UI_Button("飞船", 50, 40, 60, 40)) 
        {
            UnityEngine.Debug.Log("ui click");
        }

        if (functions_ui.UI_Button("摄像机", 120, 40, 80, 40)) 
        {
            UnityEngine.Debug.Log("ui click");
        }


        if (functions_ui.UI_Button("射击", 50, 250, 60, 40)) 
        {
            UnityEngine.Debug.Log("ui click");
        }

        //UnityEngine.Debug.Log("OnGUI()");

    }//
    
}//
