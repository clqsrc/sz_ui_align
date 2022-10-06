

-- //c 语言的接口函数

-- //这里只是写接口，基本上只是换个名称

-- 因为 lua 默认不支持 int64 所以参数全部是字符串

json = c2l_self_require("ui_lib/json.lua");
c2l_self_require("ui_lib/c_functions_blle.lua");

-- 两个参数，一个是父窗口，一个是文件名
function UI_CreateFromJson(parent, fn)

    return c2l_UI_CreateFromJson(parent, fn);
    --//return "0";
end


-- 设置大小变化事件
-- 两个参数，一个是父窗口，一个是函数，返回一个整数
function UI_SetOnSize(parent, func)

    return c2l_UI_SetOnSize(parent, func)
    --//return "0";
end

-- 自定义的 require 函数
function self_require(filename)

    local fn = filename .. ".lua";

    --//package.loaded[name] = {};

    return c2l_self_require(fn);
end

--//2022 //试图解决 lualib_bundle.lua 的问题
function require(filename)

    --return self_require(filename);

    local fn = filename .. ".lua";

    return c2l_self_require(fn);  --//奇怪，这时候 self_require 还不能用，要用原始的 c2l_self_require 才行

    --//self_require("lualib_bundle.lua");
    -- self_require("lualib_bundle_self.lua");

    -- local ____lualib = lualib_bundle_self_return();

    -- return ____lualib;

end


function long_ref(obj)

    return c2l_long_ref(obj);
    --//return "0";
end

--//取当前的系统语言
--//默认 "cn" ， "en" 则是英语
function GetOSLanguage(uijson_handle, id)

    return c2l_GetOSLanguage();
    --//return "0";
end

function UI_GetView(uijson_handle, id)

    return c2l_UI_GetView(uijson_handle, id);
    --//return "0";
end


function UI_GetRootView(uijson_handle)

    return c2l_UI_GetRootView(uijson_handle);
    --//return "0";
end

function UI_AlignControl(uijson_handle)

    return c2l_UI_AlignControl(uijson_handle);
    --//return "0";
end

--//动画移动一个 view
function View_Anima_Move(view, left, top, time_second)
    return c2l_View_Anima_Move(view, left, top, time_second);
end;

function View_GetRect(view)
    return View_GetRect_dp(view);
end;

function View_GetRect2(view)

    --return View_GetRect_dp(view);

    local s_json = c2l_View_GetRect(view);

    --ShowMessage(s_json);

    --ShowMessage(tostring(json));

    --s_json = '{left:100, top:100}'; --//no
    --s_json = '{"left":100, "top":100, "width":150}'; --//ok 即 lua 的 json 一定要把 key 加双引号


    --//太容易出错，保护起来吧
    --函数执行成功返回2个值，分别为true和另外一个函数的返回值,如果没有返回值则返回nil
    --函数执行失败返回2个值，分别为false和出错信息

    local r, j = pcall(function (str_json)

        local obj_json = json.decode(str_json);

        --ShowMessage(str_json);
        --ShowMessage(obj_json.width);

        obj_json.left = tonumber(obj_json.left); 
        obj_json.top = tonumber(obj_json.top); 
        obj_json.width = tonumber(obj_json.width); 
        obj_json.height = tonumber(obj_json.height); 

        return obj_json;

    end, s_json);

    --ShowMessage("r:" .. tostring(r) .. " result:" .. j.width);

    if r then
        return j;
    else
        return {left=0, top=0, width=0, height=0};
    end


    --local j = json.decode(s_json);

    --ShowMessage(j.width);

    -- return j;
    --//return "0";
end

function View_GetRect_dp(view)

    local s_json = c2l_View_GetRect_dp(view);

    --ShowMessage(s_json);

    --ShowMessage(tostring(json));

    --s_json = '{left:100, top:100}'; --//no
    --s_json = '{"left":100, "top":100, "width":150}'; --//ok 即 lua 的 json 一定要把 key 加双引号


    --//太容易出错，保护起来吧
    --函数执行成功返回2个值，分别为true和另外一个函数的返回值,如果没有返回值则返回nil
    --函数执行失败返回2个值，分别为false和出错信息

    local r, j = pcall(function (str_json)

        local obj_json = json.decode(str_json);

        --ShowMessage(str_json);
        --ShowMessage(obj_json.width);

        return obj_json

    end, s_json);

    --ShowMessage("r:" .. tostring(r) .. " result:" .. j.width);

    if r then
        return j;
    else
        return {left=0, top=0, width=0, height=0};
    end


    --local j = json.decode(s_json);

    --ShowMessage(j.width);

    -- return j;
    --//return "0";
