package main;


//Tablestore 是阿里云的一个新数据存储功能，可以和 oss 互动

import (
	//"bytes"
	// "encoding/base64"
	// "encoding/json"
	"fmt"
	"os"
	// "strings"
	// "time"
	
	
	//"github.com/aliyun/aliyun-oss-go-sdk/oss"
	//"github.com/aliyun/aliyun-tablestore-go-sdk/tablestore"
	//"github.com/aliyun/aliyun-tablestore-go-sdk/sample"
	"github.com/aliyun/aliyun-tablestore-go-sdk/tablestore"
)

//--------------------------------------------------------
/*

https://www.aliyun.com/product/news/detail
表格存储 - 表格存储支持数据投递数据湖OSS新功能

2020.10.14

表格存储数据湖投递可以全量备份或实时投递数据到数据湖OSS中存储，以满足更低成本的历史数据存储，以及更大规模的离线和准实时数据分析

--------------------------------------------------------
https://help.aliyun.com/document_detail/27280.html
什么是表格存储

表格存储（Tablestore）面向海量结构化数据提供Serverless表存储服务，同时针对物联网场景深度优化提供一站式的IoTstore解决方案。
适用于海量账单、IM 消息、物联网、车联网、风控、推荐等场景中的结构化数据存储，提供海量数据低成本存储、毫秒级的在线数据查询和检
索以及灵活的数据分析能力。 


--------------------------------------------------------
https://www.aliyun.com/product/news/detail
表格存储 - 表格存储支持通过DTS实时同步MySQL数据

--------------------------------------------------------
https://zhuanlan.zhihu.com/p/62747927
现代IM系统中的消息系统架构
from https://www.zhihu.com/question/68403969/answer/265131017


--------------------------------------------------------

https://developer.aliyun.com/article/739435

基于表格存储Tablestore和OSS实现企业网盘



*/
//--------------------------------------------------------

//--------------------------------------------------------


//使用阿里云官方 sdk 的版本
//github.com\aliyun\aliyun-oss-go-sdk\sample\put_object
func Putobject_new_t2() {
	
	//--------------------------------------------------------
	//应该都是 oss 节点的配置
	//var bucketName = os.Getenv("OSS_TEST_BUCKET");
	
	//应该是 hz 这样的域名, 目前有 3 个节点，hz, hk, 美国
	endpoint := os.Getenv("OTS_TEST_ENDPOINT")
	instanceName := os.Getenv("OTS_TEST_INSTANCENAME")
	accessKeyId := os.Getenv("OTS_TEST_KEYID")
	accessKeySecret := os.Getenv("OTS_TEST_SECRET")
	//instanceName 应该相当于 oss 的 bucketName 吧
	client := tablestore.NewClient(endpoint, instanceName, accessKeyId, accessKeySecret);
	
	fmt.Println(client);
	
	//--------------------------------------------------------
	


	
}//


//参考 D:\gopath1.10.8\src\_src_map\vendor\github.com\aliyun\aliyun-tablestore-go-sdk\sample\SQLOperation.go 

// SQLCreateTableSample 目前Create Table创建的是mapping映射表
//func SQLCreateTableSample(client *tablestore.TableStoreClient) {
func SQLCreateTableSample(client *tablestore.TableStoreClient) {

	println("BEGIN SQLCreateTableSample")

	request := new(tablestore.SQLQueryRequest)
	//request.Query = "create table if not exists test_http_query (a bigint, b double, c mediumtext, d mediumblob, e bool, primary key (`a`));"
	request.Query = "create table aaa2 (a bigint,  primary key (`a`));"
	response, err := client.SQLQuery(request)  //不行，新接口中似乎要换成 CreateTable
	if err != nil {
		println("[Info]: create table failed with error: ", err.Error())
		return
	}

	println("[Info]: create table success, request id: ", response.RequestId)
	println("END SQLCreateTableSample")

}//

//参考 https://developer.aliyun.com/article/738758 吧，比较啰嗦
func SQLCreateTableSample2(client *tablestore.TableStoreClient) {

	println("BEGIN SQLCreateTableSample")

	request := new(tablestore.CreateTableRequest)
	//request.Query = "create table if not exists test_http_query (a bigint, b double, c mediumtext, d mediumblob, e bool, primary key (`a`));"
	request.TableMeta = new(tablestore.TableMeta);
	response, err := client.CreateTable(request)  //不行，新接口中似乎要换成 CreateTable
	if err != nil {
		println("[Info]: create table failed with error: ", err.Error())
		return
	}

	println("[Info]: create table success, request id: ", response.RequestId)
	println("END SQLCreateTableSample")

}//

