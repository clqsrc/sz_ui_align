package main;


//Tablestore 是阿里云的一个新数据存储功能，可以和 oss 互动

import (
	"bytes"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"os"
	"strings"
	"time"
	"sync/atomic"  //应该是整数加减的原子操作而已
	
	
	"github.com/aliyun/aliyun-oss-go-sdk/oss"
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
/*
//oss 的官方文档 含各个 sdk 的说明，特别是浏览器的，可以看看跨域的详细解决方案
https://help.aliyun.com/document_detail/52834.html

golang 的，很全
https://help.aliyun.com/document_detail/32144.htm#concept-32144-zh
(不过没有上传进度条的，进度条的在 vendor\github.com\aliyun\aliyun-oss-go-sdk\oss\progress_test.go
参考这个 java 版本的 https://help.aliyun.com/document_detail/84796.htm)

浏览器的 OSS Browser.js SDK 很值得一看，另外还有一个难点就是 sts 的生成
https://help.aliyun.com/document_detail/64041.htm#concept-64041-zh

*/
//--------------------------------------------------------


//使用阿里云官方 sdk 的版本
//依赖两个文件 config.go comm.go

var (
	// Sample code's env configuration. You need to specify them with the actual configuration if you want to run sample code
	//endpoint   = os.Getenv("OSS_TEST_ENDPOINT") //App_config_oss_server
	//accessID   = os.Getenv("OSS_TEST_ACCESS_KEY_ID")
	//accessKey  = os.Getenv("OSS_TEST_ACCESS_KEY_SECRET")
	//bucketName = os.Getenv("OSS_TEST_BUCKET")
	kmsID      = os.Getenv("OSS_TEST_KMS_ID")
	accountID  = os.Getenv("OSS_TEST_ACCOUNT_ID")
	stsARN     = os.Getenv("OSS_TEST_STS_ARN")

	// Credential
	credentialAccessID  = os.Getenv("OSS_CREDENTIAL_KEY_ID")
	credentialAccessKey = os.Getenv("OSS_CREDENTIAL_KEY_SECRET")
	credentialUID       = os.Getenv("OSS_CREDENTIAL_UID")

	// The cname endpoint
	endpoint4Cname = os.Getenv("OSS_TEST_CNAME_ENDPOINT")
)

const (

	// The object name in the sample code
	objectKey       string = "my-object"
	appendObjectKey string = "my-object-append"

	// The local files to run sample code.
	localFile     	   string = "sample/BingWallpaper-2015-11-07.jpg"
	localCsvFile 	   string = "sample/sample_data.csv"
	localJSONFile 	   string = "sample/sample_json.json"
	localJSONLinesFile string = "sample/sample_json_lines.json"
	htmlLocalFile 	   string = "sample/The Go Programming Language.html"
)


//--------------------------------------------------------
var (
	pastDate   = time.Date(2009, time.November, 10, 23, 0, 0, 0, time.UTC)
	futureDate = time.Date(2049, time.January, 10, 23, 0, 0, 0, time.UTC)
)

// HandleError is the error handling method in the sample code
func HandleError(err error) {
	fmt.Println("occurred error:", err)
	//clq//os.Exit(-1)
}

// GetTestBucket creates the test bucket
func GetTestBucket(bucketName string) (*oss.Bucket, error) {
	// New client
	//client, err := oss.New(endpoint, accessID, accessKey)
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret)
	if err != nil {
		return nil, err
	}

	// Create bucket
	err = client.CreateBucket(bucketName)
	if err != nil {
		return nil, err
	}

	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		return nil, err
	}

	return bucket, nil
}

