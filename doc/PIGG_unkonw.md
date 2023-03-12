# 不懂的语法问题
```cpp
ret = recv(PIGG_pipefd[0],signals,sizeof(signals),0);
void addsig(int sig, void(handler)(int), bool restart = true);
sigfillset(&sa.sa_mask);


SIGALRM
SIGTERM

sa.sa_flags |= SA_RESTART;

int old_option = fcntl(fd,F_GETFL);

setsockopt(PIGG_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

throw std::exception();
struct sigaction sa;
if(pthread_create(PIGG_threads + i,NULL,worker,this) != 0)
if(pthread_detach(PIGG_threads[i])){   
   struct iovec PIGG_iv[2];
#include <sys/uio.h>
```
我看一些项目用的set当定时器，感觉set是最均衡的

前向声明


# 装软件

webbench

beyond compare
# 面试时候的一些问题

1. 线程池中线程的数量这么少怎么应对高并发时那么多的连接请求呢
能高并发是因为有io多路复用，io多路复用就是一个线程能处理多个套接字，没io多路复用就是一个线程对应一个套接字
2. 没有Cookies怎么保持会话
要我现场加上重定向的处理

3. 如何排查性能瓶颈
### 使用压力测试工具
### 压测的瓶颈会在哪里
io同步读
存进内存
不同项目可能会有不同的点
### 怎么验证压测瓶颈的猜测
查看系统参数
内存
cpu
文件描述符
磁盘io
看内核态和用户态占用的cpu比率
火焰图

4. 有人尝试过给这个服务器实现文件上传和下载的功能吗 类似于FTP那种的
想问下 实现的话 主要难点是啥

5. 这个项目的挑战跟难点你们说啥，除了几个bug不太知道有什么挑战跟难点

6. http服务器算io密集型吗？就是负责解析http请求报文，返回http响应报文的服务器

7. 想问问这个是一个静态网页还是需要搭建到服务器上？

8. 现在你要监控一台服务器的状态，输出监控日志，请问如何将该日志分发到不同的机器上?

9. 如果一个客户请求需要占用线程很久的时间，会不会影响接下来的客户请求呢，有什么好的策略呢?这个问题咋回答

10. 问各位一个问题 为什么IO要有内核缓冲区 为什么不直接拷贝到用户缓冲区

11. 面试的时候面试官问能不能访问动态资源或者如何扩展成HTTPS，或许你可以试试？

12. 模拟proactor比传统的reactor好在哪里

13. 麻烦问下大家，项目里的数据库连接池和线程池有什么区别？
连接池：
1、连接池是面向数据库连接的
2、连接池是为了优化数据库连接资源
3、连接池有点类似在客户端做优化

线程池：
1.、线程池是面向后台程序的
2、线程池是是为了提高内存和CPU效率
3、线程池有点类似于在服务端做优化



14. 我问一个项目问题:现在刚发布了一个新服务，但是现在需要把一个百万级别的表导出到另一张需求表中，怎么做才能在不关闭服务的情况下，同时尽量不影响用户体验(即不影响sql查询)

15. 线程池怎么优化？
加管理者线程

16. 请问这个项目用到了哪些中断？
信号、数据拷贝？

17. 这项目优化的地方很多啊
数据库可以做一个redis做缓存
定时器可以优化
链表遍历时间复杂度较高
http解析也可以优化

18. epoll底层是什么呢
我说红黑树和就绪队列 他说不够底层
事件回调
您说是注册到epoll上的事件发生了变化后 传递给了就绪链表这个意思吗
epoll 有事件发生，调用回调函数，加入就绪链表

19. 为什么给定长度的基本数据序列化的时候需要考虑字节序，而未给定的基本数据不需要？
是不是考虑是否涉及在网络端有存储嘞

20. 多线程生产者消费者模型实现

21. put和post的区别，能用put的都能用post吗
总结了一下，仅供参考（有大佬指正和补充更好了）：1.put是幂等的，post不是(幂等指多次操作结果相同，比如多次执行x=1是幂等的,多次执行x += 1不是幂等的，所以能用put的不一定能用post) 2.put必须指出具体对象，如果对象不存在则创建对象，如果对象存在，完全替换对象；post不需指明要操作的对象，如果对象不存在则创建对象，如果对象存在，则修改对象(而不是完全替换)。

22. 为什么直接写char, short, int 不行，而要写int8, int16, int32?
不同系统的int 啥的长度不同吧
1.提高可移植性
2.提高运行速度


我居然还真听完了[笑哭]有够闲，就几个题说说我的想法吧，欢迎指正。
最简单的优先队列（大小根堆） 我觉得设计过delete的优先队列还是非常厉害的，如果说出很简答被问到实现和单元测试就g了[doge]
然后proactor现在内核有io uring只吃了，大家可以了解下，挺棒的
c++98还是03的字符串cow如果提的话最好研究清楚[笑哭]不然容易翻车。因为从11开始已经不用了，原因是有的，还很有意思👀
emplace back记得了解万能引用和完美转发，11的这玩意我觉得就是神
innodb的临键锁或者是间隙锁这种东西，可以从值和区间去理解，读写锁多少有点离谱了。
b+树对于我这种强迫症来说，没有缺点[妙啊]嘻嘻，或许稳定的查找导致的重复磁盘查询也是他的单点性能缺点吧。单点多就是b树好，范围多就是b+
redis为什么是单线程这个问题我其实很认同一个观点[doge]because they did it that way and it worked
信号一般来源于中断这个我是第一次听说。。。不知道有没有大佬能解释一下
信号类型的八股对我来说跟状态码一样，背到放弃，问就是不记得，活得更轻松了。


# 调试时发生的错误
1. Segmentation fault (core dumped)
分段错误（核心转储）

1.查看允许的core文件大小
ulimit -c
ulimit -c 命令查看允许的core文件大小，默认结果为0（kb），即无法生成core文件。
2.使用命令允许core生成
ulimit -c unlimited
3.使用gdb+core 定位段错误
gdb ./PIGG_webserve core
4.进入gdb
where
使用where定位位置


# 参考资料
[Segmentation fault (core dumped) 问题如何定位(linux C/C++中)]https://blog.csdn.net/c1724136570/article/details/122290751
[WebServer服务器项目可能会被问到的问题（一）]https://www.nowcoder.com/discuss/934904?type=all&order=recall&pos=&page=2&ncTraceId=&channel=-1&source_id=search_all_nctrack&gio_id=939FED971E9FCC25B724B4054BF9953A-1661177959694
[C++整型有__int8、__int16、__int32等等，为什么还要short、int、long？]https://www.zhihu.com/question/478664628






