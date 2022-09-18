

-- //lua 语言自身源码写成的通用支持函数

--//------------------------------------------------
--require(package)
self_require("ui_lib/lua_functions_event");


--//------------------------------------------------

function strlen(str)

    --//和 string.len(str) 应该是不一样的
    -- #str 应该是更偏向字节数

    if nil == str then
        return 0;
    end

    return #str;
end;

--//取数组个数  //注意，如果索引是从 0 开始的则不正确，因为 lua 的 [#] 计算符是从 1 开始计算的
function len(array)

    --//和 string.len(str) 应该是不一样的
    -- #str 应该是更偏向字节数

    --//注：ipairs 这种方式的遍历只会从key为1的地方开始，一直以key递增1的顺序来遍历，若找到一个递增不是1的key就结束遍历，无论后面是否仍然是顺序的key。
    --//所以这里应该用 pairs 。 同理，上面的 len() 函数也是从 1 开始统计的，所以一定要注意

    if nil == array then
        return 0;
    end

    return #array;
end;

--//是否有子字符串，可以安全的用在 lstring->str 上 //2021.11 很多时候没有的话很不方便
function FindStr(s, sub_s)

    -- string.find()

    -- 原型：string.find (s, pattern [, init [, plain]])
    -- 解释：函数在字符串s里查找第一个和参数pattern匹配的子串，如果找到了一个匹配的子串，
    -- 就会返回这个子串的起始索引和结束索引，否则就会返回nil。
    -- 另外，参数init作为一个数字，指定了搜索的起始位置，这个数字默认为1可以一个负数，表示从后往前数的字符个数。
    -- 参数plain作为第四个可选参数默认为flase，传入参数true表示关闭模式匹配，所以函数只做简单的查找子串的操作，
    -- 如果子串pattern没有字符为空字符串""将会被认为是魔法字符。 如果模式匹配子串被找到了，
    -- 一个成功被找到的子串将会作为第三个返回值，放在两个索引返回值的后边而返回。

    local i_start = 0;  --//lua 的起始位置是从 1 开始的
    local i_end = 0;
    local s_captured = "";
    i_start, i_end, s_captured = string.find(s, sub_s, 1, true);  --//这里我们不使用模式


    --local r = strstr(s, sub_s);

    -- lua 中 这个返回的整数位置是有可能是 nil 的
    if (i_start ~= nil)and(i_start >= 1) then   --//找到了
        return 1;
    end;

    return 0; --//没找到
end; --//

--//拆分字符串 //来源网络，以后再自己写
function Split_v1(str, delim, maxNb)
    -- Eliminate bad cases...
    if string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos;


    --//string.gfind string.gmatch
    --//string.gfind 是5.0的 5.1改名成string.gmatch, 但string.gfind名依然有效, 只是5.0后文档里找不到说明


    for part, pos in string.gfind(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end
    -- Handle the last field
    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result

end;

--//分割字符串到数组中
function Split(szFullString, szSeparator)
    local nFindStartIndex = 1;
    local nSplitIndex = 1;
    local nSplitArray = {};

    local count = 0; --//死循环保护

    while true do

        -------- --//死循环保护
        count = count + 1;
        if count > 1000 then
            return {};
        end
        --------

       local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)
       if not nFindLastIndex then
        nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))
        break
       end
       nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)
       nFindStartIndex = nFindLastIndex + string.len(szSeparator)
       nSplitIndex = nSplitIndex + 1

    end; --//while

    return nSplitArray;
    
end;


--取整数
function Trunc(num)

    local r_int, _ = math.modf(num);

    return r_int;
end;

--取数字的正负号
-- //类似 js 的 Math.sign()取正负数符号，但在 0 时返回 1
function Math_sign(num)
    -- Math.sign()有三个可能的返回值：//js, lua 的其实都是有可能返回三个值
    -- 1 // 正数
    -- -1 // 负数
    -- 0 // 正0

    if (0 == num) then return 1; end;

    if (num > 0) then
        return 1;
    else
        return -1;
    end

end;


--去掉左右空白 //对长字符串来说效率低，所以少用
function Trim(s)

    local r = "";
    local skip_char = 1;
    for i=1,#s do

        --print(string.sub(s,i,i))
        
        local c = string.sub(s,i,i);

        if (" " == c)
        or ("\r" == c) 
        or ("\n" == c)
        or ("\t" == c)  
        then 
            skip_char = 1; 
        else skip_char = 0; end;

        --//lua 没有 continue 。lua 5.2 以后可使用新功能 goto 
        if (0 == skip_char) then
            r = r .. c;
        end

    end;
    s = r;
    r = "";

    skip_char = 1;
    --倒序再来一次
    for i=#s, 1, -1 do
        
        --print(string.sub(name,i,i))
        local c = string.sub(s,i,i);

        if (" " == c)
        or ("\r" == c) 
        or ("\n" == c)
        or ("\t" == c)  
        then 
            skip_char = 1; 
        else skip_char = 0; end;

        --//lua 没有 continue 。lua 5.2 以后可使用新功能 goto 
        if (0 == skip_char) then
            r = c .. r;
        end

    end

    return r;
end;

--取无路径的文件名 
--//string.byte 是区分不了 utf8 的单个字符串的，要处理 utf8 的单个字符，需要自己处理
function ExtractFileName(s)

    local r = "";

    --默认为第1个返回a的ascii值
    -- local r = string.byte('abcdefg')    --97

    -- --从索引2(b)到索引4(d)也就是分别返回bcd的ascii值
    -- local r1,r2,r3 = string.byte('abcdefg',2,4)    --98,99,100

    -- --返回98所对应的字符
    -- local r = string.char(98)    --a

    -- --返回98,,99,100对应的字符并连在一起返回
    -- local r = string.char(98,99,100)    --abc


    
    local char_path1 = string.byte("/");    --//默认为第1个返回的ascii值
    local char_path2 = string.byte("\\");    --//默认为第1个返回的ascii值

    --//string.byte 是否比 string.sub 快？

    local skip_char = 0;
    --倒序再来一次
    for i=#s, 1, -1 do
        
        --print(string.sub(name,i,i))
        --local c = string.sub(s,i,i);
        local c = string.byte(s, i, i);

        if (char_path1 == c)
        or (char_path2 == c)
        then
            skip_char = 1;

            break;
        end;

        --//lua 没有 continue 。lua 5.2 以后可使用新功能 goto 
        if (0 == skip_char) then
            r = string.char(c) .. r;
        end

    end;

    return r;
end;

--//字符串替换
function StringReplace(s, olds, news)

    local r = string.gsub(s, olds, news);

    return r;
end;

--//结构体转字符串，主要为调试
function Table2String(table)

    local s = "";

    for key, value in pairs(table) do
        s = s .. key .."=" .. value .. ";";        
    end

    return s;
end;