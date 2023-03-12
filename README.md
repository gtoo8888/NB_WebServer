# PIGG_WebServer
自己写的C++服务器


## 编译环境
cmake version 3.16.3 <br/>
gcc version 9.4.0 <br/>
gcc version 9.4.0 <br/>
GNU Make 4.2.1 <br/>
Ubuntu 20.04.3 LTS <br/>
Linux version 5.13.0-48-generic <br/>
hiredis

查看g++的包含路径
g++ -v -E -x c++ - 
## 需要安装的软件
```shell
sudo apt-get install libmysqlclient-dev -y # mysql的头文件
sudo apt-get install libjsoncpp-dev # jsoncpp的头文件
sudo apt-get install cmake -y
sudo apt-get install g++ -y # gcc可能不全面，导致没有g++
sudo apt-get install libc6-dev-amd64 # 解决vscode大量找不到头文件问题
sudo apt-get install uuid-dev # 现在还没有用到
```

## 编译方法
./build.sh  这是shell脚本，里面调用cmake进行编译 <br/>
那个makefile是一个假的makefile，用来清理文件 

## 远程连接，vscode 调试方法
通过launch.json调用task.json，生成可执行文件 <br/>
F5直接可以开始调试

## 提交前需要做的
make clean

## 这个服务器的一些特点
现在写了的
+ [X]日志系统
+ [X]mysql的具体使用
+ [X]锁机制、对锁的测试
+ [X]多线程机制、线程池
+ [X]对http网页请求的响应
+ [X]使用json文件进行配置

测试项目
+ [ ] 日志系统：需要测试到达最大行数后的效果
+ [ ] 日志系统：在不加锁的情况下会发生什么
+ [ ] 如果将锁直接写在代码里，该怎么使用

http://192.168.43.12:8888