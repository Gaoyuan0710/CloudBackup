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

#include <glog/logging.h>

#include"epoll_easy.hpp"
#include"mission.cpp"
#include"thread_pool.hpp"

#include "./common/config.h"

//#define MaxClientConnection 2
#define MAXUSERS            1000
#define LOGDIR "log"
#define MKDIR "mkdir -p "LOGDIR

std::string       work_ip;
std::string       work_port;
std::string       manage_server_ip;
std::string       manage_server_port;

MyDataBase        DataBase;        //数据库


void InitServer()
{
    const char config_file[] = "conf/config.txt";

    Config config_settings(config_file);

    work_ip = config_settings.ReadConf("WorkServerIp", work_ip);
    work_port = config_settings.ReadConf("WorkServerPort", work_port);
    manage_server_ip = config_settings.ReadConf("ManageServerIp", manage_server_ip);
    manage_server_port = config_settings.ReadConf("ManageServerPort", manage_server_port);

    system(MKDIR);
    FLAGS_log_dir = "./log";
 //   google::SetStderrLogging(google::INFO); 

 //   google::SetLogDestination(google::INFO,LOGDIR"/INFO_"); //设置 google::INFO 级别的日志存储路径和文件名前缀
 //   google::SetLogDestination(google::WARNING,LOGDIR"/WARNING_");   //设置 google::WARNING 级别的日志存储路径和文件名前缀
 //   google::SetLogDestination(google::ERROR,LOGDIR"/ERROR_");   //设置 google::ERROR 级别的日志存储路径和文件名前缀
    google::InstallFailureSignalHandler();

    LOG(ERROR) << "ceshi";


}
template<typename T>
void RecvFromClient( Epoll & e , const int & socket_fd_ , ThreadPool<T> & pool , 
        std::map<int, Mission *>  &socket_mission_map) {
    char buf[TCP_BUFFER_SIZE];
    while(1) {
        memset( buf , '\0' , TCP_BUFFER_SIZE );
        int ret = recv( socket_fd_ , buf , TCP_BUFFER_SIZE - 1 , 0);
        if( 0 > ret )  {
            if((errno == EAGAIN ) || (errno == EWOULDBLOCK )) {
                break;
            }
            close(socket_fd_);
            break;
        }
        else if( 0 == ret ) {
            close( socket_fd_ );
        }
        else{
            //判断用户的行为
            //UserRequest(buf,socket_fd_);
            Mission *mission = new Mission();
            strcpy(mission->buf,buf);
//            mission[socket_fd_].socket_fd_ = socket_fd_;

            mission->MissionInit(socket_fd_);
            socket_mission_map.insert(pair<int, Mission *>(socket_fd_, mission));
            pool.AddTask(*mission);
            std::cout<<"------\n";
        }
    }
}

template<typename T>
void EpollMission( Epoll & e , ThreadPool<T> & pool ,std::string & ip , std::string & port , 
        std::map <int, Mission *> & socket_mission_map)  {
    int num;
//  int socketfds[MaxClientConnection];
    e.CreateTcpSocket();
    e.RegisterSocket();
    while(1) {
        num = e.setEpollFd();
        for(int i = 0 ; i < num ; ++i )  {
            int socket_fd_ = e.events[i].data.fd;
            if( socket_fd_ == e.socket_fd_) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( e.socket_fd_ , (struct sockaddr *)&client_address , &client_addrlength);
                if( connfd >= 0 )  {
                    std::cout << "Already connect!" << std::endl; //system log
                }
                e.AddFd(e.epoll_fd_ , connfd);
            }
            else if( e.events[i].events & EPOLLIN )  {
                RecvFromClient( e , socket_fd_ , pool , socket_mission_map);
            }

        }

    }
    close(e.socket_fd_);
}

//保存登录用户的ID
int      Mission::Users[MAXUSERS+3] = {0};
std::map <int, int> user_fd_map;


int main( int argc , char * argv[] )  {

    //if(argc < 3) {
    //    LOG(ERROR) << "Wrong argv";
    //    exit(0);
   // }

    google::InitGoogleLogging(argv[0]);
    InitServer();
    pthread_t epoll_thread_id;
    std::map <int, Mission *> socket_mission_map;
    
    //Mission * mission = new Mission[MAXUSERS + 3];
    ThreadPool<Mission> pool(2);

    //Epoll e(argv[1] , argv[2]);
    Epoll e(manage_server_ip.c_str(), manage_server_port.c_str());

    //EpollMission(e , pool ,argv[1] , argv[2] , socket_mission_map);
    EpollMission(e , pool , manage_server_ip,  manage_server_port, socket_mission_map);

    return EXIT_SUCCESS;
}
