#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QVariant>

#define HLAYOUT #include <QHBoxLayout>

struct pointMessages
{
    QString whatisthis;
    float x;
    float y;
    QString inAnimation = "null";
    QString outAnimation = "null";
    QString filePath;

};

struct pointMessage
{
    QList<QString> whatHave;
    QList<pointMessages> messages;
    void setXY(float x,float y,QString setWhat){
        for(pointMessages &m_messages:messages){
            if(m_messages.whatisthis == setWhat){
                m_messages.x = x;
                m_messages.y = y;
            }
        }
    };
    void setAnimation(QString inType,QString outType,QString setWhat){
        for(pointMessages &m_messages:messages){
            if(m_messages.whatisthis == setWhat){
                m_messages.inAnimation = inType;
                m_messages.outAnimation = outType;
            }
        }
    };
    QString Cg = "";
    QList<QString> selects = {};
    int linkNum = 1;
    QString id = "";
    QList<QString> childId = {};
    bool isChoices = false;
    void setChoices(bool isIt){
        isChoices = isIt;
    }
    void setlinkNum(int num){
        linkNum = num;
    }


};

struct filePaths
{   bool isView = false;
    QString objectName;
    QString filePath;
    pointMessage viewPoint;
};



struct saves{
    QString filePath;//json文件地址
    QString doWhat;//要做什么
    QString whatBeDone;//对什么做
    QVariant data;//做的内容
};

struct mainSave{
    QString whatBeDone;
    QString DoWhat;
    QList<QString> data;
};

struct tree{
    int depth;
    int siblingIndex;
    int siblingCount;
};

#endif // STRUCTS_H
