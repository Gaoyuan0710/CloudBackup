
#ifndef CONNECTSERV_H
#define CONNECTSERV_H

#include"myinclude.h"

#include<QString>

int  ConnectServer(QString,QString);
void closefd(int);
int  openfile(const char*);
int  openfile(const char* pathname,int flags);

#endif//CONNECTSERV_H
