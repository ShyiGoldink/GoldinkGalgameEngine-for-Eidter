#ifndef TXTSEARCH_H
#define TXTSEARCH_H

//这是专门用于管理txt文件内部的数据的

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDebug>

class txtSearch
{
public:
    txtSearch(QString rootPath);//创建对象时要将rootFile的路径传入其中
    void textWrite(QString writeWhat);//向文件中写某些东西
    QString textGet();//获取整个文件的东西
    bool textCheck();//专门用于检查这个txt文件是否是GoldinkGalgameEngine的专属文件
private:
    QString m_rootPath;
};

#endif // TXTSEARCH_H
