
# <center>使用 kv 操作完全代替数据库 sql</center>


实际上大多数数据库操作都可以分解成多个 key/value 的操作形式。

目前已知要扩展的只需要是指定两个 key 的情况，而两个 key 的情况实际上可以转化成对于一个 value 的类型又是一个 kv 的 list 的情况。

这样一思想实际上和 json 只有 kv 和 array 类型就足够了是一样的。

以以下的一张用户数据表为例进行讲解: 假设表名为 "user_file"

```

    +-------+------------+-------------+-----------+-------------------+
    |   ID  |  ID_GUID   |  user_name  | file_name |  file_create_time |
    |       |            |             |           |                   |
    +-------+------------+-------------+-----------+-------------------+
    |   1   |abc-1234-cde|     ccc     |   file1   |                   |
    |       |     f      |             |           |                   |
    +-------+------------+-------------+-----------+-------------------+
    |   2   |abc-5678-cde|     ccc     |   file2   |                   |
    |       |     f      |             |           |                   |
    +-------+------------+-------------+-----------+-------------------+

```


这里假设 ID 字段是自增量的不确定主键，而 ID_GUID 是用户给每一条数据指定的主键（使用随机的 guid 生成器即可）。使用过数据库的用户都知道实际上这个自增量的 ID 用做关键字实际上是不保险的，在数据库压缩或者重新生成时会发生改变，而且也不利于数据的导入导出。

但假如只有一个用户自定义的 ID_GUID 的话在某些数据库中就会导致数据存储后读取的顺序不确定。所以数据库自增量的 ID 我们建议还是保留。

在没有数据库的文件实现中实际上相当于文件的顺序号（以生成时间排序时）。

## 

显然，要读取这个用户的不只一条文件记录，只有传统的 kv 操作是不够的。但实际上要扩展的内容并不多，只需要一个读取列表的函数和一个有两个 key 的 set 函数。

读取函数实现很简单，其实只要把普通的得到 kv 的函数扩展为得到列表（数组）的就可以了。

```

//类似于 ts 的伪码
function DBGetList(key:string):list<kv>;

```

以上使用类似于 typescript/java/c++ 的伪码。类型表示为 ts/delphi/pascal 风格，而数据类型的模板语法为 c++/java/c# 风格。因为目前比较流行而且简单，大家都看得懂 :)

以上类似于一个 hashmap 的实现函数，对于数据库的实现。加上表名和字段名即可:

```

//类似于 ts 的伪码
function DBGetList(table_name:string, key_name:string, key:string):list<kv>;

//调用示例为
DBGetList("user_file", "user_name", "ccc");

```


### 重要的设置数组中某个记录的某个字段值的函数

```

function DBSetListValue(table_name:string, list_key_name:string, list_key_value:string, key_name:string, key_value:string, value_name:string, value:string);

//另外一种风格的伪码
function DBSetListValue(table_name:string, {list_key_name:string, list_key_value:string}, {key_name:string, key_value:string}, {value_name:string, value:string});

//调用的实例为 
DBSetListValue("user_file", "user_name", "ccc", "ID", "2", "file_create_time", "1970-01-01");

//设置一条记录的 "file_create_time" 为 "1970-01-01"


```

这个函数看上去不太一目了然，我自己也得仔细写上注释才不至于以后用错 :)

这个函数用来设置一个数组中的某一个 kv 中的字段的值，显然得有一个参数表明这个字段的名称，比如 "file_name" 或者是  "file_create_time"。

而为了在数组中定位到这条记录就要再有一个关键字，可以是 "ID", "ID_GUID" 中的任意一个。当然了，假如文件名是唯一的，也可以用 "file_name" 不过很多时候文件名是允许重复的。

#### 参数说明

函数参数包括了一张表名及 3 对 kv 形式的字符串。

其中，{list_key_name:string, list_key_value:string} 用于定位要操作的数组（或者说是筛选） 

{key_name:string, key_value:string} 用于定位这个数组中的哪一条记录（可以是 id 其实也可以是行号等等之类的）

{value_name:string, value:string} 用于说明更新哪个字段，更新为什么内容。


#### 使用时机

实际上我们并不推荐使用以上的函数，因为它的性能并不好，除非是在精心设计的索引上。假如系统是以数据库实现的，实际上，给 "ID_GUID" 加上唯一不重复索引后，直接设置对应的 kv 值就可以了，不过这对于用文件实现的后端来说就做不到了。


## SQL 直接操作的问题

从业过程中见过复杂的 SQL 语句和储存过程导致用户的 mssql 服务器隔一两天就要重启一次的情况。引发的原因是开发者在开发时只注重结果，而对 SQL 语言可能引起的性能问题漠不关心（更多时候是并不了解）。

这也不能怪罪这些开发者，即便是更老道的开发者也保证不了他编写的 sql 不会造成数据库卡顿，这样的问题一旦发生是很难查找的。更不用说隐藏着什么时候触发的情况。



