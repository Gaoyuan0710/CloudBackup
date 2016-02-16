/* 
* 获取此台机器的负载信息类
* 可以获取
* cpu使用率
* 带宽
* 磁盘IO
* */


#ifndef _GETINFO_H_
#define _GETINFO_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>   //popen

/* 4种负载信息 */
enum infoType{
    cpuInfo,
    memInfo,
    netInfo,
    diskInfo
};

/* 获取负载信息类 */
class getDate
{
    public:
        getDate() { }
        std::string getInfo(int types);
        std::string getCpuInfo();
        std::string getMemInfo();
        std::string getNetInfo();
        std::string getDiskInfo();
};


#endif
