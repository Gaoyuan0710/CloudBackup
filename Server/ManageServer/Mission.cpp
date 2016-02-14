/*************************************************************************
  > File Name: epoll.hpp
  > Author: lewin
  > Mail: lilinhan1303@gmail.com
  > Company:  Xiyou Linux Group
  > Created Time: 2015年07月27日 星期一 08时40分51秒
 ************************************************************************/

#include<iostream>
#include<array>
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


//#include"jsoncpp-src-0.5.0/include/json/json.h"
#include <json/json.h>
#include"MyDataBase.cpp"

//#include "cJSON.h"
//#define SIZE                1000
#define MaxClientConnection 2
#define WORKPORT            "10001"

extern MyDataBase  DataBase;
extern std::string work_ip;
extern std::string work_port;

class Mission{
    public:
        Mission()
        {}
        ~Mission() 
        {
            LOG(INFO) << "该任务销毁";
        }

        void MissionInit(int sock)
        {
            socket_fd_ = sock;
            Uid = Users[socket_fd_];
        }

        //验证账户密码任务
        //int AccountPasswd(char *root, int & socket_fd){
        int AccountPasswd(Json::Value & root , int & socket_fd_) {
            std::string user_name , passwd, root_folder;

            user_name = root["UserName"].asString();
            passwd = root["Passwd"].asString();

            
            int ret = DataBase.AccountPasswd(user_name , passwd);


            if(ret >= 1)
            {
                //登录成功
                Users[socket_fd_] = ret;
                Uid = ret;
                LOG(INFO) << "log Uid:" << Uid;
                //获取根目录文件
                root_folder = DataBase.DirFiles(Uid, "/");

                root["status"] = Json::Value(0);
                root["AllFiles"] = Json::Value(root_folder);
                root["Folder"] = Json::Value("/");
                
                strcpy(buf, root.toStyledString().c_str());

                LOG(INFO) << "root_folder :" << root_folder;
                int len = send(socket_fd_, buf, strlen(buf), 0);
                {
                    if ( len != strlen(buf)) {
                        LOG(ERROR) << "Error in send to Client";

                        return 0;
                    }
                }
                return 1;
            }

            else if(ret == 0)
            {
                //账号错误
                root["status"] = Json::Value(1);
                strcpy(buf,root.toStyledString().c_str());
                int len = send(socket_fd_, buf, strlen(buf), 0); 
                if (len != strlen(buf)){
                    LOG(ERROR) << "Send Account Error ";
                }
                return 0;
            }
            else
            {
                //密码错误
                root["status"] = Json::Value(2);

                strcpy(buf, root.toStyledString().c_str());
                int len = send(socket_fd_, buf, strlen(buf), 0);
                if (len != strlen(buf)) {
                    LOG(ERROR) << "PassWd Error";
                }
                return -1;
            }
        }

        //注册用户
        int Regiester(Json::Value & root , int & socket_fd_)  {
            std::string user_name , passwd , email;


            user_name = root["UserName"].asString();
            passwd = root["Passwd"].asString();
            email = root["Email"].asString();

            int ret = DataBase.Register(user_name , passwd , email);

            if(ret == 1)
            {
                //注册成功
                root["status"] = Json::Value(0);
                strcpy(buf, root.toStyledString().c_str());

                if (strlen(buf) != send(socket_fd_,buf,strlen(buf),0)) {
                    LOG(ERROR) << "Register send error";
                }
                return 1;  
            }
            else if(ret == 0)
            {
                //用户已存在
                root["status"] = Json::Value(1);
                strcpy(buf, root.toStyledString().c_str());
                
                if (strlen(buf) != send(socket_fd_, buf, strlen(buf), 0)) {
                    LOG(ERROR) << "User existend send error";
                }
                return 0;
            }
            else if(ret == 2)
            {
                //邮箱已被占用
                root["status"] = Json::Value(2);

                strcpy(buf, root.toStyledString().c_str());

                if (strlen(buf) != send(socket_fd_, buf, strlen(buf), 0)){
                    LOG(ERROR) << "mail exitsed send error";
                }
                return 2;
            }
            else
            {
                LOG(ERROR) <<"添加数据失败";

                return -1;
            }

        }

        //进入目录
        void  DirFiles(Json::Value root , int & socket_fd_)  {
            std::string  folder, all_files;
            

            folder = root["Folder"].asString();
            all_files = DataBase.DirFiles(Uid, folder);
            root["AllFiles"] = Json::Value(all_files);
            strcpy(buf, root.toStyledString().c_str());
            if (strlen(buf) != send(socket_fd_, buf, strlen(buf), 0)) {
                LOG(ERROR) << "DirFiles send error";
            }
        }
        //创建新目录
        int CreateNewDir(Json::Value root , int &  socket_fd_) {
            std::string dir_name;
            dir_name = root["UserFilePath"].asString();

            if( 1 == DataBase.CreateNewDir(dir_name)) {
                return 1;
            }
            return -1;
        }
        //重命名文件或目录
        int RenameFileName(Json::Value root , int & socket_fd_) {
            std::string old_name , new_name;

            old_name = root["OldName"].asString();
            new_name = root["NewName"].asString();

            if( 1 == DataBase.RenameFileName(old_name , new_name)) {
                return 1;
            }
            return -1;
        }
        //删除文件或目录
        int DeleteFileORDir(Json::Value root , int & socket_fd_)  {
            std::string file_name;
            file_name = root["UserFilePath"].asString();

            if( 1 == DataBase.DeleteFileORDir(file_name))  {
                return 1;
            }
            return -1;
        }

