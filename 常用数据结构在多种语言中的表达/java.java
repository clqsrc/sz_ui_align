

//map

//读取完整的 http 头信息
Hashtable<String, String> heads = new Hashtable<String, String>();
ArrayList<String> head_lines = new ArrayList<String>();  //头信息的原始数据

ArrayList<String> body_lines = new ArrayList<String>();  //实体中的数据

//----
Hashtable<String, String> get_params = new Hashtable<String, String>();  //get 请求的参数
Hashtable<String, String> params = new Hashtable<String, String>();  //get 和 pos 请求的参数



