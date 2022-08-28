package main

import (
	//"bytes"
	// "encoding/base64"
	// "encoding/json"
	"fmt"
//	"os"
	// "strings"
	// "time"
	
	
	//"github.com/aliyun/aliyun-oss-go-sdk/oss"
	//"github.com/aliyun/aliyun-tablestore-go-sdk/tablestore"
	//"github.com/aliyun/aliyun-tablestore-go-sdk/sample"
	"github.com/aliyun/aliyun-tablestore-go-sdk/tablestore"
)

//本程序的专有函数

//--------------------------------------------------------
//本程序的 kv_db 实现

//同时保存到 mysql 和 ots ， 这里实现 ots
//参考 oss_table2.go


//--------------------------------------------------------

var App_config_ots_kv_tablename = "kv_file_data";  //数据放在这个 kv 表里
//var App_config_ots_kv_tablename = "kv_file_data2";  //数据放在这个 kv 表里
var App_config_ots_file_tablename = "kv_file_list";   //文件和路径放在这个 kv 表里
//var App_config_ots_file_tablename = "kv_file_list2";   //文件和路径放在这个 kv 表里

//两个 kv 表的结构是一样的，其实完成可以用一张表来存储。不过分成两张表更容易理解，也方便管理。
//实际上，这也完成了一个 hashmap 的文件系统！
//除了排序功能实现未知外，至少用来保存数据肯定是没问题的。

var App_config_ots_kv_field_name_forPathExist = "have_file_path";  //field_value //用目录或者文件是否存在来表示一条记录是否存在


//--------------------------------------------------------

//参考 PutRowSample
//例如 "user_file", "user_name", "u1", "file_name", "1.txt"
//从目前的数据库设计来说 DBUpdateValue(), DBInsertValue() 和 DBSetValue() 对 ots 来说是一样的
func DBSetValue(table string, field_key string, key string, field_value string, value string) bool {
	
	defer PrintError("DBSetValue()");
	
	fmt.Println("DBSetValue()", table, field_key, key);
	
	var r = true;
	
	var client = ots_client();
	
	
	var ots_full_path = makeAliOTS_full_path(table, field_key, key);
	
	
	putRowRequest := new(tablestore.PutRowRequest)
	putRowChange := new(tablestore.PutRowChange)
	putRowChange.TableName = App_config_ots_kv_tablename;   //tableName //这个是固定的，保存所有表的数据
	
	//ali ots 相关操作基本上都是要有一个主键 //除了最后的值以外，其他的都需要
	var putPk = makeAliOTS_PKey_kvtable(table, field_key, key, field_value);

	putRowChange.PrimaryKey = putPk;
	
	putRowChange.AddColumn("value", value);
	// putRowChange.AddColumn("f1", "dddccc")
	// putRowChange.AddColumn("col2", int64(3))
	// putRowChange.AddColumn("col3", []byte("test"))
	
	putRowChange.SetCondition(tablestore.RowExistenceExpectation_IGNORE)
	putRowRequest.PutRowChange = putRowChange
	_, err := client.PutRow(putRowRequest)

	if err != nil {
		fmt.Println("putrow failed with error[table]:", table);
		fmt.Println("putrow failed with error:", err)
	} else {
		fmt.Println("putrow finished")
	}
	
	
	//--------------------------------------------------------
	//file 文件信息表也写入一条数据比较好，相当于文件系统目录不存在时的 ForceDirectories 操作
	
	//kv 表
	var ots_key   = field_value;  //这时候字段名作为 key
	var ots_value = value;        //这时候字段值作为 value
	r = ots_DBSetValue(client, App_config_ots_kv_tablename, ots_full_path, ots_key, ots_value);

	//--
	//file 文件信息表
	var ots_full_path_forFile = ExtractFilePath(ots_full_path);
	var ots_key_forFile   = key;              //这时候的 key 固定为文件名 //这个是关键，必做要保证它加 ots_full_path 能是唯一的文件名，
	//又能在搜索中按路径列表出来
	var ots_value_forFile = "file_name";      //这时候字段值作为 key //或者用 file_name dir_name 来区分它是文件还是路径
	r = ots_DBSetValue(client, App_config_ots_file_tablename, ots_full_path_forFile, ots_key_forFile, ots_value_forFile);

	
	//return true;
	return r;
}//


