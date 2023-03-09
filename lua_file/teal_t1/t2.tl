
--//目前 teal vscode 插件只能提示代码，还不能调试

local s:string = "aaa";

local function f1(fn:string)

    print(fn:byte(2));

end;


print("aaa 2" .. s);

f1("ccc");
