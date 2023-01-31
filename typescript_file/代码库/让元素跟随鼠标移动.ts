

//--------------------------------------------------------
//2023

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

//--------------------------------------------------------

