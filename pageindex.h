#ifndef PAGEINDEX_H
#define PAGEINDEX_H

//这是用于控制首页的页面

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "recivelabel.h"
#include "selectbox.h"
#include "jsoncontorller.h"
#include "Structs.h"

class PageIndex : public QWidget
{
    Q_OBJECT
public:
    explicit PageIndex(QWidget *parent = nullptr,QString loadjsonPath = "");
    QList<filePaths> getMFilePaths();
signals:
    void updateMeida(QList<QString> dataList);
    void updateData(QString filePath,QString whatDo,QString whatBeDone,QVariant dataList);
    void updataScreen(QString objectScreen,QString filePath);
private:
    void inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget);
    void outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group);
    void setUpLabels(QList<QString> labelNames);
    void safeClearLayout(QLayout* layout);

    QString m_loadjsonPath;
    QPropertyAnimation *inAnimation;
    QParallelAnimationGroup *m_group;
    SelectBox *selects;
    JsonContorller jc;
    QHBoxLayout *mainLayout;
    QList<ReciveLabel*> m_labels;
    QList<filePaths> m_filePaths;

};

#endif // PAGEINDEX_H
