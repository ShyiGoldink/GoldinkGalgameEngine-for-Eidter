#ifndef WORKBENCH_H
#define WORKBENCH_H

//这是用于工作的主界面，其用意在于，在上方展示被操控的物件
//在下方是操控的按钮
//将两者放在一起是便于设置信号传输等问题

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include "pageindex.h"
#include "pageloading.h"
#include "pagemain.h"
#include "screenwidget.h"



class WorkBench : public QWidget
{
    Q_OBJECT
public:
    explicit WorkBench(QWidget *parent = nullptr,QString filePath = "");
    void loadLayoutWidget();//用于加载layoutWidget的大小，便于当父窗口变化时，layoutWidget也能变化
    void setStyle(QString witchStyle);
    void uiFresh(bool isEnglish);
    void setFilePath(const QString &filePath);
    void setupChild();//用于建立child的合理结构
    void save();

protected:
    //重写一个事件监听器，用于调用loadLayoutWidget()
    bool eventFilter(QObject *watched,QEvent *event)override;
    void showEvent(QShowEvent *event)override {
        QWidget::showEvent(event);
        loadLayoutWidget();
    }
signals:
    void sendMessage(QString filePath,QString whatDo,QString whatBeDone,QVariant dataList);
private:
    QString m_filePath;//全部工作区的文件路径储存在这里
    QWidget *mainWidget;
    QStackedWidget *qsw;
    PageLoading *pageLoading;
    PageIndex *pageIndex;
    PageMain *pageGame;
    QWidget *pageSave;
    QWidget *pageSetting;
    QPushButton *loadingPage;
    QPushButton *indexPage;
    QPushButton *gamePage;
    QPushButton *savePage;
    QPushButton *settingPage;
    QPushButton *prefer;
    QPushButton *next;
    //未来可能存在多语言
    QLabel *yesorno;//用于表示是否能运行
    ScreenWidget *layoutWidget;
    JsonContorller jc;

};

#endif // WORKBENCH_H
