#ifndef JSONCONTORLLER_H
#define JSONCONTORLLER_H

//这个类的作用就是管理json，并实现对json的写入

#include <QString>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QJsonArray>
#include <QMessageBox>
#include <QDir>

class JsonContorller
{
public:
    JsonContorller();
    QString getProjectRootPath();//返回有rootFile.txt的父目录
    QString getInJson(const QString &filePath,const QString &whatTo);//从filePath处的json文件，返回其中“whatTo”结点下的数据
    void writeToJson(const QString &filePath,const QString &whatTo,const QString &writeWhat); //从filePath处的Json文件，向其中的whatTo写入writeWhat
    void projectSaveControl(const QString &savePath);//专门用于控制ProjectFile，也就是控制引导页之中提前预留好打开项目的位置（位于界面右边）
};

#endif // JSONCONTORLLER_H
