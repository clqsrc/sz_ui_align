using System.Collections;
using System.Collections.Generic;
using UnityEngine;



//移动物体的函数集

public class functinos_move : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }//

    // Update is called once per frame
    void Update()
    {
        
    }//

    //是在 OnTime 即 Update 函数中调用的函数 //每秒移动多少
    static public void MoveObject_atOnTime(GameObject obj, float x, float y, float z)
    {

        //https://blog.csdn.net/maojiaoliang/article/details/123883389
        //实现每一帧物体向x方向移动1.5个单位
        //transform.Translate(1.5f*Time.deltaTime,0,0);
        //或
        //transform.position += new Vector3(1.5f*Time.deltaTime,0,0);

        obj.transform.position += new Vector3(x * Time.deltaTime, y * Time.deltaTime, z * Time.deltaTime);

    }//

    

}//
