

console.log("hi");

//目标：将 foo 标识符 转换为 bar 标识符
//首先，我们要将代码解析为 AST，以 let foo = 42 为例：

import * as ts from 'typescript'

//const code = "let foo = 1;" // 要转换的代码

const code = 
`

let foo = 1;

class Node {
    a1:string;
    getSourceFile(): string;
}

`
// 要转换的代码

const ast = ts.createSourceFile(
    /* filename */'dummy.ts',
    /* sourceText */ code,
    ts.ScriptTarget.Latest
) // 解析后的 AST


// 可以看到，调用 TypeScript 编译器解析的过程非常简单。接下来，我们创建一个非常简单的 transformer：

const transformerFactory: ts.TransformerFactory<ts.Node> = (context: ts.TransformationContext) => {
    return visitor

    function visitor(node: ts.Node): ts.Node {

        //--------------------------------------------------------
        //clq 按理说这里可以得到遍历整个 ts ast 节点的过程了。
        //应该是可以转换成其他语言了的。这个例子要比 ts 的官方例子简单得多，ts 官方例子应该已经过时，是编译不了的。

        //另外，另外一篇文章提到过 ts 中的 ast 为了性能，将类型用整数来表示了（babel 它们可能是字符串？）
        //https://ts-ast-viewer.com/
        //再结合这个网站，基本上就可以写一个自己的语言转换器了

        //另外用 "typescript to" 搜索 github 时，看到
        //https://github.com/itsdouges/typescript-transformer-handbook
        //似乎是非常好的教程

        console.log("node.kind:", node.kind);  //export enum SyntaxKind 中有详细说明  
        //F:\test1\ts1\ts2lng1\node_modules\typescript\lib/typescript.d.ts

        if (ts.isIdentifier(node))
        {
            let _node:ts.Identifier = node;
            console.log("node.text:", node.text);
        }//


        //--------------------------------------------------------


        if (ts.isIdentifier(node) && node.text === "foo") { // 判断是否是 foo 标识符
            return ts.createIdentifier("bar") // 如果是则替换为 bar 标识符
        }
        return ts.visitEachChild(node, visitor, context) // 否则继续遍历其他节点
    }
};


// 在开发 transformer 的过程中，可能涉及许多 TypeScript 的数据结构，我们可以使用 ts-ast-viewer 查看 AST 的结构并且获取生成对应代码的工厂函数。 然后我们尝试转换并生成代码：
//https://ts-ast-viewer.com/

const result = ts.transform(ast, [transformerFactory]) // 转换 AST
const node = result.transformed[0]

// 由 AST 生成代码
const printer = ts.createPrinter()
const codeAfterTransform = printer.printNode(ts.EmitHint.Unspecified, node, ast);

console.log(codeAfterTransform) // let bar = 1;





//--------------------------------------------------------
console.log("hi");

/*

linux搜索含有指定字符的文件

https://www.cnblogs.com/miracle-luna/p/11037614.html

msys2 中

cd /d/test1
grep "babel" ./* -rn


--------------------------------------------------------
https://zhuanlan.zhihu.com/p/145278299

传统方式：transformer API

TypeScript 目前提供了将代码解析为 AST, 并且进行转换的 API。 让我们通过一个简单的例子了解它



*/

