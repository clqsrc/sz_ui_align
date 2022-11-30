import { _decorator, Component, Node } from 'cc';
const { ccclass, property } = _decorator;

import * as cc from 'cc';

import { functions_camera } from './functions_camera';
import { functions_ui } from './functions_ui';


@ccclass('t1')
export class t1 extends Component {



    camera: cc.Node | null = null;  //奇特的语法

    @property({ type: Node })
    public startMenu: Node | undefined;


    //要象 u3d 一样，把场景中的摄像头拖放到这个属性上来
    @property({ type: cc.Camera })
    public mainCamera: cc.Camera | undefined;


    start() {

        //alert("aaa中文");

        //document.body.innerHTML = "aaaaaaaaaaaaaaaaaaaaaaaaaaa<input></input>";

        //----------------------------------------------------------------
        //注册全局触摸点击事件
        //cc.systemEvent.on(Node.EventType.TOUCH_END, () => {
        //    this.shoot();
        //});

        // cc.SystemEvent .systemEvent.on(Node.EventType.TOUCH_END, () => {
        //     this.shoot();
        // });

        //----------------------------------------------------------------
        // 找到摄像机
        //this.camera = cc.find("Canvas/Main Camera");
        this.camera = cc.find("Main Camera");  //ok
        // 开始触摸
        //this.node.on(cc.Node.EventType.TOUCH_START, this.onTouchStart, this);
        // 触摸中
        //this.node.on(cc.Node.EventType.TOUCH_MOVE, this.onTouchMove, this);

        //this.node.on(cc.SystemEvent.EventType.KEY_DOWN, this.OnKeyDown, this);

        //https://blog.csdn.net/qq_33514421/article/details/120813771 //过时

        //let obj_island = cc.find("islands");
        let obj_island = cc.find("Cube");
        let btnStart = cc.find("Canvas/btnStart");

        //functions_ui.SetOnClick(this, this.OnClick);  //点击事件
        //functions_ui.SetOnClick(obj_island, this.OnClick);  //点击事件  //这个效果比较奇怪
        functions_ui.SetOnClick(btnStart, this.OnClick);  //点击事件

        //functions_ui.SetOnClick(obj_island, this.OnClick2);  //点击事件  //奇怪，对 3d 物体不能这样

        functions_ui.SetOnClick_for3D(this.mainCamera, obj_island, this.OnClick2);  //要加上物理/BoxCollider

        functions_ui.SetOnClick_for3D(this.mainCamera, cc.find("mship1/ship1"), this.OnClick3);  //要加上物理/BoxCollider
        functions_ui.SetOnClick_for3D(this.mainCamera, cc.find("money_light1"), this.OnClick4);  //要加上物理/BoxCollider

        //奇怪，这个不行
        //functions_ui.SetOnClick_for3D(this.camera, obj_island, this.OnClick2);

        return;

        //下面的方法是 ok 的
        cc.input.on(cc.Input.EventType.MOUSE_DOWN, (touch)=>{

            let pos = touch.getLocation();

            //let camera:cc.Camera = this.camera as unknown as cc.Camera;
            let camera:cc.Camera = this.mainCamera;

            let ray = camera.screenPointToRay(pos.x, pos.y); //从摄像头取得的射线

            //要触发的物体要有 BoxCollider //https://docs.cocos.com/creator/manual/zh/physics/physics-collider.html
            //通过代码添加
            // import { BoxCollider } from 'cc'
            // const boxCollider = this.node.addComponent(BoxCollider);

            if (cc.PhysicsSystem.instance.raycastClosest(ray))  //应该是判断是否选中了物体
            {
                const res = cc.PhysicsSystem.instance.raycastClosestResult;  //应该是取得射线选中的结果

                let hitnode = res.collider.node;  //射线选中的物体

                alert(hitnode.name);

            }

        });

        return;

        //下面的方法是 ok 的
        cc.input.on(cc.Input.EventType.KEY_DOWN, this.OnKeyDown, this); //ok //https://blog.csdn.net/qq_36286039/article/details/125395200
        //奇怪，官方文档在哪里
        //https://docs.cocos.com/creator3d/manual/zh/getting-started/first-game/?h=input
        //中的示例代码居然是错误的
        //从以下路径找到的文档是最新的
        //1 https://github.com/cocos/cocos-tutorial-mind-your-step
        //2 https://docs.cocos.com/creator/manual/en/getting-started/first-game/
        //3 https://docs.cocos.com/creator/manual/zh/getting-started/first-game/


        //----------------------------------------------------------------
        //加载模型 //据说要用 fbx 格式或者是 ab 模型
        /*
        cc.assetManager.loadBundle("A", (err, ab: cc.AssetManager.Bundle) => {

            //释放ab包，不会释放从ab包中加载的资源
            // 释放ab包
            //assetManager.removeBundle(ab)
            
            //从ab包加载图集资源
            ab.load('test/abc', () => {
            
            });
            
            //加载声音文件
            
            //ab.load(path, 类型, 回调)
            //})
        });
        */
        //----------------------------------------------------------------
        //加载 model/islands 的 fbx 模型 //1 实例 kl
        if (false) {
            let a = new cc.Node();
            //a.addComponent

            ////var node = cc.instantiate(this.target);

            //var parent = cc.find('Canvas/bg/button'); //返回的父节点
            var parent: cc.Node | null = cc.find('islands'); //返回的父节点
            //var parent:cc.Node;
            //parent = null;
            //if (!parent) return;

            var node: cc.Node = cc.instantiate(parent); //克隆指定的任意类型的对象


            // 将实例化的节点加入到父节点中去
            //node.parent = parent;
            parent.parent.addChild(node);

            node.setPosition(0, 0, 0);

            //functions_camera.start();  //ok
        }//
        //----------------------------------------------------------------
        //加载 model/islands 的 fbx 模型 //2 按名字加载
        //动态加载的官方方法 //似乎一定要把资源放到 resources 目录  //https://docs.cocos.com/creator/manual/zh/asset/dynamic-load-resources.html
        //所有需要通过脚本动态加载的资源，都必须放置在 resources 文件夹或它的子文件夹下。resources 文件夹需要在 assets 根目录 下手动创建
        let a = new cc.Node();


        // 加载 Prefab
        cc.resources.load("model_copy/helloWorld/islands", cc.Prefab, (err, prefab) => {
            //const newNode = cc.instantiate(prefab);
            //this.node.addChild(newNode);

            //----
            var parent: cc.Node | null = cc.find('islands'); //返回的父节点
            //var node:cc.Node = cc.instantiate(parent); //克隆指定的任意类型的对象
            var node: cc.Node = cc.instantiate(prefab); //克隆指定的任意类型的对象


            // 将实例化的节点加入到父节点中去
            //node.parent = parent;
            parent.parent.addChild(node);

            node.setPosition(10, 0, 0);

            //functions_camera.start();  //ok



        });



        //----------------------------------------------------------------

    }//

