<%@page import="com.google.gson.Gson"%>
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>


<%@ page import="com.aliyuncs.DefaultAcsClient"%>
<%@ page import="com.aliyuncs.IAcsClient"%>
<%@ page import="com.aliyuncs.dysmsapi.model.v20170525.QuerySendDetailsRequest"%>
<%@ page import="com.aliyuncs.dysmsapi.model.v20170525.QuerySendDetailsResponse"%>
<%@ page import="com.aliyuncs.dysmsapi.model.v20170525.SendSmsRequest"%>
<%@ page import="com.aliyuncs.dysmsapi.model.v20170525.SendSmsResponse"%>
<%@ page import="com.aliyuncs.dysmsapi.transform.v20170525.SendSmsResponseUnmarshaller"%>
<%@ page import="com.aliyuncs.exceptions.ClientException"%>
<%@ page import="com.aliyuncs.http.FormatType"%>
<%@ page import="com.aliyuncs.http.HttpResponse"%>
<%@ page import="com.aliyuncs.profile.DefaultProfile"%>
<%@ page import="com.aliyuncs.profile.IClientProfile"%>

<%@ page import="java.nio.charset.Charset"%>
<%@ page import="java.text.SimpleDateFormat"%>
<%@ page import="java.util.Date"%>
<%@ page import="java.util.UUID"%>

<%@ page import="com.aliyuncs.http.*"%>

