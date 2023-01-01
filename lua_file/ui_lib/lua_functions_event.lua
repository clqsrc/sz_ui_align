

-- //lua 语言自身源码写成的通用支持函数

c2l_self_require("ui_lib/lua_functions_event_new.lua");


-- //------------------------------------------------
-- //try 的实现。参考 https://www.cnblogs.com/tboox/p/12014656.html
-- //pcall 和 xpcall、debug https://www.runoob.com/lua/lua-error-handling.html


--//------------------------------------------------
--require(package)


--//------------------------------------------------

-- 传递事件参数的对象和函数的中间类
UiEvent = {}
UiEvent.__index = UiEvent --元表的__index字段很重要

-- UiEvent.obj = nil;   --//谁监听的事件
-- UiEvent.func = nil;  --//事件触发时要执行的函数

function UiEvent:new()
    local obj = {}
    setmetatable(obj, UiEvent)

    -- 似乎应该放这里来
    UiEvent.class_name = "UiEvent"; -- 类名
    UiEvent.obj = nil;   --//谁监听的事件
    UiEvent.func = nil;  --//事件触发时要执行的函数

    return obj
end

---------------------------------------------------
-- //简化参数的设置 //2022.12 方便 Set_UI_OnEvent_Func 或者其他类似函数调用
function MakeEventObj(func, param)

    local eobj = UiEvent:new(); 

    eobj.obj = param;
    eobj.func = func;

    return eobj;

end;

-- //ts 是不支持返回返回值的（严格来说是无法正确转换成 lua 的）
function GetEventObj_func(eobj)

    --//return eobj.obj;  //这个是简单写法，其实可以判断

    local func = eobj;

    local theType = type(func);

    local obj = eobj;

    --ShowMessage(tostring(eobj));
    --ShowMessage(theType);
    
    if theType == "table" then
        -- 是Lua表. 也就是对象
        --obj.onSize(obj, obj);
        if "UiEvent" == obj.class_name then
            local event = UiEvent:new();
            event = obj;

            return event.func;

        end;

    end;
        
    return func;

end;

function GetEventObj_param(eobj)

    --//return eobj.obj; //这个是简单写法，其实可以判断

    local func = eobj;

    local theType = type(func)

    local obj = eobj;
    
    if theType == "table" then
        -- 是Lua表. 也就是对象
        --obj.onSize(obj, obj);
        if "UiEvent" == obj.class_name then
            local event = UiEvent:new();
            event = obj;

            --//return event.func;
            return event.obj;

        end;

    end;
        
    return nil;

end;

-- ------------------------------------------------

-- //保存成员函数指针

SaveFunc_index = 0; -- 不加 local 就是全局变量
SaveFunc_hasmap = {};

--//调用 c 语言函数时先将要回调的函数和参数存成一个对象保存在一个 hashmap 中，然后将它的 key 传递给 c 函数就可以了
--//c 语言在事件发生时将会告诉我们这个 key，然后我们根据这个 key 取回函数和参数，并调用
function SaveFunc(func)

    SaveFunc_index = SaveFunc_index + 1;

    local key = SaveFunc_index;

    SaveFunc_hasmap[tostring(key)] = func;

    return tostring(key);
end

--//当 c 语言的事件函数回调时，用 c 言语传递过来的 index 找到我们设置时的对象
function GetFunc(index)

    -- //SaveFunc_index = SaveFunc_index + 1;

    local key = index;

    local func = SaveFunc_hasmap[key];

    return func;
end

