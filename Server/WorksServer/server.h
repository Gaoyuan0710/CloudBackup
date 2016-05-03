#ifndef _SERVER_H_
#define _SERVER_H_

/* c++ */
#include <iostream>
#include <string>
#include <list>   //任务队列大小和线程池大小必须相同
#include <thread>

/* c socket api */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* 线程池 */
#include "ThreadPool.h"

#include "client.h"

enum { MaxConnctionNum = 10000};

class WorkServer
{
    public:
        /* 构造函数，初始化ip和端口 , 初始化负载服务器的ip和端口*/
        WorkServer(std::string local_ip, int local_port, std::string balance_ip, int balance_port);


        /* 析构函数，关闭打开的socket */
        ~WorkServer();
    
        /* server运行函数 */
        int Run();
    
        /* 上传and下载 */
        static bool HandlerUpload(int fd, std::string json);
        static bool HandlerDownload(int fd, std::string json); 
    
    private:
        /* register登记连接的socket */
        int Register(int fd, bool oneshot);
        /*  set nonblocking  */
        int setnonblocking(int fd);



    private:
        /* ip和端口 */
        std::string ip;
        int port;

        /* 服务端结构体 */
        struct sockaddr_in server;
        struct epoll_event events[MaxConnctionNum];
        struct epoll_event ev;

        /* socket */
        int listen_fd;
        int epoll_fd;

        
        /* 线程池 */
        ThreadPool threadpool;

        /* 服务端对于负载均衡服务器充当Clinet */
        ServerClient loadClient;
};

#endif
