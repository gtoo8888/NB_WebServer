
```cpp
ret = recv(PIGG_pipefd[0],signals,sizeof(signals),0);
void addsig(int sig, void(handler)(int), bool restart = true);
sigfillset(&sa.sa_mask);









SIGALRM
SIGTERM

sa.sa_flags |= SA_RESTART;






struct sigaction sa;
```