//ots 的原始操作
func ots_DBSetValue(client *tablestore.TableStoreClient, ots_table string, ots_full_path string, ots_key string, ots_value string) bool {
	
	defer PrintError("ots_DBSetValue()");
	
	//fmt.Println("ots_DBSetValue()", table, field_key, key);
	
	//var client = ots_client();
	
	
	putRowRequest := new(tablestore.PutRowRequest)
	putRowChange := new(tablestore.PutRowChange)
	putRowChange.TableName = ots_table; //App_config_ots_kv_tablename;  //tableName 
	
	//ali ots 相关操作基本上都是要有一个主键
	putPk := new(tablestore.PrimaryKey)
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	//putPk.AddPrimaryKeyColumn("pk2", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	
	putPk.AddPrimaryKeyColumn("full_path", ots_full_path);  //相当于是文件
	putPk.AddPrimaryKeyColumn("key", ots_key);    //相当于是文件中的一行

	putRowChange.PrimaryKey = putPk;
	
	putRowChange.AddColumn("value", ots_value);
	// putRowChange.AddColumn("f1", "dddccc")
	// putRowChange.AddColumn("col2", int64(3))
	// putRowChange.AddColumn("col3", []byte("test"))
	
	putRowChange.SetCondition(tablestore.RowExistenceExpectation_IGNORE)
	putRowRequest.PutRowChange = putRowChange
	_, err := client.PutRow(putRowRequest)

	if err != nil {
		fmt.Println("putrow failed with error[table]:", ots_table);
		fmt.Println("putrow failed with error:", err)
	} else {
		fmt.Println("putrow finished")
	}
	

	
	return true;
}//


//ali ots 相关操作基本上都是要有一个主键
//kv 表的主键

//key 还是要包括的在文件路径中的，例如现在 key 是用户名 (user_name) 的情况
func makeAliOTS_full_path(table string, field_key string, key string) (string) {
	
	//putPk := new(tablestore.PrimaryKey)
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	//putPk.AddPrimaryKeyColumn("pk2", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	//主键的类型和名称一定要对得上，其他的数据字段可以先不存在 //ots 中的主键和表名一旦建立是不能删除的
	
	var full_path = table + "/" + field_key + "/" + key;  
	
	return full_path;
	
}//


func makeAliOTS_PKey_kvtable(table string, field_key string, key string, field_value string) (*tablestore.PrimaryKey) {
	
	putPk := new(tablestore.PrimaryKey)
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	//putPk.AddPrimaryKeyColumn("pk2", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	//主键的类型和名称一定要对得上，其他的数据字段可以先不存在 //ots 中的主键和表名一旦建立是不能删除的
	
	var full_path = makeAliOTS_full_path(table, field_key, key);  //table + "/" + field_key + "/" + key;  
	//例如 "user_file/user_name/u1" 文件夹
	//如果是物理文件夹则有可能是 "mail_data/user_name/u1/user_file.txt"
	
	putPk.AddPrimaryKeyColumn("full_path", full_path);  //相当于是文件
	putPk.AddPrimaryKeyColumn("key", field_value);    //相当于是文件中的一行
	
	return putPk;
	
}//

//文件表的主键
func makeAliOTS_PKey_filetable(table string, field_key string, key string) (*tablestore.PrimaryKey) {
	
	putPk := new(tablestore.PrimaryKey)
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	//putPk.AddPrimaryKeyColumn("pk2", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	//主键的类型和名称一定要对得上，其他的数据字段可以先不存在 //ots 中的主键和表名一旦建立是不能删除的
	
	var full_path = makeAliOTS_full_path(table, field_key, key);  //table + "/" + field_key + "/" + key;  
	//例如 "user_file/user_name/u1" 文件夹
	//如果是物理文件夹则有可能是 "mail_data/user_name/u1/user_file.txt"
	
	putPk.AddPrimaryKeyColumn("full_path", full_path);  //相当于是文件
	//putPk.AddPrimaryKeyColumn("key", field_value);    //相当于是文件中的一行
	
	return putPk;
	
}//


