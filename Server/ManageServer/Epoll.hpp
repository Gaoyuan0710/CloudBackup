#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<thread>
#include<list>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<atomic>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define LISTEN_NUMBER 1024


class Epoll{
    public:
        Epoll(const char * ip , const char * port):epoll_fd_(-1) , server_ip_(ip) , server_port_(port)  {}
        ~Epoll() {}

        int setNonBlock ( int fd )  {
            int old_option = fcntl( fd , F_GETFL );
            int new_option = old_option | O_NONBLOCK;
            fcntl( fd , F_SETFL , new_option);
            return old_option;
        }

        void AddFd( int epoll_fd, int fd )  {
            epoll_event event;
            event.data.fd = fd ;
            event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
            epoll_ctl( epoll_fd, EPOLL_CTL_ADD , fd , &event );
            setNonBlock( fd );
        }

        void CreateTcpSocket(  )  {
            int port = atoi(server_port_);
            struct sockaddr_in address;
            bzero( &address , sizeof(address));
            address.sin_family = AF_INET;
            inet_pton( AF_INET , server_ip_ , &address.sin_addr);
            address.sin_port = htons( port );

            if((socket_fd_ = socket( PF_INET , SOCK_STREAM , 0)) < 0 )  {
                std::cout << "Create Tcp Socket Error! "  << std::endl;
                return;
            }
            if((bind(socket_fd_ , (struct sockaddr*)&address , sizeof( address ))) == -1)  {
                std::cout << "Tcp Socket Bind Error!" << std::endl;
                return;
            }
            if((listen( socket_fd_ , LISTEN_NUMBER)) == -1)  {
                std::cout << "Socket Listen Error!" << std::endl;
                return;
            }
        }

        void RegisterSocket()  {
            if((epoll_fd_ = epoll_create(LISTEN_NUMBER)) == -1 )  {
                std::cout << "Create Epollfd Error!" << std::endl;
                return;
            }
            AddFd( epoll_fd_ , socket_fd_ );
        }

        int setEpollFd()  {
            int num;
            if((num = epoll_wait( epoll_fd_ , events , MAX_EVENT_NUMBER , -1)) < 0 )  {
                std::cout << "Create Epollfd Error!" << std::endl;
            }
            return num;
        }

    public:
        int epoll_fd_;
        int socket_fd_; //监听用
        const char * server_ip_;
        const char * server_port_;
        epoll_event events[MAX_EVENT_NUMBER];

};


