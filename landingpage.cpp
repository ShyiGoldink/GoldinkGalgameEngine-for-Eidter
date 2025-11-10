#include "landingpage.h"
#include "ui_landingpage.h"

LandingPage::LandingPage(QWidget *parent, bool isStart)
    : QDialog(parent)
    , ui(new Ui::LandingPage)
    , m_isStart(isStart)
    , openSource(new TextButton(this, tr("必读介绍")))
    , newProject(new TextButton(this, tr("新建项目")))
    , openProject(new TextButton(this, tr("打开项目")))
    , exit(new TextButton(this,tr( "退出程序")))
    , geryLabel(new QLabel(this))
    , np(new NewProject(this,true))
    , osii(new OpenSourceIntro(this))
    , animationGroup(new QParallelAnimationGroup(this))
    , animationGroup1(new QParallelAnimationGroup(this))
{
    ui->setupUi(this);
    this->setWindowTitle("引导--欢迎使用GoldinkGalgameEngine");
    this->setWindowIcon(QIcon(":/new/prefix1/favicon.ico"));
    this->setMinimumSize(800, 450);

    jc.projectSaveSearch();
    geryLabel->setStyleSheet("QLabel{ background-color: rgb(140, 140, 140);border-radius: 3px;}");
    geryLabel->setMaximumSize(7,320);//设置一个灰色的圆柱

    QHBoxLayout *mainLayout = new QHBoxLayout(this);// 创建主布局（水平布局）
    QVBoxLayout *buttonsLayout = new QVBoxLayout();// 创建按钮布局（垂直布局）
    QVBoxLayout *projectLayout= new QVBoxLayout();//创建项目布局

    //初始化动画
    newProjectPageAnimation = new QPropertyAnimation(np,"pos");
    animationGroup->addAnimation(newProjectPageAnimation);
    newProjectPageAnimation1 = new QPropertyAnimation(osii,"pos");
    animationGroup1->addAnimation(newProjectPageAnimation1);

    buttonsLayout->addStretch(1);
    connect(this->openSource,&QPushButton::clicked,[this](){
        newAnimationSlot(newProjectPageAnimation1,animationGroup1,osii);
    });
    connect(osii,&OpenSourceIntro::exit,[this](){
        outAnimationSlot(newProjectPageAnimation1,animationGroup1);
    });
    buttonsLayout->addWidget(openSource);// 将按钮添加到按钮布局
    connect(this->newProject,&QPushButton::clicked,[this](){
        newAnimationSlot(newProjectPageAnimation,animationGroup,np);
    });
    connect(np,&NewProject::exit,[this](){
        outAnimationSlot(newProjectPageAnimation,animationGroup);
    });
    connect(np,&NewProject::newFinish,this,&LandingPage::newFinish);
    buttonsLayout->addWidget(newProject);
    connect(openProject,&QPushButton::clicked,this,&LandingPage::openProjectSlot);
    buttonsLayout->addWidget(openProject);
    connect(exit,&QPushButton::clicked,this,&LandingPage::exExit);
    buttonsLayout->addWidget(exit);
    buttonsLayout->addStretch(1);


    for(int i=1;i<=6;i++){
        QPushButton *pushbutton=new QPushButton(this);
        pushbutton->setMinimumHeight(60);
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Preferred);//修改决策来使其自动拓宽
        pushbutton->setSizePolicy(policy);
        pushbutton->setText(jc.getInJson(jc.getProjectRootPath()+"/ProjectSave.json","save"+QString::number(i)));
        pushbutton->setStyleSheet("color: rgb(87, 177, 255);");
        projectLayout->addWidget(pushbutton,1);
        connect(pushbutton,&QPushButton::clicked,[this, pushbutton](){
            emit openProjectSiganl(pushbutton->text());
        });

    }

    mainLayout->addStretch(14);
    mainLayout->addLayout(buttonsLayout, 10);
    mainLayout->addWidget(geryLabel,1);// 将按钮布局添加到主布局
    mainLayout->addLayout(projectLayout,28);

    this->setLayout(mainLayout);// 设置主布局


}

LandingPage::~LandingPage()
{
    delete ui;
}

void LandingPage::newAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget){
    newProject->setEnabled(false);
    openSource->setEnabled(false);
    //制作滑动动画
    widget->raise();
    widget->setVisible(true);
    widget->move(this->width(),0);
    animation->setDuration(300);
    animation->setStartValue(QPoint(this->width(), 0));
    animation->setEndValue(QPoint(this->width() - np->width(), 0));
    group->start();
}

void LandingPage::outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group){

    animation->setDuration(300);
    animation->setStartValue(QPoint(this->width() - np->width(), 0));
    animation->setEndValue(QPoint(this->width(), 0));
    group->start();
    newProject->setEnabled(true);
    openSource->setEnabled(true);

}

void LandingPage::openProjectSlot(){
    QString showPath = QFileDialog::getExistingDirectory(this, "选择文件路径", "/home");
    //添加检查逻辑
    emit openProjectSiganl(showPath);
}

void LandingPage::setStyle(QString witchStyle){
    if(witchStyle=="blackWhite"){
        openSource->setColor("#5a5a5a","black");
        newProject->setColor("#5a5a5a","black");
        openProject->setColor("#5a5a5a","black");
        exit->setColor("#5a5a5a","black");

        np->setStyle("blackWhite");

        osii->setStyle("blackWhite");

        this->setStyleSheet(
            "background-color:white;"
            "color:black;"
            );
    }
    else if(witchStyle=="pinkBlue"){
        openSource->setColor("rgb(0,115,175)","rgb(27,41,79)");
        newProject->setColor("rgb(0,115,175)","rgb(27,41,79)");
        openProject->setColor("rgb(0,115,175)","rgb(27,41,79)");
        exit->setColor("rgb(0,115,175)","rgb(27,41,79)");

        np->setStyle("pinkBlue");

        osii->setStyle("pinkBlue");

        this->setStyleSheet(
            "background-color: rgb(255, 239, 238);"
            "color: rgb(255, 213, 205);"
            );

    }

}

void LandingPage::uiFresh(bool isEnglish){
    if(isEnglish){
        openSource->setText("Introduction");
        newProject->setText("New Project");
        openProject->setText("Open Project");
        exit->setText("Exit");
        np->uiFresh(isEnglish);
        osii->uiFresh(isEnglish);
    }
    else{
        openSource->setText("必读介绍");
        newProject->setText("新建项目");
        openProject->setText("打开项目");
        exit->setText("退出程序");
        np->uiFresh(isEnglish);
        osii->uiFresh(isEnglish);
    }

}
