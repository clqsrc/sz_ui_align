import { _decorator, Component, Node } from 'cc';
const { ccclass, property } = _decorator;

import * as cc from 'cc';


//ui操作类


class functions_ui {


    //设置按键事件
    public static SetOnKeyDown(obj:any, func:any)
    {
        //cc.input.on(cc.Input.EventType.KEY_DOWN, this.OnKeyDown, this); 
        cc.input.on(cc.Input.EventType.KEY_DOWN, func, obj); 
        
    }//

    //设置鼠标点击事件 //似乎不能对 3d 物体设置
    public static SetOnClick(obj:any, func:any)
    {
        //cc.input.on(cc.Input.EventType.KEY_DOWN, this.OnKeyDown, this); 

        //cc.input.on(cc.Input.EventType.MOUSE_UP, func, obj);  //这个是全局的，如果只对单个物体，则是 "物体".on(...)

        ////obj.on(cc.Input.EventType.MOUSE_UP, func, obj);  //MOUSE_UP 是桌面的，手机是另外的事件

        // Node.EventType.TOUCH_START touch-start 当手指触点落在目标节点区域内时。
        // Node.EventType.TOUCH_MOVE touch-move 当手指在屏幕上移动时。
        // Node.EventType.TOUCH_END touch-end 当手指在目标节点区域内离开屏幕时。
        // Node.EventType.TOUCH_CANCEL touch-cancel 当手指在目标节点区域外离开屏幕时。

        // //https://www.jianshu.com/p/40349ad9318e

        obj.on(cc.Input.EventType.TOUCH_END, func, obj);

        
    }//

    //设置按键事件 //似乎不能对 3d 物体设置 //这个是对 3d 物体生效的
    public static SetOnClick_for3D(camera:cc.Camera, obj:any, func:any)
    {
        cc.input.on(cc.Input.EventType.MOUSE_DOWN, (touch)=>{

            let pos = touch.getLocation();

            //let camera:cc.Camera = this.camera as unknown as cc.Camera; //奇怪，这个不行
            ////let camera:cc.Camera = this.mainCamera;

            let ray = camera.screenPointToRay(pos.x, pos.y); //从摄像头取得的射线

            //要触发的物体要有 BoxCollider //https://docs.cocos.com/creator/manual/zh/physics/physics-collider.html
            //通过代码添加
            // import { BoxCollider } from 'cc'
            // const boxCollider = this.node.addComponent(BoxCollider);

            if (cc.PhysicsSystem.instance.raycastClosest(ray))  //应该是判断是否选中了物体
            {
                const res = cc.PhysicsSystem.instance.raycastClosestResult;  //应该是取得射线选中的结果

                let hitnode = res.collider.node;  //射线选中的物体

                ////alert(hitnode.name);

                //alert("obj:" + obj._id + " hitnode:" + hitnode._id);

                //if (obj == hitnode)
                if (obj._id == hitnode._id) //ts 里如何判断两个物体一致？这个是 cocos 的 html 环境下可用的
                {

                    func();

                }

            }

        });

        
    }//

    //----------------------------------------------------------------
    //取屏幕 屏幕分辨率 信息
    public static GetScreen()
    {
        //https://blog.csdn.net/weixin_32246721/article/details/112640932

        //获取屏幕分辨率
        //let frame = cc.view.getFrameSize(); //use screen.windowSize instead
        let frame = cc.screen.windowSize;

        //获取宽高比
        let ratio = frame.width / frame.height;

        // let fitHeight = true;
        // let fitWidth = false;

        // //设置适配模式
        // cc.Canvas.instance.fitHeight = fitHeight;
        // cc.Canvas.instance.fitWidth = fitWidth;


        return frame;
    }//

    //设置一个左上角位置
    //其实从 https://blog.csdn.net/qq_45021180/article/details/104662689 看，应该用 Widget 组件处理位置对齐
    public static SetPos_as_Windows()
    {

    }//


    //----------------------------------------------------------------

    public static start() {


        //----------------------------------------------------------------
        alert("functions_ui.start()");

        //----------------------------------------------------------------

    }//

    update(deltaTime: number) {
        
    }//

    //3.6 的写法 //按键事件
    OnKeyDown(param: any) {

        //cc.EventKeyboard  //从浏览器调试可知 param 其类型为 EventKeyboard
        //{
        //     "type": "keydown",
        //     "bubbles": false,
        //     "target": null,
        //     "currentTarget": null,
        //     "eventPhase": 0,
        //     "propagationStopped": false,
        //     "propagationImmediateStopped": false,
        //     "keyCode": 87,
        //     "_isPressed": true
        // }

        alert(param);
        
    }//


}//class

export {functions_ui}


