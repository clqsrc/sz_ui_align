
--//确实可以这样加载 teal 库
-- local tl = require "tl"
--//新的语言格式。目前：1.成员的 ":" 和 "." 对调; 2.可以用 // 来注释
local tl = require "tl_new_lng"


--//目前的 tl 明显是一行对一行进行的，源码生成后的行数都是一样的。


local s = "ttt";

print("aaa" .. s);

local src_lua = tl.gen('print("aaa" .. s); --//');

print("src_lua:" .. src_lua);
