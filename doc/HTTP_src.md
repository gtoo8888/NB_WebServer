# wireshark抓包到的数据
## HTTP请求 GET / HTTP/1.1\r\n
```shell
GET / HTTP/1.1\r\n
Host: 192.168.43.12:8888\r\n
Connection: keep-alive\r\n
Cache-Control: max-age=0\r\n   # 缓存服务器会将请求转发给源服务器
Upgrade-Insecure-Requests: 1\r\n    # 请求头向服务器发送一个客户端对HTTPS加密和认证响应良好
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36 Edg/106.0.1370.47\r\n
# 网站服务器通过识别 “UA”来确定用户所使用的操作系统版本、CPU 类型、浏览器版本等信息，反爬虫
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n
# 浏览器能处理的媒体类型和媒体类型的优先级
# application/xhtml+xml表示 xhtml 文档；
# application/xhtml+xml,
# pplication/xml表示 xml 文档。
# q=0.9表示优先级
Accept-Encoding: gzip, deflate\r\n
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n
\r\n
[Full request URI: http://192.168.43.12:8888/]
[HTTP request 1/1]
[Response in frame: 2291]
```

## HTTP响应 HTTP/1.1 200 OK\r\n
```shell
HTTP/1.1 200 OK\r\n
Content-Length:586\r\n
Connection:keep-alive\r\n
\r\n
[HTTP response 1/1]
[Time since request: 0.012247000 seconds]
[Request in frame: 2287]
[Request URI: http://192.168.43.12:8888/]
File Data: 586 bytes
Data (586 bytes)
# 这里面包含了具体的内容，HTML文件的内容
```


## HTTP请求 GET /favicon.ico HTTP/1.1 
浏览器自带的内容请求一个图标



## HTTP请求 POST /0 HTTP/1.1\r\n
```shell
POST /0 HTTP/1.1\r\n
Host: 192.168.43.12:8888\r\n
Connection: keep-alive\r\n
Content-Length: 0\r\n
Cache-Control: max-age=0\r\n
Upgrade-Insecure-Requests: 1\r\n
Origin: http://192.168.43.12:8888\r\n
Content-Type: application/x-www-form-urlencoded\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36 Edg/106.0.1370.47\r\n
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n
Referer: http://192.168.43.12:8888/\r\n
Accept-Encoding: gzip, deflate\r\n
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n
\r\n
[Full request URI: http://192.168.43.12:8888/0]
[HTTP request 1/1]
[Response in frame: 59]
```



## HTTP请求 POST /3CGISQL.cgi HTTP/1.1\r\n
```shell
POST /3CGISQL.cgi HTTP/1.1\r\n
Host: 192.168.43.12:8888\r\n
Connection: keep-alive\r\n
Content-Length: 22\r\n
Cache-Control: max-age=0\r\n
Upgrade-Insecure-Requests: 1\r\n
Origin: http://192.168.43.12:8888\r\n
Content-Type: application/x-www-form-urlencoded\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36 Edg/106.0.1370.47\r\n
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n
Referer: http://192.168.43.12:8888/0\r\n
Accept-Encoding: gzip, deflate\r\n
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n
\r\n
[Full request URI: http://192.168.43.12:8888/3CGISQL.cgi]
[HTTP request 1/1]
File Data: 22 bytes
HTML Form URL Encoded: application/x-www-form-urlencoded
```


# PIGG_webserver接收到的HTTP 请求
GET / HTTP/1.1
Host: 192.168.43.12:8888
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36 Edg/106.0.1370.47
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6


2112-10-21 13:36:50.920472 [info]:
2112-10-21 13:36:50.920536 [info]:request:HTTP/1.1 200 OK

2112-10-21 13:36:50.920561 [info]:request:HTTP/1.1 200 OK
Content-Length:586

2112-10-21 13:36:50.920577 [info]:request:HTTP/1.1 200 OK
Content-Length:586
Connection:keep-alive

2112-10-21 13:36:50.920592 [info]:request:HTTP/1.1 200 OK
Content-Length:586
Connection:keep-alive


