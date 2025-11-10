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
#include <QVector>
#include "Structs.h"

class JsonContorller
{
public:
    JsonContorller();
    QString getProjectRootPath();//返回有rootFile.txt的父目录
    QString getInJson(const QString &filePath,const QString &whatTo);//从filePath处的json文件，返回其中“whatTo”结点下的数据
    void writeToJson(const QString &filePath,const QString &whatTo,const QString &writeWhat); //从filePath处的Json文件，向其中的whatTo写入writeWhat
    void projectSaveControl(const QString &savePath);//专门用于控制ProjectFile，也就是控制引导页之中提前预留好打开项目的位置（位于界面右边）
    void userSettingControl(const QString &whatTo,const QString &writeWhat);//专门向userSetting里写东西
    void projectSaveSearch();//专门用于检查是否存在空的项目路径并进行管理
    QVector<QString> getWhatHaveVector(const QString &jsonPath);
    void mixControl(const QString &filePath,const QString &doWhat,const QString &whatBeDone,QVariant what);//用于综合管理保存
    QJsonValue changeToJsonValue(QVariant);
    QList<filePaths> loadPageMain(const QString &jsonPath);
    bool savePageMain(const QString& saveDir, const QList<filePaths>& data);
    bool addFilePathsToMainJson(const QString& mainJsonPath, const filePaths& newData);
    void fontFielpathContol(const QString& filePath,const QString& fontName);
    void saveFontUserUse(const QString& fontName);
    QString getFontUserUse();
    QMap<QString, QString> getAllFontsFromJson();
    bool writeJsonObject(const QString& filePath, const QJsonObject& jsonObject);
    bool writeJsonArray(const QString& filePath,  const QJsonArray& jsonArray);
    QJsonObject filePathsToJson(const filePaths& fp);

private:
    QJsonObject pointMessagesToJson(const pointMessages& msg);
    QJsonObject pointMessageToJson(const pointMessage& pm);

};

#endif // JSONCONTORLLER_H
