#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

//这是引导页面，目的是为了引导用户对本游戏引擎使用

#include <QDialog>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "textbutton.h"
#include "jsoncontorller.h"
#include "newproject.h"
#include "opensourceintro.h"


namespace Ui {
class LandingPage;
}

class LandingPage : public QDialog
{
    Q_OBJECT
signals:
    void exExit();//用于向外传递退出信号

private slots:
    void newAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget);//这是点击动画，用在这里是为了美观，当用户点击新建项目之后，“新建项目”
                            //这几个字会向左移动，并且变大，同时无法被点击，窗口大小也不能再移动
                            //同时界面的右边会从右移动出一个界面，用于创建新项目，点击取消后回复
    void outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group);//这就是取消动画槽函数



public:
    explicit LandingPage(QWidget *parent = nullptr,bool isStart = true);//构造函数中，isStart是为了方便用户的使用，
                                                                               //如果是在使用前，则传入true，正常使用
                                                                               //如果是在使用中打开引导页寻求帮助，则有一些功能无法使用
    void setStyle(QString whichStyle);
    ~LandingPage();

private:
    Ui::LandingPage *ui;
    bool m_isStart;
    TextButton *openSource;
    TextButton *newProject;
    TextButton *openProject;
    TextButton *exit;
    QLabel *geryLabel;
    JsonContorller jc;
    NewProject *np;
    OpenSourceIntro *osi;


    QPropertyAnimation *newProjectPageAnimation;//用于控制页面的滑入滑出动画
    QPropertyAnimation *newProjectPageAnimation1;//用于控制openSource的播放
    QParallelAnimationGroup *animationGroup;//动画组
    QParallelAnimationGroup *animationGroup1;//动画组2

};

#endif // LANDINGPAGE_H
