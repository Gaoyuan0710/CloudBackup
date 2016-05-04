#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "server.h"
#include "./common/config.h"

int main(int argc, char *argv[])
{
    const char  config_file[] = "conf/config.txt";
    Config config_settings(config_file);

    std::string local_ip;
    std::string local_port;
    std::string balance_ip;
    std::string balance_port;
    int local_port_int;
    int balance_port_int;

    balance_ip = config_settings.ReadConf("BalanceServerIp", balance_ip);
    balance_port = config_settings.ReadConf("BalanceServerPort", balance_port);
    local_ip = config_settings.ReadConf("LocalServerIp", local_ip);
    local_port = config_settings.ReadConf("LocalServerPort", local_port);


    local_port_int = atoi(local_port.c_str());
    balance_port_int = atoi(balance_port.c_str());

    
    WorkServer server(local_ip, local_port_int, balance_ip, balance_port_int);
    server.Run();

    return EXIT_SUCCESS;
}