//https://github.com/aliyun/aliyun-tablestore-go-sdk/blob/master/sample.go
func oss_table_test1() {
	// Replace the endpoint info
	
	//可以看到，和 oss 的服务器并不通用
	//endpoint := App_config_oss_server_hz;  //os.Getenv("OTS_TEST_ENDPOINT")
	var endpoint = App_config_ots_server_hz;
	
	
	instanceName := App_config_oss_table_instanceName_hz;  //os.Getenv("OTS_TEST_INSTANCENAME")
	accessKeyId := App_config_oss_key;  //os.Getenv("OTS_TEST_KEYID")
	accessKeySecret := App_config_oss_keysecret;  //os.Getenv("OTS_TEST_SECRET")
	client := tablestore.NewClient(endpoint, instanceName, accessKeyId, accessKeySecret);
	
	fmt.Println(client);
	
	//应该是创建一个 sql 类型的表
	SQLCreateTableSample(client);
	
	SQLSelectSample(client, "aaa");
	
	PutRowSample(client, "aaa");
	
	//--------------------------------------------------------
	//以上的 sql 类型的，基本上和 mysql 差不多。下面是 kv 的
	
	
	putRow(client, "aaa", "123");

	/*
	sample.UpdateRowWithIncrement(client, "sampletable")
	//return
	// Table operation
	sample.CreateTableSample(client, "sampletable")
	sample.CreateTableKeyAutoIncrementSample(client)
	sample.DeleteTableSample(client)
	sample.ListTableSample(client)
	sample.UpdateTableSample(client, "sampletable")
	sample.DescribeTableSample(client, "sampletable")

	// Single row operation
	sample.PutRowSample(client, "sampletable")
	sample.UpdateRowSample(client, "sampletable")
	sample.GetRowSample(client, "sampletable")
	sample.DeleteRowSample(client, "sampletable")
	sample.PutRowWithKeyAutoIncrementSample(client)

	// Multi row operation
	sample.BatchWriteRowSample(client, "sampletable")
	sample.BatchGetRowSample(client, "sampletable")
	sample.GetRangeSample(client, "sampletable")

	// Stream sample
	// sample.GetStreamRecordSample(client, "streamtable1")

	// computeSplitpoint
	sample.ComputeSplitPointsBySize(client, "sampletable")

	// transaction
	sample.PutRowWithTxnSample(client, "transtable1")

	// globalindex
	sample.CreateTableWithGlobalIndexSample(client, "globalindex1")

	//SearchIndex
	sample.CreateSearchIndexWithVirtualField(client, "virtual_sample_table", "virtual_sample_index")

	//SearchIndex: agg & group by
	sample.CreateSearchIndexForAggregationAndGroupBy(client, "agg_sample_table", "agg_sample_index")
	sample.WriteDataForAggregationAndGroupBy(client, "agg_sample_table")
	sample.AggregationSample(client, "agg_sample_table", "agg_sample_index")
	sample.GroupBySample(client, "agg_sample_table", "agg_sample_index")

	sample.ParallelScanSingleConcurrency(client, "scan_sample_table", "scan_sample_index")
	sample.ParallelScanMultiConcurrency(client, "scan_sample_table", "scan_sample_index")

	// update searchIndex schema
	sample.UpdateSearchIndexSchema(client, "go_sdk_test_table", "go_sdk_test_index", "go_sdk_test_index_reindex")

	// SQL sample
	sample.SQLQuerySample(client)

	// Server side encryption sample
	sample.ServerSideEncryptionSample(client)
	*/
	
}//


//实测，目前来说 //这个只能得到预定义字段，在添加数据时增加的字段是不行的
func SQLSelectSample(client *tablestore.TableStoreClient, table_name string) {
	println("BEGIN SQLSelectSample")
	request := new(tablestore.SQLQueryRequest)
	
	//request.Query = "select * from test_http_query;"
	request.Query = "select * from " + table_name + ";"
	
	response, err := client.SQLQuery(request)
	if err != nil {
		println("[Info]: select failed with error: ", err.Error())
		return
	}

	resultSet := response.ResultSet
	columns := resultSet.Columns();   //这个只能得到预定义字段，在添加数据时增加的字段是不行的
	for resultSet.HasNext() {
		row := resultSet.Next()
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
	}
	println("END SQLSelectSample")
}//


