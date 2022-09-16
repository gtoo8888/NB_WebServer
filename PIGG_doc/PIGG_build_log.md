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

## 22022-9-16
1. 修改CMake文件http和sql_connection加入编译，修改语法问题

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



<!-- 8. mysql线程池
sql_connection_pool.cpp/.h -->
数据库可以最后再写上去，因为可以先不存到数据库

# 参考文献
[日志系统]https://mp.weixin.qq.com/s?__biz=MzI3NzE0NjcwMg==&mid=2650122657&idx=1&sn=c5ce1d8059c40e4cd6deb42a34f8fe49&chksm=f36bb480c41c3d96f69a9fbbc8e7e1515b8bbec87742f76fa3dfda0019a7b58aa282c3ef9bde&scene=21#wechat_redirect
[Reactor模型和Proactor模型]https://cloud.tencent.com/developer/article/1488120
[Reactor模型]https://www.cnblogs.com/CodeBear/p/12567022.html
##### 参考的github仓库
[TinyWebServer]https://github.com/qinguoyi/TinyWebServer
[HAHA-WebServer里面添加了json配置]https://github.com/tomatowithpotato/HAHA-WebServer
[markparticle/WebServer]https://github.com/markparticle/WebServer
