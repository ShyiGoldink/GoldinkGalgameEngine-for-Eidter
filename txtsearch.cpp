#include "txtsearch.h"

txtSearch::txtSearch(QString rootPath)
    :m_rootPath(rootPath)
{}

QString txtSearch::textGet(){
    QFile file(m_rootPath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"无法打开文件并进行读取"<<m_rootPath;
        file.close();
        return QString();
    }

    QTextStream texts(&file);
    QString allContent = texts.readAll();

    file.close();

    return allContent;
}

void txtSearch::textWrite(QString writeWhat){
    QFile file(m_rootPath);
    if(file.open(QIODevice::Append|QIODevice::Text)){
        QTextStream texts(&file);
        texts<<(writeWhat);
        file.close();
    }
    else{
         file.close();
        qDebug()<<"无法打开文件进行写入";
    }
}

bool txtSearch::textCheck(){
    QFile file(m_rootPath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream texts(&file);
        QString context = texts.readLine();
        file.close();
        if(context == "#GoldinkGalgameEngine"){
            return true;
        }
    }
    else{
        file.close();
        qDebug()<<"无法打开文件并进行读取";
        return false;
    }
    return false;
}
