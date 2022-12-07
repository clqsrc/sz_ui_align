package com.javasrc.http;

//https://blog.csdn.net/merenguesl/article/details/123521339


import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Iterator;
import java.util.Set;

public class EchoServer {
    private Selector selector=null;
    private ServerSocketChannel serverSocketChannel=null;
    private final int port=4444;

    public EchoServer() throws IOException {
        //创建一个selector对象
        selector=Selector.open();
        //创建一个serverSocketChannel对象
        serverSocketChannel=ServerSocketChannel.open();
        //使得在同一个主机上关闭了服务器程序，紧接着再启动该服务器程序时，可以顺利绑定到相同的端口
        serverSocketChannel.socket().setReuseAddress(true);
        //设置为非阻塞模式
        serverSocketChannel.configureBlocking(false);
        //将服务器进程与一个本地端口绑定
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        System.out.println("服务器启动...");
    }

    public void Service() throws IOException{
        //等待连接
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
        while(selector.select()>0){
            Set readKeys=selector.selectedKeys(); //获得selector的selected-keys 集合
            Iterator it=readKeys.iterator();
            while(it.hasNext()){
                //System.out.println(it);
                SelectionKey key=null;
                //处理selectionKey
                try{
                    key = (SelectionKey) it.next(); //取出selectionKey
                    it.remove(); //把selectionKey从Selector的selected-key集合中删除

                    if(key.isAcceptable()){
                        HandleAcceptable(key);
                    }
                    if(key.isReadable()) {
                        HandleReadable(key);
                    }
                    if(key.isWritable()){
                        HandleWriteable(key);
                    }
                }catch(IOException e){
                    e.printStackTrace();
                    try{
                        if(key!=null){
                            //使得这个selectionKey失效
                            key.cancel();
                            key.channel().close(); //关闭于这个selectionKey关联的SocketChannel
                        }
                    }catch (IOException ex){
                        ex.printStackTrace();
                    }
                }

            }
        }
    }

    /**
     *@Description: 处理接收连接就绪事件
     */
    public void HandleAcceptable(SelectionKey key) throws IOException {
        //获得与SelectionKey关联的ServerSocketChannel
        ServerSocketChannel ssc=(ServerSocketChannel) key.channel();
        //获得与客户连接的SocketChannel
        SocketChannel socketChannel= ssc.accept();
        System.out.println("接收到客户连接，来自:"+socketChannel.socket().getInetAddress()+ ":"+socketChannel.socket().getPort());
        //把SockerChannel设置为非阻塞模式
        socketChannel.configureBlocking(false);
        //创建一个用于存放用户发来数据的缓冲区
        ByteBuffer buffer=ByteBuffer.allocate(1024);
        //SocketChannel像selector注册读就绪和写就绪事件
        socketChannel.register(selector,SelectionKey.OP_READ|SelectionKey.OP_WRITE,buffer);//关联了一个buffer附件
    }

    /**
     *@Description: 处理写就绪事件
     */
    public void HandleWriteable(SelectionKey key) throws IOException{
        //获得与SelectionKey关联的附件
        ByteBuffer buffer=(ByteBuffer) key.attachment();
        //获得与SelectionKey关联的SocketChannel
        SocketChannel socketChannel=(SocketChannel) key.channel();
        //切换为读模式，把buffer极限设为位置，把位置置0
        buffer.flip();
        Charset charset= StandardCharsets.UTF_8;
        //将ByteBuffer转为string，好进行字符串操作
        String data=charset.decode(buffer).toString();
        //如果还没有读到一行数据，就返回
        if(data.indexOf("\r\n")==-1) return;
        //截取一行数据
        String outputData=data.substring(0,data.indexOf("\n")+1);
        //将输出的字符串转换为字节，放到outputBuffer中
        ByteBuffer outputBuffer=charset.encode("server:"+outputData);
        //输出outputBuffer中所有的字节
        while(outputBuffer.hasRemaining()){
            socketChannel.write(outputBuffer);
        }

        //删除ByteBuffer中的已处理数据
        ByteBuffer temp=charset.encode(outputData);
        buffer.position(temp.limit());
        buffer.compact();

    }

    /**
     *@Description: 处理读就绪事件
     */
    public void HandleReadable(SelectionKey key) throws IOException{
        //获得与SelectionKey关联的附件
        ByteBuffer buffer=(ByteBuffer) key.attachment();
        //获得与SelectionKey关联的SocketChannel
        SocketChannel socketChannel=(SocketChannel) key.channel();
        //创建一个ByteBuffer，用于存放读到的数据
        ByteBuffer readBuffer=ByteBuffer.allocate(32);
        socketChannel.read(readBuffer);
        ByteBuffer tmp=readBuffer;//用于在控制台显示
        readBuffer.flip();
        //把buffer的极限设为容量
        buffer.limit(buffer.capacity());
        //把readBuffer中的内容拷贝到buffer中
        buffer.put(readBuffer);

       tmp.flip();
       Charset charset=StandardCharsets.UTF_8;
       System.out.println("来自客户端"+socketChannel.socket().getPort()+":"+charset.decode(tmp).toString());
    }

    static public void main(String[] args) throws IOException{
        new EchoServer().Service();
    }

}//

