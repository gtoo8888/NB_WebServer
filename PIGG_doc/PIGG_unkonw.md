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


struct sigaction sa;
```


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







