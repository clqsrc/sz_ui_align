
//nodejs 环境下才有的功能函数
//http://t.zoukankan.com/lingnweb-p-9945027.html
var fs = require('fs');

/*
//读文件
fs.readFile('package.json', function (err, data) {
    if (err) {
        document.write( err.message);
        return;
    } else {
        document.write("aaa");
        console.log(data);
    }
});//

*/


function LoadFromFile(fn)
{
    //fs.readFileSync // 同步版的 fs.readFile() 。
    
    return fs.readFileSync(fn).toString();

    //----


    //读文件 //这是异步的，怎样变成同步
    fs.readFile(fn, function (err, data) {
        if (err) {
            document.write( err.message);
            return;
        } else {
            //document.write("aaa");
            console.log(data);

            //---- data 是 buffer 如何转换成字符串
            let s = data.toString();

            alert(s);

        }
    });//

}//

function SaveToFile(fn, s, msg_show)
{
    // fs.writeFileSync(fn, s, (err) => {
           
    //     if (err) throw err;
           
    //     console.log('The file has been saved!');

    // });

    fs.writeFile(fn, s, (err) => {
           
        if (err){
            
            if (1 == msg_show)
            alert("保存失败！" + fn);
            
            throw err;
        }//
           

        //console.log('The file has been saved!');
        if (1 == msg_show)
        alert("保存成功！" + fn);
        
    });
         
    //return fs.readFileSync(fn).toString();

}//


//--------------------------------------------------------
//下面实际上是 nwjs 的接口，与 electron 并不兼容

//nwjs 窗口最大化
function NW_WindowMax()
{
    //js获取屏幕对象使其最大化并且展示
    var win=require('nw.gui').Window.get(); 
    win.maximize();
    win.show();
    
        
}//


/* electron的源代码保护不行

nw可以讲源代码编译成assembly，能保护源代码

光凭这一条，nw完爆electron */

//nwjs 取命令行参数
//gui.App.argv
//http://docs.nwjs.io/en/latest/
function NW_GetCmd()
{
    var gui = require('nw.gui');
    //console.log(gui.App.argv);//['-port', '8080']
    
    //alert(gui.App.argv);

    //return gui.App.argv; //这时候是一个数组

    //return gui.App.argv.toString();
    return gui.App.argv[0].toString();  //这个要更合理,特别是假如 nwjs 为解决多实例，要使用 --user-data-dir=%APPDATA%\\test", 这样的附加参数时
        
}//

