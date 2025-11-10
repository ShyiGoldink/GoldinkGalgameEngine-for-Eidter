#include "pageindex.h"

PageIndex::PageIndex(QWidget *parent,QString loadjsonPath)
    : QWidget(parent)
    ,m_loadjsonPath(loadjsonPath)
    ,m_group(new QParallelAnimationGroup(this))
    ,mainLayout(new QHBoxLayout(this))
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setLayout(mainLayout);

    selects = new SelectBox(this
                            ,{"开始游戏","快速开始","存档","鉴赏","设置","退出游戏","背景","BGM",
                                      "开始游戏:Hover","快速开始:Hover","存档:Hover","鉴赏:Hover","设置:Hover","退出游戏:Hover","Hover音效","点击音效"}
                            ,jc.getWhatHaveVector(m_loadjsonPath+"/jsons/index.json"));
    selects -> move(-11.2*selects->width(),0);

    inAnimation = new QPropertyAnimation(selects,"pos");
    m_group->addAnimation(inAnimation);
    connect(selects,&SelectBox::in,[this](){
        inAnimationSlot(inAnimation,m_group,selects);
    });

    connect(selects,&SelectBox::out,[this](){
        outAnimationSlot(inAnimation,m_group);
    });

    connect(selects,&SelectBox::outMessage,[this](QList<QString> outList){
        emit updateData(m_loadjsonPath+"/jsons/index.json","updata","WhatHave",QVariant::fromValue(outList));
        emit updateMeida(outList);
        setUpLabels(outList);
    });

    mainLayout->setContentsMargins(selects->width()*2,0,0,0);
    setUpLabels(jc.getWhatHaveVector(m_loadjsonPath+"/jsons/index.json"));

    filePaths a;
    a.isView = true;

    for(const QString &objectName:jc.getWhatHaveVector(m_loadjsonPath+"/jsons/index.json")){
        if(jc.getInJson(m_loadjsonPath+"/jsons/index.json",objectName).isEmpty()){
            continue;
        }
        a.viewPoint.whatHave.append(objectName);
        pointMessages b;
        b.whatisthis = objectName;
        b.filePath = jc.getInJson(m_loadjsonPath+"/jsons/index.json",objectName);
        b.x = jc.getInJson(m_loadjsonPath+"/jsons/index.json",objectName+"X").toFloat();
        b.y = jc.getInJson(m_loadjsonPath+"/jsons/index.json",objectName+"Y").toFloat();
        //涉及到xy的储存方式，稍后进行设计
        a.viewPoint.messages.append(b);

    }
    m_filePaths.append(a);
    /*qDebug()<<a.isView<<a.filePath<<a.viewPoint.whatHave
             <<a.viewPoint.messages[0].filePath<<a.viewPoint.messages[0].x
             <<a.viewPoint.messages[0].y<<a.viewPoint.messages[0].whatisthis
             <<a.objectName;*/
}

void PageIndex::inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget){
    //可能的防呆设计还未设计

    widget->raise();
    widget->setVisible(true);
    widget->move(-widget->width()*0.91,0);
    animation->setDuration(300);
    animation->setStartValue(QPoint(-widget->width()*0.91,0));
    animation->setEndValue(QPoint(0,0));
    group->start();
}

void PageIndex::outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group){
    //可能的防呆设计还未设计


    animation->setDuration(300);
    animation->setStartValue(QPoint(0,0));
    animation->setEndValue(QPoint(-selects->width()*0.91,0));
    group->start();
}


void PageIndex::safeClearLayout(QLayout* layout)
{
    if (!layout) return;

    // 停止所有动画
    if (m_group->state() == QAbstractAnimation::Running) {
        m_group->stop();
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

void PageIndex::setUpLabels(QList<QString> labelNames){

    safeClearLayout(mainLayout);


    for(int i=0;i<labelNames.count();i+=4){
        QVBoxLayout *insideLayout = new QVBoxLayout();
        for(int j = 0;j<4 && (i+j)<labelNames.count();j++){
            ReciveLabel *label = new ReciveLabel(this,labelNames[i+j]);
            connect(label,&ReciveLabel::message,[this](QString whatTo,QString path){
                emit updateData(m_loadjsonPath+"/jsons/index.json","updata",whatTo, QVariant(path));
                emit updateData(m_loadjsonPath+"/jsons/index.json","updata",whatTo+"X", 0);
                emit updateData(m_loadjsonPath+"/jsons/index.json","updata",whatTo+"Y", 0);
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

QList<filePaths> PageIndex::getMFilePaths(){
    return m_filePaths;
}
