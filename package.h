#ifndef PACKAGE_H
#define PACKAGE_H

#include "Structs.h"
#include "jsoncontorller.h"
#include <QString>
#include <QObject>
#include <QMap>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QApplication>


class package:public QObject
{
    Q_OBJECT;
public:
    package();
    void packageIt(const QString& projectPath,const QString& aimPath);
    QString divide(const QString& filePath);
private:
    bool setExecutableIcon(const QString& exePath, const QString& iconPath);
    void processIconAndExecutable(const QString& projectPath, const QString& aimPath);
    bool copyDirectory(const QString& sourcePath, const QString& destinationPath);

    JsonContorller jc;
    int m_pics;
    int m_videos;
    int m_mps;

    QMap<QString,QString> m_mirror;//<Origin,newEnglishName>对应表
};

#endif // PACKAGE_H
