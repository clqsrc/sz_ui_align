

-- //lua 语言自身源码写成的通用支持函数



-- ------------------------------------------------

-- //保存成员函数指针

--//SaveFunc_index = 0; -- 不加 local 就是全局变量
UI_OnEvent_List = {};

--//设置 UI_OnEvent 时要调用的函数和参数 //obj 可以是 UiEvent, func 等，设置时是什么，取出时就是什么
function Set_UI_OnEvent_Func(view, event_name, obj)

    local event_key = event_name;

    --SaveFunc_index = SaveFunc_index + 1;

    --local key = SaveFunc_index;

    local index = "view:" .. view .. ", event_name:" .. event_name;

    local key = index;
    local func = obj;

    UI_OnEvent_List[tostring(key)] = func;

    return tostring(key);
end

--//UI_OnEvent 触发时根据传入的内容取出设置的 obj (如果有设置的话)
function Get_UI_OnEvent_Func(view, event_name)

    -- //SaveFunc_index = SaveFunc_index + 1;

    local index = "view:" .. view .. ", event_name:" .. event_name;

    local key = index;

    local func = UI_OnEvent_List[key];

    return func;

end;



