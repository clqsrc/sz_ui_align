

class Car { 
    // 字段 
    engine:string; 
 
    // 构造函数  
    constructor(engine:string) { 
        this.engine = engine 
    }  
 
    // 方法 
    disp():void { 
        console.log("发动机为 :   "+this.engine) 
    } 
}


let c:Car = new Car("777");


c.disp();


//--------------------------------------------------------
//可以参考在线版 https://typescripttolua.github.io/play
//在线版转换出来的更简单清晰

//set PATH=D:\no_install\node-v16.15.1-win-x64;%PATH%
//npx typescript-to-lua class1.ts
//tsc dom1.ts --allowJs
//npx typescript-to-lua dom1.ts --allowJs

//或者是发布是只复制 local function __TS__Class(self) 这样的函数过去就可以了

//浏览器的话要用 webpack 打包

//--------------------------------------------------------
// https://crayon-shin-chan.blog.csdn.net/article/details/79069992
// 使用ts-node直接运行ts项目
// 这里指的是整个项目使用ts写，不进行编译，直接运行
// index.js
// require('ts-node/register');
// require('./server');
// 先使用ts-node/register进行注册，然后直接运行server.ts脚本
//--------------------------------------------------------
// node不能直接运行ts,需要先将ts转成js，然后再运行，
// 但是用下面的东西就可以直接在node上运行ts了。
// nodejs环境运行ts
// npm install @types/node --save-dev
// npm install ts-node -g       
// 运行方法：
// ts-node 文件名.ts