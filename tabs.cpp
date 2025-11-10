#include "tabs.h"

Tabs::Tabs(QWidget *parent,QString projectFilePath)
    : QWidget{parent}
    ,m_projectPath(projectFilePath)
    ,sml(new ShowMultimediaList(this))
    ,wb(new WorkBench(this,m_projectPath))
{
    QHBoxLayout *mianLayout = new QHBoxLayout(this);//主布局
    mianLayout->addWidget(sml,1);
    mianLayout->addWidget(wb,9);
    setStyle("pinkBlue");

    connect(wb,&WorkBench::sendMessage,[this](QString filePath,QString whatDo,QString whatBeDone,QVariant data){
        emit sendMessage(filePath,whatDo,whatBeDone,data);
        qDebug()<<"before final"<<data;
    }
            );
}

void Tabs::load(){

    sml->loadItem();

}

void Tabs::save(){
    sml->saveItem();
    wb->save();
}

void Tabs::import(const QString &filePath){

    sml->importItem(filePath);

}

void Tabs::setSmlPath(const QString &filePath){
    sml->setProjectFilePath(filePath);
    m_projectPath = filePath;
}

QString Tabs::getSmlPath(){
    return m_projectPath;
}

void Tabs::setStyle(const QString &witchStyle){
    if(witchStyle=="blackWhite"){
        this->setStyleSheet(
            "background-color:#fcfcfc;"
            "color:black;"
            );
        sml -> setStyle("blackWhite");
        wb -> setStyle("blackWhite");
    }
    else if(witchStyle=="pinkBlue"){
        this->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );
        sml -> setStyle("pinkBlue");
        wb -> setStyle("pinkBlue");
    }

}

void Tabs::uiFresh(bool isEnglish){
    wb -> uiFresh(isEnglish);
}

void Tabs::setWbPath(const QString &filePath){
    wb->setFilePath(filePath);
}
