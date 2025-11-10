#ifndef SHOWMULTIMEDIA_H
#define SHOWMULTIMEDIA_H

//这是用于素材边栏的展示页面

#include <QObject>
#include <QListWidget>
#include <QDir>
#include <QMenu>
#include <QSettings>
#include <QCryptographicHash>
#include <QMimeData>
#include <QDrag>
#include "filecontroller.h"

class ShowMultimediaList: public QListWidget
{
    Q_OBJECT;
public:
    ShowMultimediaList(QWidget *parent = nullptr);
    void setProjectFilePath(const QString &filePath);//为对象提供文件路径
    void setStyle(QString witchStyle);//依然是修改类型函数
    void loadItem();//全面升级为QSetting保存，保证用户修改后的顺序
    void saveItem();//保存文件
    void importItem(const QString &filePath);//导入item
    ~ShowMultimediaList();
protected:
    void startDrag(Qt::DropActions supportedActions) override;

public slots:
    void showSlot(QListWidgetItem *item);
    void showMenu(const QPoint &point);

private:
    FileController fc;
    QString projectFilePath;
    QString key;

};

#endif // SHOWMULTIMEDIA_H