// DeleteTestBucketAndLiveChannel 删除sample的channelname和bucket，该函数为了简化sample，让sample代码更明了
func DeleteTestBucketAndLiveChannel(bucketName string) error {
	// New Client
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret)
	if err != nil {
		return err
	}

	// Get Bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		return err
	}

	marker := ""
	for {
		result, err := bucket.ListLiveChannel(oss.Marker(marker))
		if err != nil {
			HandleError(err)
		}

		for _, channel := range result.LiveChannel {
			err := bucket.DeleteLiveChannel(channel.Name)
			if err != nil {
				HandleError(err)
			}
		}

		if result.IsTruncated {
			marker = result.NextMarker
		} else {
			break
		}
	}

	// Delete Bucket
	err = client.DeleteBucket(bucketName)
	if err != nil {
		return err
	}

	return nil
}

// DeleteTestBucketAndObject deletes the test bucket and its objects
func DeleteTestBucketAndObject(bucketName string) error {
	// New client
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret)
	if err != nil {
		return err
	}

	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		return err
	}

	// Delete part
	keyMarker := oss.KeyMarker("")
	uploadIDMarker := oss.UploadIDMarker("")
	for {
		lmur, err := bucket.ListMultipartUploads(keyMarker, uploadIDMarker)
		if err != nil {
			return err
		}
		for _, upload := range lmur.Uploads {
			var imur = oss.InitiateMultipartUploadResult{Bucket: bucket.BucketName,
				Key: upload.Key, UploadID: upload.UploadID}
			err = bucket.AbortMultipartUpload(imur)
			if err != nil {
				return err
			}
		}
		keyMarker = oss.KeyMarker(lmur.NextKeyMarker)
		uploadIDMarker = oss.UploadIDMarker(lmur.NextUploadIDMarker)
		if !lmur.IsTruncated {
			break
		}
	}

	// Delete objects
	marker := oss.Marker("")
	for {
		lor, err := bucket.ListObjects(marker)
		if err != nil {
			return err
		}
		for _, object := range lor.Objects {
			err = bucket.DeleteObject(object.Key)
			if err != nil {
				return err
			}
		}
		marker = oss.Marker(lor.NextMarker)
		if !lor.IsTruncated {
			break
		}
	}

	// Delete bucket
	err = client.DeleteBucket(bucketName)
	if err != nil {
		return err
	}

	return nil
}

// Object defines pair of key and value
type Object struct {
	Key   string
	Value string
}

// CreateObjects creates some objects
func CreateObjects(bucket *oss.Bucket, objects []Object) error {
	for _, object := range objects {
		err := bucket.PutObject(object.Key, strings.NewReader(object.Value))
		if err != nil {
			return err
		}
	}
	return nil
}

// DeleteObjects deletes some objects.
func DeleteObjects(bucket *oss.Bucket, objects []Object) error {
	for _, object := range objects {
		err := bucket.DeleteObject(object.Key)
		if err != nil {
			return err
		}
	}
	return nil
}




//--------------------------------------------------------