end

function View_SetRect(view, rect)

    return View_SetRect_dp(view, rect);
end;

function View_SetRect2(view, rect)

    local left   = tonumber(rect.left) --5.3 之后的 int64 类型
    local top    = tonumber(rect.top)
    local width  = tonumber(rect.width)
    local height = tonumber(rect.height)

    c2l_View_SetRect(view, left, top, width, height);

end

function View_SetRect_dp(view, rect)

    local left   = tonumber(rect.left) --5.3 之后的 int64 类型
    local top    = tonumber(rect.top)
    local width  = tonumber(rect.width)
    local height = tonumber(rect.height)

    c2l_View_SetRect_dp(view, left, top, width, height);

end

--//保存一个值到手机中
function SetSaveValue(key, value)

    c2l_SetSaveValue(key, value);

end

function GetSaveValue(key)

    return c2l_GetSaveValue(key);

end

--//拉伸让图片充满控件　
function Functions_ShowImageAll_Scale(view)

    c2l_Functions_ShowImageAll_Scale(view);

end

--//lua 中直接用字符串来表示颜色就行了//暂时
function HexColor(hex_color)

    return hex_color;

end

--//连接蓝牙
function Blle_ConnectDevice(blle_obj, mac_addr)

    c2l_Blle_ConnectDevice(blle_obj, mac_addr);

end

--//关闭蓝牙连接
function Blle_CloseConnect(blle_obj)

    c2l_Blle_CloseConnect(blle_obj);

end

--//取当前设备的服务个数
function Blle_GetServicesCount(blle_obj)

    return c2l_Blle_GetServicesCount(blle_obj);

end

--//取当前设备的某个服务下的节点 chr 个数
function Blle_GetServiceCharCount(blle_obj, service_index)

    return c2l_Blle_GetServiceCharCount(blle_obj, service_index);

end

--//取 id
function Blle_GetCharUUID(blle_obj, service_index, char_index)

    return c2l_Blle_GetCharUUID(blle_obj, service_index, char_index);

end

--//发送一个字符串
function Blle_SendBuf(blle_obj, service_index, char_index, str_buf)

    return c2l_Blle_SendBuf(blle_obj, service_index, char_index, str_buf);

end

--//是否可监听
function Blle_CharIsNotify(blle_obj, service_index, char_index)

    return c2l_Blle_CharIsNotify(blle_obj, service_index, char_index);

end


--//开始监听
function Blle_StartNotifyChar(blle_obj, service_index, char_index)

    c2l_Blle_StartNotifyChar(blle_obj, service_index, char_index);

end


--//设备视图的背景色
function View_SetBackgroundColor(view, hex_color)

    c2l_View_SetBackgroundColor(view, hex_color);

end

--//视图提前
function View_BringToFront(view)

    c2l_View_BringToFront(view);

end

--//隐藏视图
function View_Hide(view)

    c2l_View_Hide(view);

end

function View_Show(view)

    c2l_View_Show(view);

end

--//旋转视图 //参数为 视图、旋转角度、持续时间（毫秒）//注意，这是顺时针的旋转
--//void View_RotateAni(chandle view, int angle_360, int millisecond)
function View_RotateAni(view, angle_360, millisecond)

    c2l_View_RotateAni(view, angle_360, millisecond);

end

function View_GetParentView(view)

    return c2l_GetParentView(view);

end

--//创建一个视图
function UI_CreateView(view)

    return c2l_CreateView(view);

end


--//创建一个视图 //2022 可以指定类名，如 "image"
function UI_CreateViewClass(view, ui_class)

    return c2l_UI_CreateViewClass(view, ui_class);

end

--//通过 id 设置字体颜色
function UI_Div_SetTxtColor(uijson, id, hexColor)

    c2l_UI_Div_SetTxtColor(uijson, id, hexColor);

end

--//临时
function lg_GetReturnCaption(caption)

    --//2021.8 支持显示回车，本系统中暂时用两个井号表示，即 ##
    
    --caption = [Functions Replace:caption s_old:@"##" s_new:@"\n"];
    caption = StringReplace(caption, "##", "\n");

    --//caption = @"aaa";  //tet

    return caption;
end;

