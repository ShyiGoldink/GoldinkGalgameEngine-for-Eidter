#ifndef OPENPROJECT_H
#define OPENPROJECT_H

//这个类的作用就是打开工程文件夹

#include <QString>

class OpenProject
{
public:
    OpenProject();
private:
    QString m_rootFilePath;//储存打开文件的路径
};

#endif // OPENPROJECT_H