<%@ page import="java.io.IOException"%>
<%@ page import="java.net.HttpURLConnection"%>
<%@ page import="java.util.Map"%>
<%

    //另外一个短信发送的版本，使用公司的阿里云账号
    
    //阿里云 api
    //
    //appkey:
    //LTAIDqcN8RXcQx4g
    //ser..:
    //RGQPHuzGCdgB9rKi8z3HXbkhC7vgH5
    //
    //oss 和短信应该都是这个。
    
    //https://help.aliyun.com/document_detail/55284.html
    
    //---------------------------------------------------------------
    //import com.aliyuncs.DefaultAcsClient;
    //import com.aliyuncs.IAcsClient;
    //import com.aliyuncs.dysmsapi.model.v20170525.QuerySendDetailsRequest;
    //import com.aliyuncs.dysmsapi.model.v20170525.QuerySendDetailsResponse;
    //import com.aliyuncs.dysmsapi.model.v20170525.SendSmsRequest;
    //import com.aliyuncs.dysmsapi.model.v20170525.SendSmsResponse;
    //import com.aliyuncs.dysmsapi.transform.v20170525.SendSmsResponseUnmarshaller;
    //import com.aliyuncs.exceptions.ClientException;
    //import com.aliyuncs.http.FormatType;
    //import com.aliyuncs.http.HttpResponse;
    //import com.aliyuncs.profile.DefaultProfile;
    //import com.aliyuncs.profile.IClientProfile;
    //
    //import java.nio.charset.Charset;
    //import java.text.SimpleDateFormat;
    //import java.util.Date;
    //import java.util.UUID;
    
    //---------------------------------------------------------------
    
    class smssender extends Object
    {


        //sms_msg_code 是要发送的验证码
        //sPhone 是要发送的手机号
        public void sendsms(String sms_msg_code, String sPhone) throws Exception
        {
            try{
                
                final String appkey = "keyid";     //你的accessKeyId,参考本文档步骤2
                final String appsec = "keys"; //你的accessKeySecret，参考本文档步骤2
                
                //必填:短信签名-可在短信控制台中找到
                //request.setSignName("云通信");
                final String sms_sign = "xx科技"; //xx科技

                //必填:短信模板-可在短信控制台中找到，发送国际/港澳台消息时，请使用国际/港澳台短信模版
                //request.setTemplateCode("SMS_1000000");    
                final String sms_tem = "SMS_164235335"; //SMS_164235335
                
                //可选:outId为提供给业务方扩展字段,最终在短信回执消息中将此值带回给调用者
                //request.setOutId("yourOutId");
                final String sms_outid = "yourOutId";

                
                
                //final String sPhone = "18978360917";
                
                //---------------------------------------------------------------
                
                //设置超时时间-可自行调整
                System.setProperty("sun.net.client.defaultConnectTimeout", "10000");
                System.setProperty("sun.net.client.defaultReadTimeout", "10000");
                //初始化ascClient需要的几个参数
                final String product = "Dysmsapi";//短信API产品名称（短信产品名固定，无需修改）
                final String domain = "dysmsapi.aliyuncs.com";//短信API产品域名（接口地址固定，无需修改）
                //替换成你的AK
                final String accessKeyId = appkey; //"yourAccessKeyId";//你的accessKeyId,参考本文档步骤2
                final String accessKeySecret = appsec; //"yourAccessKeySecret";//你的accessKeySecret，参考本文档步骤2
                //初始化ascClient,暂时不支持多region（请勿修改）
                IClientProfile profile = DefaultProfile.getProfile("cn-hangzhou", accessKeyId,
                accessKeySecret);
                DefaultProfile.addEndpoint("cn-hangzhou", "cn-hangzhou", product, domain);
                IAcsClient acsClient = new DefaultAcsClient(profile);

                //组装请求对象
                SendSmsRequest request = new SendSmsRequest();
                //使用post提交
                request.setMethod(MethodType.POST);
                //必填:待发送手机号。支持以逗号分隔的形式进行批量调用，批量上限为1000个手机号码,批量调用相对于单条调用及时性稍有延迟,验证码类型的短信推荐使用单条调用的方式；发送国际/港澳台消息时，接收号码格式为国际区号+号码，如“85200000000”
                //request.setPhoneNumbers("1500000000");
                request.setPhoneNumbers(sPhone);
                //必填:短信签名-可在短信控制台中找到
                //request.setSignName("云通信");
                request.setSignName(sms_sign);

                //必填:短信模板-可在短信控制台中找到，发送国际/港澳台消息时，请使用国际/港澳台短信模版
                //request.setTemplateCode("SMS_1000000");
                request.setTemplateCode(sms_tem);

                //可选:模板中的变量替换JSON串,如模板内容为"亲爱的${name},您的验证码为${code}"时,此处的值为
                //友情提示:如果JSON中需要带换行符,请参照标准的JSON协议对换行符的要求,比如短信内容中包含\r\n的情况在JSON中需要表示成\\r\\n,否则会导致JSON在服务端解析失败
                //参考：request.setTemplateParam("{\"变量1\":\"值1\",\"变量2\":\"值2\",\"变量3\":\"值3\"}")
                ////request.setTemplateParam("{\"name\":\"Tom\", \"code\":\"123\"}");
                
                //其实只用到了 code ，这个由服务器生成
                request.setTemplateParam("{\"name\":\"Tom\", \"code\":\"" + sms_msg_code + "\"}");
                
                //可选-上行短信扩展码(扩展码字段控制在7位或以下，无特殊需求用户请忽略此字段)
                //request.setSmsUpExtendCode("90997");

                //可选:outId为提供给业务方扩展字段,最终在短信回执消息中将此值带回给调用者
                //request.setOutId("yourOutId");
                request.setOutId(sms_outid);

                //请求失败这里会抛ClientException异常
                SendSmsResponse sendSmsResponse = acsClient.getAcsResponse(request);
                
                if(sendSmsResponse.getCode() != null && sendSmsResponse.getCode().equals("OK")) 
                {
                    //请求成功
                }
            
            
            } catch (Exception e) {
                e.printStackTrace();
                
                throw new Exception("sms 异常！sendsms()" + e.getMessage()); 

            }finally{
//                try {
//                 //fis.close();
//                 } catch (IOException e) {
//                 // TODO Auto-generated catch block
//                 e.printStackTrace();
//                }

            }//try

        }//func
    
    }//class

    //-------- 测试发送
    
//    Random rand = new Random();
//    Integer code = 100000 + rand.nextInt(800000); //保证是 6 位数
//    
//    //对应数据库的 sms_code [users 表,f4 数据库]
//    
//    smssender t = new smssender();
//    t.sendsms(code.toString(), "13507734567");

%>
