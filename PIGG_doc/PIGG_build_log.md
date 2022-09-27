# 写作日志

## 2022-7-24
1. task.josn和lanch.json可以跑起来了，可以用vscode远程连接调试
2. 使用cmake进行编译
3. cmake编译中添加了mysql,可以操作数据库了

## 2022-7-25
1. 学习了namespace和share_ptr是怎么使用的


## 2022-7-30
1. 之前发现使用语句，但是日志无法写入
fix:使用阻塞队列插入，但是自己还没有实现阻塞队列的内容，导致一直是空实现，就没法写进去了
2. 遗留的问题：单例模式在vscode中如何调试，因为进去就是一个instant()函数了

## 2022-7-30
1. 实现了阻塞队列PIGG_block_queue.h和线程锁PIGG_lock.h，但是都还没有开始使用，现在都是没有线程不安全的

## 2022-7-31
1. 增加了mysql连接池的构造函数sql_connection_pool.h

## 2022-9-16
1. 修改CMake文件http和sql_connection加入编译，修改语法问题
2. 增加github中的workflow自动化测试

## 2022-9-18
1. 写具体的http的内容，函数体都写好了，完成了http应答的部分

## 2022-9-19
1. 继续写具体的http的内容，http的读取
2. 增加了HTML页面

## 2022-9-20
1. 增加了timer的内容，项目中自己写的一个定时器
2. 主函数运行中的webserver中增加了定时器处理的功能
3. 梳理主函数运行流程，接下来准备要跑起来了

## 2022-9-22
1. 程序跑起来，进入主循环event_loop()没有问题，但是使用postman发送数据，就导致程序Segmentation fault (core dumped)错误
2. 卡在epoll_wait里面，正确的程序可以一直循环，怀疑是没有开线程池的原因，所以一直在等待
3. PIGG_http_conn::read_once()函数没有编写没法使用LT和ET读取数据
4. 复习了epoll的使用流程，复习epoll相关函数

## 2022-9-23
1. 增加了很多八股文
2. 学习定时器的相关操作，了解时间轮的原理

## 2022-9-27
1. 增加了线程池，增加了连接池

## 注意事项
1. 使用vscode远程登录调试的时候，需要用root身份登录，不然无法连接数据库
2. 查找相关的帮助 ```man pthread_cond_broadcast```
3. epoll_event这个结构体返回的类型，在```man epoll_ctl中查看```

# 项目实现顺序
1. 现有一个主函数
main.cpp
2. 主函数写好以后得现有基本的配置
PIGG_config.cpp/.h
3. 各个阶段的初始化都写在这里，虽然现在还没有初始化函数
PIGG_webserver.cpp/.h
4. 先得有日志系统，日志的初始化，日志的输出，先写出来，现在只要能打印就行了（阻塞队列+线程锁后面加）
PIGG_log.cpp/.h
5. *[支线]*零基础做的项目，对有些函数的实现不清楚，所以加了一个PIGG_test.cpp来测试一些小模块的使用
PIGG_test.cpp/.h
6. 补充基础模块，日志的阻塞队列+线程锁
PIGG_block_queue.h+PIGG_lock.h
7. http协议的处理
PIGG_http.cpp/.h
8. 补充了PIGG_webserver.cpp中epoll连接的部分
9. 线程池
PIGG_threadpool.h
10. mysql连接池
sql_connection_pool.cpp/.h




1.数据库可以最后再写上去，因为可以先不存到数据库
2.写.cpp文件，具体函数的时候，可以先把函数名字写出来，先搭出框架
3.定时器还没写


# 不会的
1.reactor，proactor
2.信号量
3.PIGG_http_conn::process()中的modfd
开个标志位，开始通讯，还是不会搞


# 扩展

1. 动态资源
2. 如何扩展成HTTPS
3. 使用json文件进行配置
4. 小顶堆实现定时器系统
5. 文件上传和下载的功能吗 类似于FTP那种的


# 参考文献
[日志系统]https://mp.weixin.qq.com/s?__biz=MzI3NzE0NjcwMg==&mid=2650122657&idx=1&sn=c5ce1d8059c40e4cd6deb42a34f8fe49&chksm=f36bb480c41c3d96f69a9fbbc8e7e1515b8bbec87742f76fa3dfda0019a7b58aa282c3ef9bde&scene=21#wechat_redirect
[Reactor模型和Proactor模型]https://cloud.tencent.com/developer/article/1488120
[Reactor模型]https://www.cnblogs.com/CodeBear/p/12567022.html
##### 参考的github仓库
[TinyWebServer]https://github.com/qinguoyi/TinyWebServer
[HAHA-WebServer里面添加了json配置]https://github.com/tomatowithpotato/HAHA-WebServer
[markparticle/WebServer]https://github.com/markparticle/WebServer
[本项目是一个基于Linux 的轻量级Web服务器，能够对浏览器的连接请求进行解析处理，处 理完之后给浏览器客户端返回一个响应，如文字、图片、视频等;
小顶堆实现定时器系统]https://github.com/huboy-zhao/MyWebserver
[使用C++11实现数据库连接池连接数据库]https://github.com/xbQAQ/httpServer