//是否存在
//public Boolean DBHave(String table, String field, String key)
//  GetRowSample()
func DBHaveLine(table string, field_key string, key string, field_value string, value string) bool {
	
		
	defer PrintError("DBGetLine()");
	
	fmt.Println("DBGetLine()", table, field_key, key);
	

	return true;
	
}//


//和单纯是是否存在一行数据还是不同的，例如查找一个用户是否存在时
//用虚拟的目录是否存在来表示一个 key 的值是否存在
//public Boolean DBHave(String table, String field, String key)
func DBHave(table string, field_key string, key string) bool {
	
	defer PrintError("DBHave()");
	
	fmt.Println("DBHave()", table, field_key, key);
	
	//field_value string, value string

	//--------
	//var lines = make([]map[string]string, 0);
	//var lines = make([]map[string]interface{}, 0)
	var line = make(map[string]string);
	//--------
	
	var client = ots_client();
	
	getRowRequest := new(tablestore.GetRowRequest)
	criteria := new(tablestore.SingleRowQueryCriteria)
	
	// putPk := new(tablestore.PrimaryKey)
	// putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	// putPk.AddPrimaryKeyColumn("pk2", int64(2))
	// putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))
	
	//----
	//ali ots 相关操作基本上都是要有一个主键 //除了最后的值以外，其他的都需要
	//主键的类型和名称一定要对得上，其他的数据字段可以先不存在 //ots 中的主键和表名一旦建立是不能删除的
	
	//ali ots 相关操作基本上都是要有一个主键 //除了最后的值以外，其他的都需要
	//用目录或者文件是否存在来表示一条记录是否存在
	//var putPk = makeAliOTS_PKey_kvtable(table, field_key, key, field_value);
	var putPk = makeAliOTS_PKey_filetable(table, field_key, key);
	
	//----

	criteria.PrimaryKey = putPk;
	getRowRequest.SingleRowQueryCriteria = criteria
	//getRowRequest.SingleRowQueryCriteria.TableName = App_config_ots_kv_tablename;   //tableName //这个是固定的，保存所有表的数据
	getRowRequest.SingleRowQueryCriteria.TableName = App_config_ots_file_tablename;   //tableName //这个是固定的，保存所有表的数据
	getRowRequest.SingleRowQueryCriteria.MaxVersion = 1
	getResp, err := client.GetRow(getRowRequest)

	colmap := getResp.GetColumnMap();

	fmt.Println("length is ", len(colmap.Columns));
	if err != nil {
		
		fmt.Println("getrow failed with error:", err);
		
		return false;
		
	} else {
		fmt.Println("get row col0 result is ", getResp.Columns[0].ColumnName, getResp.Columns[0].Value)
	}
	
	//--------
	//有值的话就放到 map 当中
	for i:=0; i<len(colmap.Columns); i++ {
		
		var k =getResp.Columns[0].ColumnName;

		var v = ToString(getResp.Columns[0].Value);  //getResp.Columns[0].Value;
		
		line[k] = v;
		
		
	}//for
	
	//--------
	
	fmt.Println("DBHave() line:", line);

	

	return true;
	
}//

	
//取一行数据
//GetRowSample
//不过，这和我们文件 kv 表的思想是不一样的
func ots_DBGetLine(table string, field_key string, key string, field_value string) (map[string]string) {
	
	defer PrintError("DBGetLine()");
	
	fmt.Println("DBGetLine()", table, field_key, key);
	
	//--------
	//var lines = make([]map[string]string, 0);
	//var lines = make([]map[string]interface{}, 0)
	var line = make(map[string]string);
	//--------
	
	var client = ots_client();
	
	getRowRequest := new(tablestore.GetRowRequest)
	criteria := new(tablestore.SingleRowQueryCriteria)
	
	// putPk := new(tablestore.PrimaryKey)
	// putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	// putPk.AddPrimaryKeyColumn("pk2", int64(2))
	// putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))
	
	//ali ots 相关操作基本上都是要有一个主键 //除了最后的值以外，其他的都需要
	var putPk = makeAliOTS_PKey_kvtable(table, field_key, key, field_value);

	criteria.PrimaryKey = putPk;
	getRowRequest.SingleRowQueryCriteria = criteria
	getRowRequest.SingleRowQueryCriteria.TableName = App_config_ots_kv_tablename;   //tableName //这个是固定的，保存所有表的数据
	getRowRequest.SingleRowQueryCriteria.MaxVersion = 1
	getResp, err := client.GetRow(getRowRequest)

	colmap := getResp.GetColumnMap();

	fmt.Println("length is ", len(colmap.Columns));
	if err != nil {
		
		fmt.Println("getrow failed with error:", err);
		
		return nil;
		
	} else {
		fmt.Println("get row col0 result is ", getResp.Columns[0].ColumnName, getResp.Columns[0].Value)
	}
	
	//--------
	//有值的话就放到 map 当中
	for i:=0; i<len(colmap.Columns); i++ {
		
		var k =getResp.Columns[0].ColumnName;

		var v = ToString(getResp.Columns[0].Value);  //getResp.Columns[0].Value;
		
		line[k] = v;
		
		
	}//for
	
	//--------


	return line;
	
}//

