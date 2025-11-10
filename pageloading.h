#ifndef PAGELOADING_H
#define PAGELOADING_H

//本类用于实现"首尾管理“设计页面

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "recivelabel.h"
#include "selectbox.h"
#include "jsoncontorller.h"
#include "Structs.h"


class PageLoading:public QWidget
{
    Q_OBJECT
public:
    PageLoading(QWidget *parent = nullptr,QString loadjsonPath = "");
    QList<filePaths> getMFilePaths();
signals:
    void updateMeida(QList<QString> dataList);
    void updateData(QString filePath,QString whatDo,QString whatBeDone,QVariant dataList);
    void updataScreen(QString objectName,QString filePath);

private:
    void inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget);
    void outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group);
    void setupLabels(QList<QString> labelNames);
    void safeClearLayout(QLayout* layout);

    QString m_loadjsonPath;
    QPropertyAnimation *inAnimation;
    QParallelAnimationGroup *animationGroup;
    SelectBox *selects;
    JsonContorller jc;
    QHBoxLayout *mainLayout;
    QList<ReciveLabel*> m_labels;
    QList<filePaths> m_filePaths;

};

#endif // PAGELOADING_H
