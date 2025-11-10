#ifndef TABS_H
#define TABS_H

//这是每个页面的布局

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "showmultimedia.h"
#include "workbench.h"

class Tabs : public QWidget
{
    Q_OBJECT
public:
    explicit Tabs(QWidget *parent = nullptr,QString projectPath = "");
    void load();
    void setStyle(const QString &witchStyle);
    void import(const QString &filePath);
    void uiFresh(bool isEnglish);
    void save();
    void setSmlPath(const QString &filePath);
    void setWbPath(const QString &filePath);//为workbench提供文件路径
    QString getSmlPath();

signals:
    void sendMessage(QString filePath,QString whatDo,QString whatBeDone,QVariant dataList);
private:
    QString m_projectPath;
    ShowMultimediaList *sml;
    WorkBench *wb;
};

#endif // TABS_H