        //上传文件
        int UploadFile(Json::Value root , int & socket_fd_)  {
            LOG(INFO) << "start uid: " << Uid;
            LOG(INFO) <<"json : " << root.toStyledString();

            std::string md5_str, user_file_path, file_name, file_path;
            //获取上传文件名
            file_path = root["File"].asString();

    
            std::string::size_type    position;
            position = file_path.find_last_of("/");
            file_name = file_path.substr(position + 1, file_path.length());

            md5_str = root["Md5"].asString();
            int   have = root["Have"].asInt();

            //上传到客户端的路径
            user_file_path = root["Path"].asString() + file_name;

            std::string      user_file_size;
            std::string      server_ip;
            

            if(have == 1)
            {
                user_file_size = root["Size"].asString();
                server_ip = root["Ip"].asString();
                
            }


            int ret = DataBase.UploadFile(md5_str, user_file_path, Uid, server_ip, user_file_size, have);
            if(ret == 1)
            {
                //秒传
                root["Have"] = Json::Value(1);

                strcpy(buf,root.toStyledString().c_str());
                if (strlen(buf) != send(socket_fd_,buf,strlen(buf),0)) {
                    LOG(ERROR) << "秒传 send error";
                }
                return 1;
            }
            else if(ret == 0)
            {
                //返回子服务器信息
                root["Ip"] = Json::Value(work_ip);
                root["Port"] = Json::Value(work_port);
                root["Have"] = Json::Value(0);


                strcpy(buf, root.toStyledString().c_str());
                if (strlen(buf) != send(socket_fd_, buf, strlen(buf), 0)) {
                    LOG(ERROR) << "返回子服务器信息 send error";
                }
                return 0;
            }
            else if(ret == 2)
            {
                return 2;
            }
            return -1;
        }

        //下载文件
        void DownloadFile(Json::Value root , int & socket_fd_)  {

            std::string user_file_path , server_ip , file_size, file_md5_str;

            user_file_path = root["Path"].asString();

            MYSQL_RES*    res;
            MYSQL_ROW     row;

            res = DataBase.DownloadFile(user_file_path, Uid);


            while((row = mysql_fetch_row(res))!=NULL)
            {
                server_ip = row[5];  //服务器文件路径
                file_size = row[3];  //用户文件大小
                file_md5_str = row[6];   //文件md5
                break;
            }


            mysql_free_result(res);

            root["Ip"] = Json::Value(server_ip);   //子服务器Ip
            root["Port"] = Json::Value(WORKPORT); //子服务器端口
            root["Size"] = Json::Value(file_size);
            root["Md5"] = Json::Value(file_md5_str);

            

            strcpy(buf,root.toStyledString().c_str());

            //给客户端发过去
            if (strlen(buf) != send(socket_fd_ , buf , strlen(buf) , 0)) {
                LOG(ERROR) << "Dowload file send error";
            }
        }

        //监控文件
        int MonitorFile(Json::Value root , int & socket_fd_)  {
            std::string user_file_path , user_file_size , server_file_path , md5_str;
            int flag;

            user_file_path = root["UserFilePath"].asString();
            user_file_size = root["UserFileSize"].asString();
            server_file_path = root["ServerFilePath"].asString();
            md5_str = root["MD5"].asString();
            flag = root["flag"].asInt();


            //未修改
            if( true == DataBase.MonitorFile(user_file_path , user_file_size , server_file_path , md5_str , flag))  {
                return 1;
            }
            return -1;
        }

        //恢复文件
        void RestoreFile(Json::Value root , int & socket_fd_)  {
            Json::Value croot;
            std::string user_file_path , server_file_path;
            
            user_file_path = root["UserFilePath"].asString();
        //    server_file_path = DataBase.DownloadFile(user_file_path);
            croot["PATH"] = server_file_path;


            //给客户端发过去
            std::string s = croot.toStyledString();
            const char * buf = s.c_str();
            send(socket_fd_ , (void *)buf , sizeof(buf) , 0);
        }

        void Banlance(Json::Value root, int & socket_fd_)
        {
            LOG(INFO) << "Balance json str " << root.toStyledString();
            work_ip = root["ip"].asString();
            work_port = root["port"].asString();

            LOG(INFO) << "work_ip:" << work_ip << "  " << "work_port : " << work_port;
        }

        //根据用户给的行为来判断需要调用什么函数
        void UserRequest() {
            Json::Reader reader;
            Json::Value root;
            int status;


            if(reader.parse(buf,root))  {
                status = root["status"].asInt();
                LOG(INFO) << "user request :" << status;
            }
            LOG(INFO) << " buf json " << buf;
            switch (status)
            {
                case 0:
                    Banlance(root, socket_fd_);
                    break;
                case 1:
                    AccountPasswd(root , socket_fd_);
                    break;
                case 2:
                    Regiester(root , socket_fd_);
                    break;
                case 3:
                    DirFiles(root , socket_fd_);
                    break;
                case 4:
                    CreateNewDir(root , socket_fd_);
                    break;
                case 5:
                    RenameFileName(root , socket_fd_);
                    break;
                case 6:
                    DeleteFileORDir(root , socket_fd_);
                    break;
                case 7:
                    UploadFile(root , socket_fd_);
                    break;
                case 8:
                    DownloadFile(root, socket_fd_);
                    break;
                case 9:
                    MonitorFile(root , socket_fd_);
                    break;
                case 10:
                    RestoreFile(root , socket_fd_);
                    break;
            }
        }
    public:
//        MyDataBase db[MaxClientConnection];
        char    buf[1024];
        int     socket_fd_;
        int     Uid;      // 用户数据库ID
    private:
        static int Users[];  //保存登录用户的ID
//        static std::map<int, int> socket_fd_map_;
//        static std::map<int, int> user_id_map_;

};