//--------------------------------------------------------
func putRow(client *tablestore.TableStoreClient, tableName string, pkValue string) {
	
	var PRIMARY_KEY_NAME       = "id";
	
	putRowRequest := new(tablestore.PutRowRequest)
	putRowChange := new(tablestore.PutRowChange)
	putRowChange.TableName = tableName
	putPk := new(tablestore.PrimaryKey)
	putPk.AddPrimaryKeyColumn(PRIMARY_KEY_NAME, pkValue)

	putRowChange.PrimaryKey = putPk
	putRowChange.AddColumn("price", int64(5120))
	putRowChange.SetCondition(tablestore.RowExistenceExpectation_IGNORE)
	putRowRequest.PutRowChange = putRowChange
	_, err := client.PutRow(putRowRequest)
	if err != nil {
		fmt.Println("PutRow failed", tableName, err.Error())
	}
}

func getRow(client *tablestore.TableStoreClient, tableName string, pkValue string) {
	
	var PRIMARY_KEY_NAME       = "id";
	
	getRowRequest := new(tablestore.GetRowRequest)
	criteria := new(tablestore.SingleRowQueryCriteria)
	putPk := new(tablestore.PrimaryKey)
	putPk.AddPrimaryKeyColumn(PRIMARY_KEY_NAME, pkValue)

	criteria.PrimaryKey = putPk
	getRowRequest.SingleRowQueryCriteria = criteria
	getRowRequest.SingleRowQueryCriteria.TableName = tableName
	getRowRequest.SingleRowQueryCriteria.MaxVersion = 1
	getResp, err := client.GetRow(getRowRequest)

	if err != nil {
		fmt.Println("GetRow failed", tableName, err)
	} else {
		colmap := getResp.GetColumnMap()
		fmt.Println(tableName, "length is ", len(colmap.Columns))
		fmt.Println("get row col0 result is ", getResp.Columns[0].ColumnName, getResp.Columns[0].Value)
	}
}


//--------------------------------------------------------
//来自 aliyun-tablestore-go-sdk\sample\SingleRowOperation.go
//有主键的，其他可以动态，这个可以很容易用本地 json 文件模拟 //所以可以很容易代替本地的 json 文件
func PutRowSample(client *tablestore.TableStoreClient, tableName string) {
	fmt.Println()
	putRowRequest := new(tablestore.PutRowRequest)
	putRowChange := new(tablestore.PutRowChange)
	putRowChange.TableName = tableName
	putPk := new(tablestore.PrimaryKey)
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	//putPk.AddPrimaryKeyColumn("pk2", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	//主键的类型和名称一定要对得上，其他的数据字段可以先不存在
	//putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	putPk.AddPrimaryKeyColumn("id", int64(2))
	//putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	putRowChange.PrimaryKey = putPk
	putRowChange.AddColumn("col1", "col1data1")
	putRowChange.AddColumn("f1", "dddccc")
	putRowChange.AddColumn("col2", int64(3))
	putRowChange.AddColumn("col3", []byte("test"))
	putRowChange.SetCondition(tablestore.RowExistenceExpectation_IGNORE)
	putRowRequest.PutRowChange = putRowChange
	_, err := client.PutRow(putRowRequest)

	if err != nil {
		fmt.Println("putrow failed with error:", err)
	} else {
		fmt.Println("putrow finished")
	}
}//


func GetRowSample(client *tablestore.TableStoreClient, tableName string) {
	fmt.Println("begin to get row")
	getRowRequest := new(tablestore.GetRowRequest)
	criteria := new(tablestore.SingleRowQueryCriteria)
	putPk := new(tablestore.PrimaryKey)
	putPk.AddPrimaryKeyColumn("pk1", "pk1value1")
	putPk.AddPrimaryKeyColumn("pk2", int64(2))
	putPk.AddPrimaryKeyColumn("pk3", []byte("pk3"))

	criteria.PrimaryKey = putPk
	getRowRequest.SingleRowQueryCriteria = criteria
	getRowRequest.SingleRowQueryCriteria.TableName = tableName
	getRowRequest.SingleRowQueryCriteria.MaxVersion = 1
	getResp, err := client.GetRow(getRowRequest)

	colmap := getResp.GetColumnMap()

	fmt.Println("length is ", len(colmap.Columns))
	if err != nil {
		fmt.Println("getrow failed with error:", err)
	} else {
		fmt.Println("get row col0 result is ", getResp.Columns[0].ColumnName, getResp.Columns[0].Value)
	}
}//