//使用阿里云官方 sdk 的版本
//github.com\aliyun\aliyun-oss-go-sdk\sample\put_object
func Putobject_new_t1() {
	
	//--------------------------------------------------------
	//应该都是 oss 节点的配置
	var bucketName = App_config_oss_bucket; //os.Getenv("OSS_TEST_BUCKET");
	
	//应该是 hz 这样的域名, 目前有 3 个节点，hz, hk, 美国
	//var endpoint   = os.Getenv("OSS_TEST_ENDPOINT")
	
	
	// New client
	//client, err := oss.New(endpoint, accessID, accessKey);
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret);
	
	if err != nil {
		fmt.Println("Putobject_new()", err);
		return;
	}


	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		fmt.Println("Putobject_new()", err);
		return;
	}
	HandleError(err)
	
	//--------------------------------------------------------
	
	// Create bucket
	// bucket, err := GetTestBucket(bucketName)
	// if err != nil {
	// 	HandleError(err)
	// }

	var val = "花间一壶酒，独酌无相亲。 举杯邀明月，对影成三人。"

	// Case 1: Upload an object from a string
	//err = bucket.PutObject(objectKey, strings.NewReader(val))
	err = bucket.PutObject("objectKey.txt", strings.NewReader(val))
	if err != nil {
		HandleError(err)
	}

	// Case 2: Upload an object whose value is a byte[]
	err = bucket.PutObject(objectKey, bytes.NewReader([]byte(val)))
	if err != nil {
		HandleError(err)
	}

	// Case 3: Upload the local file with file handle, user should open the file at first.
	fd, err := os.Open(localFile)
	if err != nil {
		HandleError(err)
	}
	defer fd.Close()

	err = bucket.PutObject(objectKey, fd)
	if err != nil {
		HandleError(err)
	}

	// Case 4: Upload an object with local file name, user need not open the file.
	err = bucket.PutObjectFromFile(objectKey, localFile)
	if err != nil {
		HandleError(err)
	}

	// Case 5: Upload an object with specified properties, PutObject/PutObjectFromFile/UploadFile also support this feature.
	options := []oss.Option{
		oss.Expires(futureDate),
		oss.ObjectACL(oss.ACLPublicRead),
		oss.Meta("myprop", "mypropval"),
	}
	err = bucket.PutObject(objectKey, strings.NewReader(val), options...)
	if err != nil {
		HandleError(err)
	}

	props, err := bucket.GetObjectDetailedMeta(objectKey)
	if err != nil {
		HandleError(err)
	}
	fmt.Println("Object Meta:", props)

	// Case 6: Upload an object with sever side encrpytion kms and kms id specified
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.ServerSideEncryption("KMS"), oss.ServerSideEncryptionKeyID(kmsID))
	if err != nil {
		HandleError(err)
	}

	// Case 7: Upload an object with callback
	callbackMap := map[string]string{}
	callbackMap["callbackUrl"] = "http://oss-demo.aliyuncs.com:23450"
	callbackMap["callbackHost"] = "oss-cn-hangzhou.aliyuncs.com"
	callbackMap["callbackBody"] = "filename=${object}&size=${size}&mimeType=${mimeType}"
	callbackMap["callbackBodyType"] = "application/x-www-form-urlencoded"

	callbackBuffer := bytes.NewBuffer([]byte{})
	callbackEncoder := json.NewEncoder(callbackBuffer)
	//do not encode '&' to "\u0026"
	callbackEncoder.SetEscapeHTML(false)
	err = callbackEncoder.Encode(callbackMap)
	if err != nil {
		HandleError(err)
	}

	callbackVal := base64.StdEncoding.EncodeToString(callbackBuffer.Bytes())
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.Callback(callbackVal))
	if err != nil {
		HandleError(err)
	}

	// Case 8: Big file's multipart upload. It supports concurrent upload with resumable upload.
	// multipart upload with 100K as part size. By default 1 coroutine is used and no checkpoint is used.
	err = bucket.UploadFile(objectKey, localFile, 100*1024)
	if err != nil {
		HandleError(err)
	}

	// Part size is 100K and 3 coroutines are used
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Routines(3))
	if err != nil {
		HandleError(err)
	}

	// Part size is 100K and 3 coroutines with checkpoint
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Routines(3), oss.Checkpoint(true, ""))
	if err != nil {
		HandleError(err)
	}

	// Specify the local file path for checkpoint files.
	// the 2nd parameter of Checkpoint can specify the file path, when the file path is empty, it will upload the directory.
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Checkpoint(true, localFile+".cp"))
	if err != nil {
		HandleError(err)
	}

	// Case 9: Set the storage classes.OSS provides three storage classes: Standard, Infrequent Access, and Archive.
	// Supported APIs: PutObject, CopyObject, UploadFile, AppendObject...
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.ObjectStorageClass("IA"))
	if err != nil {
		HandleError(err)
	}

	// Upload a local file, and set the object's storage-class to 'Archive'.
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.ObjectStorageClass("Archive"))
	if err != nil {
		HandleError(err)
	}

	// Delete object and bucket
	//err = DeleteTestBucketAndObject(bucketName)
	//if err != nil {
	//	HandleError(err)
	//}

	fmt.Println("PutObjectSample completed")
	
}//


