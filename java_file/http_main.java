

import com.javasrc.src.*;

import static com.javasrc.src.functions.*;

import com.javasrc.http.http;
import com.javasrc.http.*;
import com.javasrc.java_lua.*;
import com.javasrc.java_js.*;
import com.javasrc.map.*;
import java.awt.image.*;
import java.awt.*;

public class http_main {

    public static void main(String[] args) {

        FindStr();

        //-------- 路径映射列表
        http_page.AddToPageList(new http_page_index(), "/index.jsp");
        //http_page.AddToPageList(new http_page_index(), "/index.php");  //模拟 php 其实也是可以的

        http_page.AddToPageList(new http_page_json_t1(), "/json_t1.jsp"); //测试 json 库
        http_page.AddToPageList(new http_page_file_to_one(), "/file_to_one.jsp"); //文件合并

        //--------

        http.CreateServer(8008);

        //--------------------------------------------------------
        map.t3();


        //--------------------------------------------------------

        System.out.println("Hello World");

        System.out.println("aaa");

    }//

}//



