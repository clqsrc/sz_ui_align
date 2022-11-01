//package com.xmppmini;
package hpublic;

import java.util.concurrent.locks.*;

//参考 https://blog.csdn.net/a992036795/article/details/51372272
/*
显示锁———-ReentrankLock和Condition
ReentrankLock 和内置锁synchronized相比，实现了相同的语义，但是更具有更高的灵活性。
(1)获得和释放的灵活性。
(2)轮训锁和定时锁。
(3)公平性。
基本操作:
lock(): 获取锁

tryLock(): 尝试获取锁

tryLock(long timeout,TimeUnit unit): 尝试获取锁，如果到了指定的时间还获取不到，那么超时。

unlock(): 释放锁

newCondition(): 获取锁的 Condition

使用ReentrantLock的一般组合是 lock、tryLock、与unLock成对出现，需要注意的是，千万不要忘记调用unlock来释放锁，负责可能引发死锁的问题。ReentrantLock的常用形式如下所示:

public class ReentrantLockDemo {
    Lock lock = new ReentrantLock();

    public void doSth(){
        lock.lock();
        try {
            //执行某些操作
        }finally {
            lock.unlock();
        }
    }
}
*/


public class thread_lock extends ReentrantLock {
}
