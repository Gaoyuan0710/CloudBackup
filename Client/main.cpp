#include"mainwindow.h"
#include"enterwidget.h"
#include"addmonitorwidget.h"

#include<QApplication>
#include<QTextCodec>
#include<QString>

int          sockFd = -1;
QString      servIp;
QString      servPort;
QString      userName;

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //设置编码
/*
    AddMonitorWidget    w;
    w.resize(800,600);
    w.show();
*/
    
    MainWindow    *mainWindow = new MainWindow;

//    EnterWidget   enterWidget;
//    mainWindow->mainSplitter->show();
    LoginWidget loginWidget;
    loginWidget.show();
    
    QObject::connect(&loginWidget,SIGNAL(LoginOk(QString,QString,int,QString,QString)),mainWindow,SLOT(ShowMainWindow(QString,QString,int,QString,QString)));

    
    return app.exec();
}