--//2021.11.10 ok
function UI_OnSize(s_key)

    --//c2l_ShowMessage(s_key); --//ok

    --//c2l_ShowMessage('UI_OnSize()'); --//ok

    

    local func = GetFunc(s_key);  --//用来取出对象是可以的，用来取函数还是不行

    --c2l_ShowMessage(tostring(func));

    --//local CarMainForm 
    local obj = func;
    --CarMainForm(obj).onSize(obj);

    --local obj = CarMainForm:new();
    --obj.onSize(obj, obj); --//不行//当作这个类的话是可以的
    --obj.onSize2(obj);  --//奇怪，这样是 ok 的
    ----CarMainForm.onSize(obj, obj)  --//这样是最好的，不用强制类型转换

    --ShowMessage(type(obj));  --//没有用，这个是 "table"

    --ShowMessage(tostring(obj.aaa));  --//既然是 table ，那么任意取一个键值 //不存在的键值的话会显示为 nil

    local theType = type(func)
    if theType == "number" then
    -- 是数字
    elseif theType == "string" then
    -- 是字符串
    elseif theType == "table" then
        -- 是Lua表. 也就是对象
        --obj.onSize(obj, obj);
        if "UiEvent" == obj.class_name then
            local event = UiEvent:new();
            event = obj;
            event.func(event.obj);
        else
            obj.onSize(obj);
        end
        
    elseif theType == "function" then
        -- 是Lua函数
        func();
    end


end    

--//2022 因为 tsc 给成员函数都生成了 self 参数，所以不得不修改，其他事件函数也一样要修改。不过其实也就是判断一下参数个娄就可以了
function UI_OnClick(s_key)

    local func = GetFunc(s_key);  --//用来取出对象是可以的，用来取函数还是不行

    local obj = func;

    --ShowMessage("UI_OnClick:" .. tostring(s_key)); 
    --2022 for qt //似乎 qt 下没有传递这个参数过来,有的，是参数传给 lua 时的问题
    --ShowMessage("UI_OnClick:" .. type(obj));  --//没有用，这个是 "table"

    local theType = type(func)
    if theType == "number" then
    -- 是数字
    elseif theType == "string" then
    -- 是字符串
    elseif theType == "table" then
        -- 是Lua表. 也就是对象
        --obj.onSize(obj, obj);
        if "UiEvent" == obj.class_name then
            local event = UiEvent:new();
            event = obj;

            --//2022 tsc 生成的 lua 会自带 self 参数，所以这时候 obj 不能给第一个参数 //那么怎么知道这个 func 是有两个参数的呢？
            --//debug.getinfo(f).nparams给出f中的参数数量
            local count = debug.getinfo(event.func).nparams;
            --ShowMessage("UI_OnClick: count=" .. tostring(count));
            --ShowMessage("UI_OnClick: event.obj=" .. tostring(event.obj));
            --event.func(event.obj);

            --//其实可以直接调用两个参数，动态语言多给一个参数也是可以的
            --event.func(event.obj, event.obj);
            if (2 == count) then
                event.func(event.obj, event.obj);
            else
                event.func(event.obj);
            end;
        end
    elseif theType == "function" then
    -- 是Lua函数
        func();
    end


end;

