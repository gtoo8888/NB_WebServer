# NB_WebServer
自己写的C++服务器


## 编译环境

cmake version 3.16.3

gcc version 9.4.0

gcc version 9.4.0

GNU Make 4.2.1

Ubuntu 20.04.3 LTS

Linux version 5.13.0-48-generic

## 编译方法
./build.sh  这是shell脚本，里面调用cmake进行编译
那个makefile是一个假的makefile，用来清理文件

## 远程连接，vscode 调试方法
通过launch.json调用task.json，生成可执行文件
F5直接可以开始调试

## 提交前需要做的
make clean


## 注意事项
使用vscode远程登录调试的时候，需要用root身份登录，不然无法连接数据库

http://192.168.0.103:9006/