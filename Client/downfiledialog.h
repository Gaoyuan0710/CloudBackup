
#ifndef DOWNFILEDIALOG_H
#define DOWNFILEDIALOG_H

#include<QFileDialog>
#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>

class DownFileDialog : public QDialog
{
    Q_OBJECT
public:
    DownFileDialog(QDialog* parent=0);
    ~DownFileDialog(){}
private slots:
    void LinePathShow(const QString &path);
    void DownLoadFile();
public:
    //当前被选中文件
    QString         file;
    //当前选中文件路径
    QString         filePath;
private:
    QFileDialog*    fileDialog;
    QLabel*         saveLabel;
    QLineEdit*      saveLineEdit;
    QPushButton*    saveButton;

};

#endif//DOWNFILEDIALOG_H

