#include "jsoncontorller.h"

JsonContorller::JsonContorller() {}

QString JsonContorller::getProjectRootPath(){

    QDir dir(QCoreApplication::applicationDirPath());
    while (!dir.exists("rootFile.txt")) {
        if (!dir.cdUp()) {
            return "";
        }
    }
    return dir.absolutePath();

};

void JsonContorller::writeToJson(const QString &filePath,const QString &WhatTo,const QString &writeWhat) {
    QJsonObject root;
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            root = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            return;
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","文件不存在或无法打开");
        return;
    }


    root[WhatTo] = writeWhat;


    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","无法打开文件进行写操作");
    }
}

QString JsonContorller::getInJson(const QString &filePath,const QString &whatTo) {
    QJsonObject root;
    QFile file(filePath);

    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            root = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            file.close();
            return QString();
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
        return QString();
    }

    if (root.contains(whatTo)) {
        QJsonValue value = root.value(whatTo);
        if (value.isString()) {
            return value.toString();
        } else if (value.isDouble()) {
            return QString::number(value.toDouble());
        } else if (value.isBool()) {
            return value.toBool() ? "true" : "false";
        } else if (value.isNull()) {
            return "null";
        } else if (value.isObject() || value.isArray()) {
            QJsonDocument doc(value.isObject() ? value.toObject() : QJsonValue(value.toArray()).toObject());
            return doc.toJson(QJsonDocument::Compact);
        }
    } else {
        QMessageBox::warning(nullptr,"warning","写入文件失败，请检查文件是否存在！");
    }

    return QString(); // 默认返回空字符串
}

void JsonContorller::projectSaveControl(const QString &newFilePath){//后需要添加加载功能，即在屏幕左下角放一个猫猫罐子用于显示是否成功
    //首先处理newFilePath，得到项目的名称
    QDir dir(newFilePath);
    if(dir.isEmpty()){
        QMessageBox::warning(nullptr,"warning","项目不存在！");
        return;
    }

    QString fileName=dir.dirName();

    //接下来做处理逻辑，首先先解析Json文件
    QString jsonPath=getProjectRootPath()+"/ProjectSave.json";
    QJsonObject obj;
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            obj = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            file.close();
            return;
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
    }
    //这一步依然属于处理逻辑中，但是现在做的是将5->6,4->5..直到1然后1再设置为新的内容
    for(int i=6;i>1;i--){
        QString j="save"+QString::number(i-1);
        QString k="save"+QString::number(i);
        obj[k]=obj[j];
    }
    obj["save1"]=newFilePath;

    //写入文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","无法打开文件进行写操作");
    }
}