    //类似于 u3d 的函数
    update(deltaTime: number) {

        //return;

        //----------------------------------------------------------------
        //参考 http://newbt.net/ms/vdisk/show_bbs.php?id=5E52705D239AD4E759EF9413ACBA9367&pid=164
        //中的 u3d 示例，应该有

        //let pos1:cc.math.Vec3 = {};
        //let pos1:cc.Vec3 = {};

        let cur_pos = this.mainCamera.node.position;

        let new_pos = cur_pos.clone();

        new_pos.x = new_pos.x + 1;  //cocos 中的位置单位是什么？米？

        let speed = 0.005;
        speed = 1;     //表示每秒移动一个单位
        cc.Vec3.lerp(new_pos, cur_pos, new_pos, deltaTime * speed);

        //cocos 的函数比较奇怪，这个大概相关于 u3d 中的 新位置 = lerp(起点，终点，速度)

        this.node.setPosition(new_pos);

        return;
        //----------------------------------------------------------------
        //照抄，小改，还不知道原理，以后再修改、注释
        /*
        let cur_pos = this.mainCamera.node.position;

        let new_pos = this.mainCamera.node.position;

        let offset = {
            x: 1,
            y: 1,
            z: 0,
        };

        offset = { x: 0.1, y: 0, z: 0 };  //很重要，改为这个会变慢

        let speed = 0.005;  //这个似乎影响不大
        //speed = 1;  //这个似乎影响不大


        //http://t.zoukankan.com/creator-star-p-11670019.html
        //计算要移动的目标位置
        //cc.Vec3.add(new_pos, cur_pos, this.offset);
        cc.Vec3.add(new_pos, cur_pos, offset);
        //插值计算
        //Vec3.lerp(this.point, this.node.position, this.point, deltaTime * this.speed);
        cc.Vec3.lerp(new_pos, cur_pos, new_pos, deltaTime * speed);
        
        //移动节点
        //this.node.setPosition(this.point);
        this.node.setPosition(new_pos);
        //*/

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

    //对象的点击事件
    OnClick(param: any) {

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

        alert(param + "选中按钮");

    }//

    //对象的点击事件
    OnClick2(param: any) {

        ////
        alert(param + "选中小岛");

    }//

    //对象的点击事件
    OnClick3(param: any) {

        ////
        //alert(param + "选中小岛2");

        cc.find("Canvas/txtInfo").getComponent(cc.RichText).string = "ok 拾取一个能量块";

        

    }//

    //对象的点击事件
    OnClick4(param: any) {

        ////
        //alert(param + "选中小岛2");

        cc.find("Canvas/txtInfo").getComponent(cc.RichText).string = "得到 100 金币";

        

    }//


}//class




