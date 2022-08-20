using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//取得物体，以及 物体类型转换的各种函数集

public class functions_object : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    //旋转物体顺时针 90 度，因为现在的模型都是向上的
    static public void RotateObject90(GameObject obj)
    {

        obj.transform.Rotate(0, 90, 0, Space.Self);

    }//

    static public void RotateObject(GameObject obj, float x, float y, float z)
    {

            //obj.transform.Rotate(0, 90, 0, Space.Self);
            obj.transform.Rotate(x, y, z, Space.Self);

    }//


    //立即移动
    static public void MoveObject(GameObject obj, float x, float y, float z)
    {

        //https://blog.csdn.net/maojiaoliang/article/details/123883389
        //实现每一帧物体向x方向移动1.5个单位
        //transform.Translate(1.5f*Time.deltaTime,0,0);
        //或
        //transform.position += new Vector3(1.5f*Time.deltaTime,0,0);

        obj.transform.position = new Vector3(x, y, z);

    }//

    //加载一个模型
    //几个要注意的问题：
    //1.模型要在 Resources 目录下，而且是 Resources->Prefabs 下
    //2.模型从 blender 过来的时候默认的材质是 Standard ，会很模糊，要修改为修改为 Mobile/Diffuse
    //导入后还不能直接修改。要改导入器的属性（据说是插件），对于目前的 u3d 来说，其实就是要先点击
    //Prefabs 中的导入模型，然后选择 inspector 属性页面。
    //然后选择 materials 页，选择材质 在location处选择 使用扩展模式。
    //之后才能对单个物体修改材质球为内置的 Mobile/Diffuse 材质球。另外我的版本这样还只能改父节点的
    //对子节点还要将父节点的材质球拖给它们才行。
    //参考 https://blog.csdn.net/Yuanben_wuxin/article/details/84839045
    //https://www.jianshu.com/p/ddbfbdbd713a
    //3.是导入的模型有曲面时不光滑的问题。
    //这个只要在导入器 inspector 属性中让它重新计算法线就行了。方法是
    //Model - Geometry 中，将 Normals 设为 Calculate，Apply，让 Unity 重计算模型的法线，模型及其在Project 等窗口的缩略图即显示为平滑。
    //参考 https://blog.csdn.net/xzqsr2011/article/details/111402614
    //他们讨论的是 fbx 格式，其实我的 dae 格式也是一样的。
    //4.材质球颜色不正常的解决
    //我暂时是用图片贴图的颜色卡方式来解决的，即用贴图来代替颜色，然后再修改 uv 映射为一个图片上的点。
    //参考 https://jingyan.baidu.com/article/6766299733e7c815d41b842b.html
    static public GameObject LoadModel(string fn)
    {

        GameObject boxPrefab;
        //Texture tex;
        /// 
        //boxPrefab = Resources.Load<GameObject>("Prefabs/box");
        //tex = Resources.Load<Texture>("Images/aaa");

        //1.2 添加预设文件
        //将添加的模型文件存放到 Assets->Resources->Prefabs 目录下（没有此目录的创建目录）

        boxPrefab = Resources.Load<GameObject>(fn);


        Vector3 vector3 = new Vector3( 0.1f, 0, 0.1f);  //位置

        GameObject box = MonoBehaviour.Instantiate(boxPrefab, vector3, Quaternion.identity);

        //box.GetComponent<MeshRenderer>().material.mainTexture = tex;  //贴图

        box.AddComponent<Rigidbody>();  //应该是添加刚体 //但它会默认有重力，如何去掉呢 //添加刚体后会互相碰撞并弹开

        //参考 https://www.jianshu.com/p/3ebf6ae6333a 还可以设置重量等
        Rigidbody rb = box.GetComponent<Rigidbody>();
        rb.useGravity = false;

        return box;

    }//func

    //加载一个模型 //去掉加载时添加刚体后默认有的重力
    static public void LoadModel_removeGravity(GameObject box)
    {
        //box.AddComponent<Rigidbody>();  //应该是添加刚体 //但它会默认有重力，如何去掉呢 //添加刚体后会互相碰撞并弹开

        //参考 https://www.jianshu.com/p/3ebf6ae6333a 还可以设置重量等
        Rigidbody rb = box.GetComponent<Rigidbody>();
        rb.useGravity = false;

    }//func


}//


/*

// 删除节点上自己这个脚本组件
MonoBehaviour.Destroy(this);

// 删除节点上的其他组件
MonoBehaviour.Destroy(this.GetComponent<Image>());

3.当我们的组件实例被删除的时候，会调用OnDestroy()函数

// OnDestroy()函数，可以在里面写去掉委托中的订阅的语句，取消事件的监听
void OnDestroy() {
　　this.scene.e -= this.on_item_event_called;
　　this.scene.action -= this.on_action_called;
　　Debug.Log("OnDestroy");
}

二、显示和隐藏节点

1.显示和隐藏节点

// 显示和隐藏这个节点，false隐藏，true显示。就是所挂脚本的节点的Inspector视图面板的左上角那个打钩的去掉和勾起的状态切换。
this.gameObject.SetActive(false);

2.显示节点的时候会调用OnEnable()函数

// 节点激活的时候调用;
void OnEnable() {
　　Debug.Log("OnEnable");
}

3.隐藏节点的时候会调用OnDisable()函数
// 节点隐藏或禁止的时候调用
void OnDisable() {
　　Debug.Log("OnDisable");
}

*/