//正规地上传一个文件
func oss_PutFile(local_full_path string, server_path string) {
	
	defer PrintError("oss_PutFile");
	
	//--------------------------------------------------------
	//应该都是 oss 节点的配置
	var bucketName = App_config_oss_bucket; //os.Getenv("OSS_TEST_BUCKET");
	
	//应该是 hz 这样的域名, 目前有 3 个节点，hz, hk, 美国
	//var endpoint   = os.Getenv("OSS_TEST_ENDPOINT")
	
	
	// New client
	//client, err := oss.New(endpoint, accessID, accessKey);
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret);
	
	if err != nil {
		fmt.Println("oss_PutFile()", err);
		return;
	}


	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		fmt.Println("Putobject_new()", err);
		return;
	}
	HandleError(err)
	
	//--------------------------------------------------------
	
	// Create bucket
	// bucket, err := GetTestBucket(bucketName)
	// if err != nil {
	// 	HandleError(err)
	// }

	var val = "花间一壶酒，独酌无相亲。 举杯邀明月，对影成三人。"

	// Case 1: Upload an object from a string
	//err = bucket.PutObject(objectKey, strings.NewReader(val))
	err = bucket.PutObject("objectKey.txt", strings.NewReader(val))
	if err != nil {
		HandleError(err)
	}

	// Case 2: Upload an object whose value is a byte[]
	err = bucket.PutObject(objectKey, bytes.NewReader([]byte(val)))
	if err != nil {
		HandleError(err)
	}

	// Case 3: Upload the local file with file handle, user should open the file at first.
	fd, err := os.Open(localFile)
	if err != nil {
		HandleError(err)
	}
	defer fd.Close()

	err = bucket.PutObject(objectKey, fd)
	if err != nil {
		HandleError(err)
	}

	// Case 4: Upload an object with local file name, user need not open the file.
	err = bucket.PutObjectFromFile(objectKey, localFile)
	if err != nil {
		HandleError(err)
	}

	// Case 5: Upload an object with specified properties, PutObject/PutObjectFromFile/UploadFile also support this feature.
	options := []oss.Option{
		oss.Expires(futureDate),
		oss.ObjectACL(oss.ACLPublicRead),
		oss.Meta("myprop", "mypropval"),
	}
	err = bucket.PutObject(objectKey, strings.NewReader(val), options...)
	if err != nil {
		HandleError(err)
	}

	props, err := bucket.GetObjectDetailedMeta(objectKey)
	if err != nil {
		HandleError(err)
	}
	fmt.Println("Object Meta:", props)

	// Case 6: Upload an object with sever side encrpytion kms and kms id specified
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.ServerSideEncryption("KMS"), oss.ServerSideEncryptionKeyID(kmsID))
	if err != nil {
		HandleError(err)
	}

	// Case 7: Upload an object with callback
	callbackMap := map[string]string{}
	callbackMap["callbackUrl"] = "http://oss-demo.aliyuncs.com:23450"
	callbackMap["callbackHost"] = "oss-cn-hangzhou.aliyuncs.com"
	callbackMap["callbackBody"] = "filename=${object}&size=${size}&mimeType=${mimeType}"
	callbackMap["callbackBodyType"] = "application/x-www-form-urlencoded"

	callbackBuffer := bytes.NewBuffer([]byte{})
	callbackEncoder := json.NewEncoder(callbackBuffer)
	//do not encode '&' to "\u0026"
	callbackEncoder.SetEscapeHTML(false)
	err = callbackEncoder.Encode(callbackMap)
	if err != nil {
		HandleError(err)
	}

	callbackVal := base64.StdEncoding.EncodeToString(callbackBuffer.Bytes())
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.Callback(callbackVal))
	if err != nil {
		HandleError(err)
	}

	// Case 8: Big file's multipart upload. It supports concurrent upload with resumable upload.
	// multipart upload with 100K as part size. By default 1 coroutine is used and no checkpoint is used.
	err = bucket.UploadFile(objectKey, localFile, 100*1024)
	if err != nil {
		HandleError(err)
	}

	// Part size is 100K and 3 coroutines are used
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Routines(3))
	if err != nil {
		HandleError(err)
	}

	// Part size is 100K and 3 coroutines with checkpoint
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Routines(3), oss.Checkpoint(true, ""))
	if err != nil {
		HandleError(err)
	}

	// Specify the local file path for checkpoint files.
	// the 2nd parameter of Checkpoint can specify the file path, when the file path is empty, it will upload the directory.
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.Checkpoint(true, localFile+".cp"))
	if err != nil {
		HandleError(err)
	}

	// Case 9: Set the storage classes.OSS provides three storage classes: Standard, Infrequent Access, and Archive.
	// Supported APIs: PutObject, CopyObject, UploadFile, AppendObject...
	err = bucket.PutObject(objectKey, strings.NewReader(val), oss.ObjectStorageClass("IA"))
	if err != nil {
		HandleError(err)
	}

	// Upload a local file, and set the object's storage-class to 'Archive'.
	err = bucket.UploadFile(objectKey, localFile, 100*1024, oss.ObjectStorageClass("Archive"))
	if err != nil {
		HandleError(err)
	}

	// Delete object and bucket
	//err = DeleteTestBucketAndObject(bucketName)
	//if err != nil {
	//	HandleError(err)
	//}

	fmt.Println("PutObjectSample completed")
	
}//


