#include "get_info.h"

std::string
getDate::getInfo(int types)
{
    std::string retInfo;
    switch(types)
    {
        case cpuInfo:
            retInfo = getCpuInfo();
            break;

        case memInfo:
            retInfo = getMemInfo();
            break;

        case netInfo:
            retInfo = getNetInfo();
            break;
        
        case diskInfo:
            retInfo = getDiskInfo();
            break;
    }
    return retInfo;
}

std::string
getDate::getCpuInfo()
{
    FILE *fp;
    char buffer[512];
    /* 通过top命令获得cpu使用率, -n表示只执行一次 */
    std::string command("top -n 1 | grep Cpu | awk \'{print}\' > null");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
   // printf("cpu info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getMemInfo()
{
    FILE *fp;
    char buffer[512];
    /* 通过top命令获得memory使用率, -n表示只执行一次 */
    std::string command("top -n 1 | grep Mem | awk \'{print}\' > null");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("memory info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getNetInfo()
{
    FILE *fp;
    char buffer[512];
    /* net  sar */
    std::string command("sar -n DEV 1 1 | grep wlan0 | grep 平均时间 | awk \'{print}\' > null");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("net info:%s", buffer);
    pclose(fp);
    return buffer;
}

std::string 
getDate::getDiskInfo()
{
    FILE *fp;
    char buffer[512];
    /* iostat */
    std::string command("iostat | grep sda | awk \'{print}\' > null");
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        perror("popen error");
        exit(1);
    }
    fgets(buffer, 512, fp);
    //printf("disk info:%s", buffer);
    pclose(fp);
    return buffer;
}
