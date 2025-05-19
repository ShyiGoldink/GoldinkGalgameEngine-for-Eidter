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

void JsonContorller::writeToJson(QString filePath, QString WhatTo, QString writeWhat) {
    QJsonObject root;
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            root = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","文件不存在或无法打开");
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

QString JsonContorller::getInJson(QString filePath, QString whatTo) {
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