//--------------------------------------------------------
//有进度条的上传，来自 progress_test.go

// OssProgressListener is the progress listener
type OssProgressListener struct {
	TotalRwBytes int64
}

// ProgressChanged handles progress event
func (listener *OssProgressListener) ProgressChanged(event * oss.ProgressEvent) {
	switch event.EventType {
	case oss.TransferStartedEvent:
		fmt.Printf("Transfer Started, ConsumedBytes: %d, TotalBytes %d.\n",
			event.ConsumedBytes, event.TotalBytes)
	case oss.TransferDataEvent:
		atomic.AddInt64(&listener.TotalRwBytes, event.RwBytes)
		fmt.Printf("Transfer Data, ConsumedBytes: %d, TotalBytes %d, %d%%.\n",
			event.ConsumedBytes, event.TotalBytes, event.ConsumedBytes*100/event.TotalBytes)
	case oss.TransferCompletedEvent:
		fmt.Printf("Transfer Completed, ConsumedBytes: %d, TotalBytes %d.\n",
			event.ConsumedBytes, event.TotalBytes)
	case oss.TransferFailedEvent:
		fmt.Printf("Transfer Failed, ConsumedBytes: %d, TotalBytes %d.\n",
			event.ConsumedBytes, event.TotalBytes)
	default:
	}
}//