--//---- 语言函数
function lg(key, cn_ref)
    local txt = c2l_UI_LG(key, cn_ref);

    txt = lg_GetReturnCaption(txt);  --//临时,解决英文未换行的问题。以后全部写在框架中。

    return txt;
end

--//关闭软键盘
function UI_Div_CloseKeyboard(uijson)
    c2l_UI_Div_CloseKeyboard(uijson);

end;

--//取 edit 内容
function View_Edit_GetText(edit)
    local txt = c2l_View_Edit_GetText(edit);
    return txt;
end

--//注意，这个是直接操作 view 里面的 edit 内部控件，按道理不应该开放，不过目前暂时可用
function View_Edit_SetText(edit, txt)
    c2l_View_Edit_SetText(edit, txt);
end

--//设置为密码框
function View_Edit_SetPass(edit, as_pass)
    c2l_View_Edit_SetPass(edit, as_pass);
end


--//通过 id 设置标题
function UI_Div_SetTxt(uijson, id, txt)

    c2l_UI_Div_SetTxt(uijson, id, txt);

end

function UI_Div_SetTxt_forObj(uijson, view, txt)

    c2l_UI_Div_SetTxt_forObj(uijson, view, txt);

end

--//设置一个 uijson 页面的语言包 //lng 目前的取值是 en ,默认是 cn
function UI_SetLanguage(uijson, lng)

    c2l_UI_SetLanguage(uijson, lng);

end

function UI_Div_SetTxtColor_forObj(uijson, view, color)

    c2l_UI_Div_SetTxtColor_forObj(uijson, view, color);

end

function UI_Div_SetImage_forObj(uijson, view_obj, image)

    c2l_UI_Div_SetImage_forObj(uijson, view_obj, image);
    
end;

--//设置圆角
function UI_Div_SetRadius_forObj(uijson, view, radius)

    c2l_UI_Div_SetRadius_forObj(uijson, view, radius);

end

--//字符串居左边
function UI_Div_SetTxtAlignLeft_forObj(uijson, view)

    c2l_UI_Div_SetTxtAlignLeft_forObj(uijson, view);

end

--//通过 id 设置图片文件
function UI_Div_SetImage(uijson, id, image)

    c2l_UI_Div_SetImage(uijson, id, image);

end

--//滚动视图拉伸内容高度，以显示所有控件
function ScrollView_ShowAllChild(view)

    c2l_ScrollView_ShowAllChild(view);

end


--//这会在事件发生时调用 lua_functions_event.lua 中的 UI_OnClick 函数，在里面会再转换出 obj ，所以不要直接调用这个函数，而是要调用 ui_json:SetOnClick 来间接使用
function UI_SetOnClick(view, func)

    return c2l_UI_SetOnClick(view, func);
    --//return "0";
end;

function UI_HttpPost(url, func)

    return c2l_UI_HttpPost(url, func);
    --//return "0";
end;

--//这个版本多传了一个参数
function UI_HttpPost_v2(url, obj, func)

    local event = UiEvent:new();
    event.obj = obj;
    event.func = func;

    local r = c2l_UI_HttpPost(url, SaveFunc(event));

    --return r;
end;

--//oss 必须用 get 请求
function UI_HttpGet(url, obj, func)

    UI_HttpPost_ClearParam();
    UI_HttpPost_v2(url, obj, func);

end;

--//2022.09
--//参考 View_GetRect_dp() json 格式的返回值会引起异常，要做保护处理
function RunJson(json_param)
    
    --//return c2l_RunJson(json_param);

    --local s_json = c2l_View_GetRect_dp(view);
    local s_json = c2l_RunJson(json_param);

    --ShowMessage(s_json);

    --ShowMessage(tostring(json));

    --s_json = '{left:100, top:100}'; --//no
    --s_json = '{"left":100, "top":100, "width":150}'; --//ok 即 lua 的 json 一定要把 key 加双引号


    --//太容易出错，保护起来吧
    --函数执行成功返回2个值，分别为true和另外一个函数的返回值,如果没有返回值则返回nil
    --函数执行失败返回2个值，分别为false和出错信息

    local r, j = pcall(function (str_json)

        local obj_json = json.decode(str_json);

        --ShowMessage(str_json);
        --ShowMessage(obj_json.width);

        return obj_json

    end, s_json);

    --ShowMessage("r:" .. tostring(r) .. " result:" .. j.width);

    if r then
        return {response=s_json, result=1, json=j};
    else
        ShowMessage("no json format at RunJson():" .. s_json);
        --ShowMessage(j.width);
        return {response=s_json, result=0, error="no json format at RunJson()"};
    end


    --local j = json.decode(s_json);

    --ShowMessage(j.width);
    --//return "0";