//取一个字段值
//GetRowSample
//不过，这和我们文件 kv 表的思想是不一样的
//public String DBGetValue(String table, String field, String key)
func DBGetValue(table string, field_key string, key string, field_value string) (string) {
	
	defer PrintError("DBGetValue()");
	
	fmt.Println("DBGetValue()", table, field_key, key);
	
	//--------
	//var lines = make([]map[string]string, 0);
	//var lines = make([]map[string]interface{}, 0)
	var line = make(map[string]string);
	//--------
	
	var client = ots_client();
	
	getRowRequest := new(tablestore.GetRowRequest)
	criteria := new(tablestore.SingleRowQueryCriteria)
	
	// putPk := new(tablestore.PrimaryKey)
	// putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	// putPk.AddPrimaryKeyColumn("pk2", int64(2))
	// putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))
	
	//ali ots 相关操作基本上都是要有一个主键 //除了最后的值以外，其他的都需要
	var putPk = makeAliOTS_PKey_kvtable(table, field_key, key, field_value);

	criteria.PrimaryKey = putPk;
	getRowRequest.SingleRowQueryCriteria = criteria
	getRowRequest.SingleRowQueryCriteria.TableName = App_config_ots_kv_tablename;   //tableName //这个是固定的，保存所有表的数据
	getRowRequest.SingleRowQueryCriteria.MaxVersion = 1
	getResp, err := client.GetRow(getRowRequest)

	colmap := getResp.GetColumnMap();

	fmt.Println("length is ", len(colmap.Columns));
	if err != nil {
		
		fmt.Println("getrow failed with error:", err);
		
		//return nil;
		return "";
		
	} else {
		fmt.Println("get row col0 result is ", getResp.Columns[0].ColumnName, getResp.Columns[0].Value)
	}
	
	//--------
	//有值的话就放到 map 当中
	for i:=0; i<len(colmap.Columns); i++ {
		
		var k =getResp.Columns[0].ColumnName;

		var v = ToString(getResp.Columns[0].Value);  //getResp.Columns[0].Value;
		
		line[k] = v;
		
		
	}//for
	
	//--------


	//return line;
	return line["value"];
	
}//


//ots 支持这样的语句 select * from kv_file_data2 where full_path in (select full_path from kv_file_data2)
//concat() 才能连接数据串，用 + 加号的话会当做数字进行计算
//select concat(full_path, "/", `key`) from kv_file_list2  它的 key 是特殊关键字
//所以查询多人数据的语句是这个 select * from kv_file_data2 where full_path in (select concat(full_path, "/", `key`) from kv_file_list2)

