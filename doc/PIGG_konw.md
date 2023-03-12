# 学习到的内容

## 服务器性能指标

## QPS
QPS：Queries Per Second
每秒查询
是一台服务器每秒能够相应的查询次数，是对一个特定的查询服务器在规定时间内所处理流量多少的衡量标准
更正确地称为每秒请求数（RPS：Request Per Second）。

例:
假如我们一天有10万pv(访问量),
公式 (100000 * 80%) / (86400*20%) = 4.62 QPS(峰值时间的每秒请求)

公式原理：每天80%的访问集中在20%的时间里，这20%时间叫做峰值时间。

那我们还可以转一下公式算出我们需要的机器数量

** 机器：峰值时间的每秒请求 / 单台的QPS = 机器数量 **

作者：南苑bert
链接：
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


## TPS
TPS：单位时间（每秒）处理的事务数。
TPS包括一条消息入和一条消息出，加上一次用户数据库访问。
TPS：Transactions Per Second（每秒传输的事物处理个数）

# RT
RT，响应时间
响应时间：执行一个请求从开始到最后收到响应数据所花费的总体时间,即从客户端发起请求到收到服务器响应结果的时间。

响应时间RT(Response-time)，是一个系统最重要的指标之一，它的数值大小直接反应了系统的快慢。


## PV
页面访问次数：Page View

## UV
访客数（去重复）：Unique Visitor

## Concurrency
并发数：同一时刻系统同时处理的请求数（相对并发，绝对并发）。
并发连接数
并发量（并发连接数）：指单台服务器每秒处理的连接数

## 高并发关键指标：
-响应时间(Response Time)
-吞吐量(Throughput)
-每秒查询率QPS(Query Per Second)
-每秒事务处理量TPS(Transaction Per Second)
-同时在线用户数量


2C4G机器单机一般1000QPS。
8C8G机器单机可承受7000QPS。

# 对于高并发场景需要具体分析
自己机器的情况：
4核16G单机


# 回调函数
传递参数变成了传函数
阻塞式回调
延迟式回调


# 其他的服务器
Asio是一个建立在Boost所提供的相关组件之上的异步的网络库
Netty 是一个利用 Java 的高级网络的能力，隐藏其背后的复杂性而提供一个易于使用的 API 的客户端/服务器框架。
Tomcat是一款Apache下面的开源的Servlet容器，实现了对Servlet和JSP规范的支持。另外 Tomcat 本身内含了一个 HTTP 服务器，所以也可以被当作一个 Web 服务器来使用。
Twisted是一个基于Reactor模式的异步IO网络框架
Lighttpd是开源的网络服务器软件。它专为资源有限的环境而设计，因为它消耗最少的CPU和RAM。它也适用于Windows和Linux操作系统 (OS)：
Nginx是十分轻量级的HTTP服务器，是一个高性能的HTTP和反向代理服务器，Nginx以事件驱动的方式编写，所以有非常好的性能，同时也是一个非常高效的反向代理、负载平衡。



# hiredis
git clone git@github.com:redis/hiredis.git
cd hiredis
make
make install

安装到了
/usr/local/include/hiredis
/usr/local/lib
后面链接时候报错：
确保你的/etc/ld.so.conf里面有 /usr/local/lib 这一行
sudo ldconfig


# 参考资料
[QPS高并发性能指标及其计算公式]https://blog.csdn.net/xili2532/article/details/120180162
[一文搞懂高并发性能指标：QPS、TPS、RT、并发数、吞吐量]https://zhuanlan.zhihu.com/p/337708438
https://www.jianshu.com/p/b57fb501505c
[一直再说高并发，多少QPS才算高并发？]https://cloud.tencent.com/developer/article/1476579
[Tomcat 简介]https://www.cnblogs.com/54chensongxia/p/13221769.html
[Twisted基础介绍（一）]https://zhuanlan.zhihu.com/p/84036822
[什么是Lighttpd网络服务器？]https://www.wbolt.com/lighttpd-web-server.html
[Apache、Lighttpd、Nginx 三种web服务器对比]https://www.cnblogs.com/rxbook/p/7085921.html
[关于C++ 回调函数(callback) 精简且实用]https://blog.csdn.net/zhoupian/article/details/119495949
[回调函数（callback）是什么？]https://www.zhihu.com/question/19801131
[Redis C++ API(hiredis)的完整使用教程]https://blog.csdn.net/weixin_44005915/article/details/94936436
[无法加载libhiredis.so库]https://blog.csdn.net/qq_45434780/article/details/115375309





