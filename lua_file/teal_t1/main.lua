
--//确实可以这样加载 teal 库
local tl = require "tl"


local s = "ttt";

print("aaa" .. s);

local src_lua = tl.gen('print("aaa" .. s);');

print("src_lua:" .. src_lua);
