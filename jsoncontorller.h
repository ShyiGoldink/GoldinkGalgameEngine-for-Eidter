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

class JsonContorller
{
public:
    JsonContorller();
    QString getProjectRootPath();//返回有rootFile.txt的父目录
    QString getInJson(QString filePath, QString whatTo);//从filePath处的json文件，返回其中“whatTo”结点下的数据
    void writeToJson(QString filePath,QString whatTo,QString writeWhat); //从filePath处的Json文件，向其中的whatTo写入writeWhat
};

#endif // JSONCONTORLLER_H
