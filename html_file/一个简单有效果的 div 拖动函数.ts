
//一个简单有效果的 div 拖动函数

//https://blog.csdn.net/Lou__Lan/article/details/127820475
//让元素跟随鼠标移动
function MakeDivCanMove(goods:HTMLElement)
{        
    // let goods = document.createElement('div');
    // document.body.appendChild(goods);
    // goods.setAttribute('id', 'goods')
    goods.onmousedown = function (e) { // 给元素绑定鼠标按下事件
        // 当鼠标按下时，获取鼠标相对于元素的坐标
        let mousedownX = e.offsetX;
        let mousedownY = e.offsetY;
        document.onmousemove = function (event) {
            // 当鼠标移动时，获取鼠标移动的坐标
            let mousemoveX = event.clientX
            let mousemoveY = event.clientY

            // 把鼠标移动的位置重新赋值给 元素的位置  并且减去鼠标在元素上按下时的初始坐标可以让元素一直处于移动状态
            goods.style.left = (mousemoveX - mousedownX) + 'px'
            goods.style.top = (mousemoveY - mousedownY) + 'px'
        }
    }
    // 在新位置 结束移动事件，让元素停在新位置
    document.onmouseup = function () {
        // 删除鼠标的移动事件
        document.onmousemove = null;
    }
}//


//对我们的设计器并不是很准确，所以要换一个版本
function MakeDivCanMove_v2(div:HTMLElement)
{        
    div.onmousedown = function (e) { // 给元素绑定鼠标按下事件
        // 当鼠标按下时，获取鼠标相对于元素的坐标
        //let mousedownX = e.offsetX;
        //let mousedownY = e.offsetY;

        //https://blog.csdn.net/weixin_33935505/article/details/93151846
        //event.offsetX、event.offsetY
        //鼠标相对于事件源元素（srcElement）的X,Y坐标，只有IE事件有这2个属性，标准事件没有对应的属性

        //event.screenX、event.screenY
        //鼠标相对于用户显示器屏幕左上角的X,Y坐标。标准事件和IE事件都定义了这2个属性

        //event.clientX、event.clientY
        //鼠标相对于浏览器窗口可视区域的X，Y坐标（窗口坐标），可视区域不包括工具栏和滚动条。IE事件和标准事件都定义了这2个属性

        //记录下两个鼠标位置的差就可以了
        let mousedownX = e.screenX;
        let mousedownY = e.screenY;

        let lastX = div.offsetLeft;
        let lastY = div.offsetTop;

        document.onmousemove = function (event) {

            event.preventDefault();  //取消默认行为，目的是希望在拖动控件时不选中文字

            // 当鼠标移动时，获取鼠标移动的坐标
            //let mousemoveX = event.clientX
            //let mousemoveY = event.clientY
            let mousemoveX = event.screenX;
            let mousemoveY = event.screenY;

            // 把鼠标移动的位置重新赋值给 元素的位置  并且减去鼠标在元素上按下时的初始坐标可以让元素一直处于移动状态
            div.style.left = lastX + (mousemoveX - mousedownX) + 'px';
            div.style.top  = lastY + (mousemoveY - mousedownY) + 'px';


            //关于offsetLeft和left的区别（关于offsetLeft：https://developer.mozilla.org/zhCN/docs/Web/API/HTMLElement/offsetLeft）
            // 1.定义了position：relative或absolute属性的元素才有left属性，元素都有offsetLeft属性。
            // 2.元素内联样式中设置了left，才能通过div.style.left获取；offsetLeft直接通过div.offsetLeft获取。
            // 3.left可读可写，获取到的是字符串；offsetLeft只读，获取到的是数字。
            // 相同点：定位方式一样，如果父元素设置了定位元素（position设置为relative或absolute），则相对于定位元素定位，否则就是相对于根元素定位。


        }//event move
    }//event down

    // 在新位置 结束移动事件，让元素停在新位置
    document.onmouseup = function () {
        // 删除鼠标的移动事件
        document.onmousemove = null;
    }//event up
}//