--//这是给 C 语言接口调用的函数
--//通用的响应函数，比如 http
--//s_key 是 lua 调用时传入的，event_key 和 event_value 是对应的 c 语言事件中传递过来的
--//官方文档 http://newbt.net/ms/vdisk/show_bbs.php?id=DA71D3887B11AAA56B6C4FCCC8795DDB&pid=161
function UI_OnEvent(s_key, event_key, event_value)

    local func = GetFunc(s_key);  --//用来取出对象是可以的，用来取函数还是不行

    local obj = func;

    --ShowMessage("UI_OnEvent:" .. type(obj));  --//没有用，这个是 "table"

    local theType = type(func)
    if theType == "number" then
        -- 是数字
        ShowMessage("UI_OnEvent()" .. "是数字不是函数。");
    elseif theType == "string" then
        -- 是字符串
        ShowMessage("UI_OnEvent()" .. "是数字不是函数。");
    elseif theType == "table" then
        -- 是Lua表. 也就是对象
        --obj.onSize(obj, obj);
        if "UiEvent" == obj.class_name then
            local event = UiEvent:new();
            event = obj;
            event.func(event.obj, event_key, event_value);
        end
    elseif theType == "function" then
        -- 是Lua函数
        func(event_key, event_value);
    else

        --//特殊的处理
        if "AppOnSize" == event_key then
            AppOnSize();  --//全局的大小变化事件
            return;
        end; --//if 2

        --------------------------------------------------------
        --//简化的鼠标按下事件 //这时的 s_key 并没有从 lua 中通过事件设置转递过去，而是在 qt 中直接将
        --//QWidget * view 的指针转换为 string.对于无法直接使用指针的情况双方定一个指针转换成数字的统一方法就可以了(比如 java 中)
        if "mouse_down" == event_key then
            --ShowMessage("UI_OnEvent()" .. "鼠标按下事件"..event_value);

            --//这时的 s_key 直接是 c 语言里的 view 对象指针，对于 java 有可能是代替指针的虚拟指针的整数值。不管是哪个都可以当做 view
            --//直接用相关函数操作。因为 creatview 和 getview 时也要返回这个值
            --//View_Hide(s_key);  --//可以隐藏它看看是不是这么回事 //ok 确实成功了，即使没有在 lua 中先设置事件

            return;
        end; --//if 2

        if "mouse_down_left" == event_key then
            ShowMessage("UI_OnEvent() mouse_down_left" .. "鼠标左键按下事件"..event_value);

            local view = s_key;

            --//local func2 = Get_UI_OnEvent_Func(view, "mouse_down_left");
            --//func2();

            local eobj = Get_UI_OnEvent_Func(view, "mouse_down_left");

            local func2 = GetEventObj_func(eobj);
            local obj2 = GetEventObj_param(eobj);

            func2(nil, obj2);  --//将取出的参数放到第 2 个参数的位置，因为 ts 转换的 lua 函数第一个参数可能是默认带有的 self,为了最大兼容性最好忽略掉第一个参数

            return;
        end; --//if 2

        if "mouse_down_right" == event_key then

            ShowMessage("UI_OnEvent() mouse_down_right" .. "鼠标右键按下事件"..event_value);

            local view = s_key;

            --//local func2 = Get_UI_OnEvent_Func(view, "mouse_down_right");
            --//func2();

            local eobj = Get_UI_OnEvent_Func(view, "mouse_down_right");

            local func2 = GetEventObj_func(eobj);
            local obj2 = GetEventObj_param(eobj);

            func2(nil, obj2);  --//将取出的参数放到第 2 个参数的位置，因为 ts 转换的 lua 函数第一个参数可能是默认带有的 self,为了最大兼容性最好忽略掉第一个参数

            return;
        end; --//if 2

        

        --------------------------------------------------------

        ShowMessage("UI_OnEvent()" .. "什么都不是，不是函数。func:" .. tostring(func) .. " s_key:" .. tostring(s_key) .. " event_key:" .. tostring(event_key));
    end;


end;

--//自定义 loader
--package.searchers
--function new_lua_loader
--[[
    package.searchers[2] = function( name )
        print("try to load", name )
    end
    package.searchers[3] = nil
    package.searchers[4] = nil
    
    --require "libtest"
]]
--end

--[[
package.searchers[2] = function( name )
    --ShowMessage("try to load");
    print("try to load", name )

    --//package.loaded[modulename] = nil
    --package.loaded[name] = 110
    package.loaded[name] = {}

    return package.loaded[name];

end
--package.searchers[1] = searchers[2] --nil
--//package.searchers[2] = nil
package.searchers[3] = nil
package.searchers[4] = nil
]]

--//按上面的方法结合 https://blog.csdn.net/Yueya_Shanhua/article/details/52241544
--应该是可以直接替换掉 loader 的

--ShowMessage("package.loaders =" .. tostring(package.loaders)); //可能是版本的，原因 5.4.3 下并不能这样.可以看到这个字段是 nil

--package.searchers[1] = searchers[2] --nil
--//package.searchers[2] = nil
--package.searchers[3] = nil
--package.searchers[4] = nil