end;

--//--------------------------------------------------------
-- 这里都是基本 RunJson 的 “伪函数”

--//创建一个 tcp 连接  //实现方将返回一个数字，用来表示生成的对象，实际上不是指针，而是在对象列表中的索引
--//不需要传入回调函数指针，统一回调 lua 的回调函数就行。但 lua 里面还是要指定接收函数，以便处理

--//RunJsonObj({ function_name="create_tcp_connect", func=111 });  --//ok //这个更好用，而且不容易出错
function RunJsonObj(ojb_json_param)
    local json_param = json.encode(ojb_json_param);

    --ShowMessage(json_param);

    --//要先执行 c 语言的 json 调用得到结果
    local r = RunJson(json_param);

    return r;
end;


function UI_create_tcp_connect(func)

    --------

    local event = UiEvent:new();
    --event.obj = obj;
    event.func = func;

    --------

    local i_func = SaveFunc(event);  --//这个要传递给 c 语言，回调时会传回来。传回来的按这个得到 event 对象实例
    -- 再通过 event.func 回调。为简单起见，这里的 func 全部实现为不带参数的函数（原来的 onclick 这些都有，太复杂了）。
    -- 以后觉得不够的话再增加有一个 obj 参数的。

    --------
    local param = { function_name="create_tcp_connect", func=i_func };

    local json_param = json.encode(param);

    --ShowMessage(json_param);

    --//要先执行 c 语言的 json 调用得到结果
    local r = RunJson(json_param);

    --//r.result 为 1 时，里面的 json 才是真实的 c 语言函数的返回结果
    if (1 == r.result) then

        ShowMessage("UI_create_tcp_connect()" .. r.response);

        return StrToInt(r.json.result);  --//j.json 中才是 c 语言接口实际返回的内容 //j.json.result 会是它在 java 对象列表中的索引

    end;

    return 0;

end;

--//socket 其实是 UI_create_tcp_connect() 返回的伪值，其实等于 java 对象列表中的索引值。不能小于 1
function UI_tcp_connect(socket, host, port, use_ssl)

    --------
    if (socket < 1) then
        ShowMessage("UI_tcp_connect() socket index error.");
        return;
    end;
    --------
    local param = { function_name="tcp_connect", host=host, port=port, use_ssl=use_ssl };

    local json_param = json.encode(param);
    local j = RunJson(json_param);

    -- 这个是非阻塞函数，不用返回值
    -- if (1 == j.result) then
    --     return j.json.result;  
    -- end;

    -- return 0;

end;

--//--------------------------------------------------------


--//保存到文件
function UI_HttpGetToFile(url, fn, obj, func)

    local event = UiEvent:new();
    event.obj = obj;
    event.func = func;

    local r = c2l_UI_HttpGetToFile(url, fn, SaveFunc(event));

    --return r;
end;

function UI_HttpPost_SetParam(key, value)

    c2l_UI_HttpPost_SetParam(key, value);

    --return r;
end;


function UI_HttpPost_ClearParam()



    c2l_UI_HttpPost_ClearParam();

    --return r;
end;


--//动态设置一个回调函数覆盖此变量即可
G_UI_OnTime = nil;
--//这个和 Blle_OnEvent 一样都是不能改的，由 c 语言直接调用
function UI_OnTime()
    if nil ~= G_UI_OnTime then
        G_UI_OnTime();
    end

end;

function ShowMessage(str)

    c2l_ShowMessage(str)
    --return "0";
end;

--//------------------------------------------------
-- 下面是 app 相关的，其他程序中不会用到

function ShowCarTypeForm(parentView)

    c2l_ShowCarTypeForm(parentView);

end;

function ShowCarMainForm_c(parentView)

    c2l_ShowCarMainForm(parentView);

end;

function ShowCarMacposForm_c(parentView)

    c2l_ShowCarMacposForm(parentView);

end;


--function ShowCarWidthForm(parentView)  -- 这个有重复的了
function ShowCarWidthForm_c(parentView)

    c2l_ShowCarWidthForm(parentView);

end;

function ShowCarSettingForm(parentView)

    c2l_ShowCarSettingForm(parentView);

end;