// TestPutObject
//func (s *OssProgressSuite) TestPutObject(c *C) 
//func TestPutObject_andOssProgress(client * oss.Client, bucket * oss.Bucket) 
func TestPutObject_andOssProgress() {
	
	
	defer PrintError("TestPutObject_andOssProgress");
	
	//--------------------------------------------------------
	//应该都是 oss 节点的配置
	var bucketName = App_config_oss_bucket; 
	
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret);
	
	if err != nil {
		fmt.Println("oss_PutFile()", err);
		return;
	}


	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		fmt.Println("Putobject_new()", err);
		return;
	}


	//--------------------------------------------------------
	
	//objectName := RandStr(8) + ".jpg"
	objectName := "test_progress_rand.jpg"; //上传后的文件
	//localFile := "../sample/The Go Programming Language.html"
	localFile := "d:/go1.18.3.windows-amd64.zip"

	fileInfo, err := os.Stat(localFile)
	//c.Assert(err, IsNil)
	fmt.Println(fileInfo, err);

	// PutObject
	fd, err := os.Open(localFile)
	//c.Assert(err, IsNil)
	defer fd.Close()
	fmt.Println(err);

	progressListener := OssProgressListener{}
	err = bucket.PutObject(objectName, fd, oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())

	// PutObjectFromFile
	progressListener.TotalRwBytes = 0
	err = bucket.PutObjectFromFile(objectName, localFile, oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())

	// DoPutObject
	fd, err = os.Open(localFile)
	//c.Assert(err, IsNil)
	defer fd.Close()

	request := &oss.PutObjectRequest{
		ObjectKey: objectName,
		Reader:    fd,
	}

	progressListener.TotalRwBytes = 0
	options := []oss.Option{oss.Progress(&progressListener)}
	_, err = bucket.DoPutObject(request, options)
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())

	// PutObject size is 0
	progressListener.TotalRwBytes = 0
	err = bucket.PutObject(objectName, strings.NewReader(""), oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, int64(0))

	fmt.Println("OssProgressSuite.TestPutObject")
}//


//正式版本
func PutObject_andOssProgress(full_path_local string, full_path_server string) {
	
	
	defer PrintError("PutObject_andOssProgress");
	
	//--------------------------------------------------------
	//应该都是 oss 节点的配置
	var bucketName = App_config_oss_bucket; 
	
	client, err := oss.New(App_config_oss_server_hz, App_config_oss_key, App_config_oss_keysecret);
	
	if err != nil {
		fmt.Println("oss_PutFile()", err);
		return;
	}


	// Get bucket
	bucket, err := client.Bucket(bucketName)
	if err != nil {
		fmt.Println("Putobject_new()", err);
		return;
	}


	//--------------------------------------------------------
	
	//objectName := "test_progress_rand2.jpg"; //full_path_server;  //"test_progress_rand.jpg"; //上传后的文件
	//localFile := "d:/apns_test.exe"; //full_path_local;    //"d:/go1.18.3.windows-amd64.zip"  //本地的全路径名
	objectName := full_path_server;  //"test_progress_rand.jpg"; //上传后的文件
	localFile := full_path_local;    //"d:/go1.18.3.windows-amd64.zip"  //本地的全路径名

	/*
	//这个应该是第 1 种用法
	fileInfo, err := os.Stat(localFile)
	//c.Assert(err, IsNil)
	fmt.Println(fileInfo, err);

	// PutObject
	fd, err := os.Open(localFile)
	//c.Assert(err, IsNil)
	defer fd.Close()
	fmt.Println(err);

	progressListener := OssProgressListener{}
	err = bucket.PutObject(objectName, fd, oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())
	*/
	
	progressListener := OssProgressListener{}

	//这个应该是第 2 种用法
	// PutObjectFromFile
	progressListener.TotalRwBytes = 0
	err = bucket.PutObjectFromFile(objectName, localFile, oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())
	
	return;

	//后面的，其实是展示另外一种用法
	/*
	// DoPutObject
	fd, err = os.Open(localFile)
	//c.Assert(err, IsNil)
	defer fd.Close()

	request := &oss.PutObjectRequest{
		ObjectKey: objectName,
		Reader:    fd,
	}

	progressListener.TotalRwBytes = 0
	options := []oss.Option{oss.Progress(&progressListener)}
	_, err = bucket.DoPutObject(request, options)
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, fileInfo.Size())

	// PutObject size is 0
	progressListener.TotalRwBytes = 0
	err = bucket.PutObject(objectName, strings.NewReader(""), oss.Progress(&progressListener))
	//c.Assert(err, IsNil)
	//c.Assert(progressListener.TotalRwBytes, Equals, int64(0))

	//fmt.Println("OssProgressSuite.TestPutObject")
	
	fmt.Println("PutObject_andOssProgress() ok");
	*/
}//



//--------------------------------------------------------