//取一行数据
//我们文件 kv 表的思想中，这相当于读取整个文件的内容 
//目前来说可以直接用 sql 操作, 本质上可以用允许多 key 的 kv 结构来实现,不过 ali ots 目前没有这种表
func DBGetLine(table string, field_key string, key string) (map[string]string) {
	
	defer PrintError("DBGetLine()");
	
	fmt.Println("DBGetLine()", table, field_key, key);
	
	//--------
	//var lines = make([]map[string]string, 0);
	//var lines = make([]map[string]interface{}, 0)
	var line = make(map[string]string);
	//--------
	
	var client = ots_client();
	
	var full_path = makeAliOTS_full_path(table, field_key, key);
	
	var sql = "select * from " + App_config_ots_kv_tablename + " where full_path ='" + full_path + "' ";

	var ots_lines, err = ots_SQL_Query(client, sql);
	
	if (err != nil) {
		
		fmt.Println("DBGetLine() err: ", err);
		return line;
	}
	
	//--------
	//有值的话就放到 map 当中
	for i:=0; i<len(ots_lines); i++ {
		
		var ots_line = ots_lines[i];
		
		var k = ots_line["key"];

		var v = ots_line["value"];
		
		line[k] = v;
		
		
	}//for
	
	//--------
	fmt.Println("DBGetLine() line: ", line);

	return line;
	
}//



//实测，目前来说 //这个只能得到预定义字段，在添加数据时增加的字段是不行的
//func SQLSelectSample()
//golang 的 map 和其他容器应该都是引用，所以是不用上指针的
//func ots_SQL_Query(client *tablestore.TableStoreClient, sql string) (* []map[string]string, error) 
func ots_SQL_Query(client *tablestore.TableStoreClient, sql string) ([]map[string]string, error) {
	
	println("BEGIN ots_SQL_Query()")
	request := new(tablestore.SQLQueryRequest)
	
	//request.Query = "select * from test_http_query;"
	//request.Query = "select * from " + table_name + ";"
	request.Query = sql + ";"
	
	response, err := client.SQLQuery(request)
	if err != nil {
		println("[Info] ots_SQL_Query(): select failed with error: ", err.Error())
		return nil, err;
	}
	
	//var lines = new ([]map[string]string);
	var lines = make([]map[string]string, 0);

	resultSet := response.ResultSet
	columns := resultSet.Columns();   //这个只能得到预定义字段，在添加数据时增加的字段是不行的
	for resultSet.HasNext() {
		row := resultSet.Next();
		
		var line = make (map[string]string);
		
		for i := 0; i < len(columns); i++ {
			name := columns[i].Name
			println("columnName: ", name)
			isnull, err := row.IsNull(i)
			if err != nil {
				println("[INFO:] get column error, name: ", name, ", error: ", err.Error())
				continue
			}
			if isnull {
				println("[INFO]: column is SQL NULL, name: ", name)
				continue
			}
			
			
			var s,_ = row.GetString(i);
			
			fmt.Println(s);
			
			line[name] = s;
			
			// switch columns[i].Type {
			// case tablestore.ColumnType_STRING:
			// 	println(row.GetString(i))
			// 	println(row.GetStringByName(name))
			// case tablestore.ColumnType_INTEGER:
			// 	println(row.GetInt64(i))
			// 	println(row.GetInt64ByName(name))
			// case tablestore.ColumnType_DOUBLE:
			// 	println(row.GetFloat64(i))
			// 	println(row.GetFloat64ByName(name))
			// case tablestore.ColumnType_BINARY:
			// 	println(row.GetBytes(i))
			// 	println(row.GetBytesByName(name))
			// case tablestore.ColumnType_BOOLEAN:
			// 	println(row.GetBool(i))
			// 	println(row.GetBoolByName(name))
			// }
			
		}
		
		//(*lines) = append((*lines), line);
		lines = append(lines, line);
	}
	println("END SQLSelectSample")
	
	return lines, nil;
}//



//用做长连接可能是不行的
func ots_client() (*tablestore.TableStoreClient) {
	
	
	//可以看到，和 oss 的服务器并不通用
	//endpoint := App_config_oss_server_hz;  //os.Getenv("OTS_TEST_ENDPOINT")
	var endpoint = App_config_ots_server_hz;
	
	instanceName := App_config_oss_table_instanceName_hz;  //os.Getenv("OTS_TEST_INSTANCENAME")
	accessKeyId := App_config_oss_key;  //os.Getenv("OTS_TEST_KEYID")
	accessKeySecret := App_config_oss_keysecret;  //os.Getenv("OTS_TEST_SECRET")
	client := tablestore.NewClient(endpoint, instanceName, accessKeyId, accessKeySecret);
	
	return client;
	
}//


