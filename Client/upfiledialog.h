#ifndef UPFILEDIALOG_H
#define UPFILEDIALOG_H

#include<QFileDialog>

class UpFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    UpFileDialog(QFileDialog* parent=0);
    ~UpFileDialog(){}
public slots:
    void UpLoadFile();
    void GetSelectedPath(const QString&);
public:
    QString           currentPath;    //选中的文件路径
    QString           clientPath;     //客户端上传路径
};

#endif//UPFILEDIALOG_H
