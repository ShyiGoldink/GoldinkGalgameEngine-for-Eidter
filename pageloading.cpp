#include "pageloading.h"
#include "recivelabel.h"

PageLoading::PageLoading(QWidget *parent,QString loadjsonPath):
    QWidget(parent)
    ,m_loadjsonPath(loadjsonPath)
    ,animationGroup(new QParallelAnimationGroup(this))
    ,mainLayout(new QHBoxLayout(this))

{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //通过jsonController来u获取QVector来方柏霓selects使用
    setLayout(mainLayout);

    selects = new SelectBox(this
                            ,{tr("op动画"),tr("开头图片1"),tr("开头图片2"),tr("开头图片3"),tr("结束图片1"),tr("结束图片2"),tr("结束图片3"),tr("ed动画"),tr("op音频"),tr("ed音频")}
                            ,jc.getWhatHaveVector(m_loadjsonPath+"/jsons/oped.json"));
    selects ->move(-11.2*selects->width(),0);

    inAnimation = new QPropertyAnimation(selects,"pos");
    animationGroup -> addAnimation(inAnimation);

    connect(selects,&SelectBox::in,[this](){
        inAnimationSlot(inAnimation,animationGroup,selects);
    });

    connect(selects,&SelectBox::out,[this](){
        outAnimationSlot(inAnimation,animationGroup);
    });

    connect(selects,&SelectBox::outMessage,[this](QList<QString> outlist){
        emit updateData(m_loadjsonPath+"/jsons/oped.json","updata","WhatHave",QVariant::fromValue(outlist));
        emit updateMeida(outlist);
        setupLabels(outlist);
        qDebug()<<"middle"<<outlist;
    });
    mainLayout->setContentsMargins(selects->width()*2, 0, 0, 0);
    setupLabels(jc.getWhatHaveVector(m_loadjsonPath+"/jsons/oped.json"));

    for(const QString &objectName:jc.getWhatHaveVector(m_loadjsonPath+"/jsons/oped.json")){
        if(jc.getInJson(m_loadjsonPath+"/jsons/oped.json",objectName).isEmpty()){
            //m_filePaths.clear();
            continue;
        }
        filePaths a;
        a.objectName=objectName;
        a.filePath=jc.getInJson(m_loadjsonPath+"/jsons/oped.json",objectName);
        m_filePaths.append(a);
    }


}

void PageLoading::inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget){
    //可能的防呆设计还未设计

    widget->raise();
    widget->setVisible(true);
    widget->move(-widget->width()*0.91,0);
    animation->setDuration(300);
    animation->setStartValue(QPoint(-widget->width()*0.91,0));
    animation->setEndValue(QPoint(0,0));
    group->start();
}

void PageLoading::outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group){
    //可能的防呆设计还未设计


    animation->setDuration(300);
    animation->setStartValue(QPoint(0,0));
    animation->setEndValue(QPoint(-selects->width()*0.91,0));
    group->start();
}

void PageLoading::safeClearLayout(QLayout* layout)
{
    if (!layout) return;

    // 停止所有动画
    if (animationGroup->state() == QAbstractAnimation::Running) {
        animationGroup->stop();
    }

    // 递归清除布局
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QLayout* childLayout = item->layout()) {
            safeClearLayout(childLayout);
            childLayout->deleteLater();
        }
        else if (QWidget* widget = item->widget()) {
            widget->disconnect(); // 断开所有连接
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}



void PageLoading::setupLabels(QList<QString> labelNames){

    safeClearLayout(mainLayout);


    for(int i=0;i<labelNames.count();i+=4){
        QVBoxLayout *insideLayout = new QVBoxLayout();
        for(int j = 0;j<4 && (i+j)<labelNames.count();j++){
            ReciveLabel *label = new ReciveLabel(this,labelNames[i+j]);
            connect(label,&ReciveLabel::message,[this](QString whatTo,QString path){
                emit updateData(m_loadjsonPath+"/jsons/oped.json","updata",whatTo, QVariant(path));
                emit updataScreen(whatTo,path);
            });
            insideLayout->addWidget(label);
        }
        mainLayout->addLayout(insideLayout,2);

    }
    mainLayout->addStretch(2);
    selects->raise();
    this->update();
}

QList<filePaths> PageLoading::getMFilePaths(){
    return m_filePaths;
}
