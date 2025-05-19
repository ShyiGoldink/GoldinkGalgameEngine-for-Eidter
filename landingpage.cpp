#include "landingpage.h"
#include "ui_landingpage.h"

LandingPage::LandingPage(QWidget *parent, bool isStart)
    : QDialog(parent)
    , ui(new Ui::LandingPage)
    , m_isStart(isStart)
    , openSource(new TextButton(this, "开源介绍"))
    , newProject(new TextButton(this, "新建项目"))
    , openProject(new TextButton(this, "打开项目"))
    , exit(new TextButton(this, "退出"))
    , geryLabel(new QLabel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("引导--欢迎使用GoldinkGalgameEngine");
    this->setWindowIcon(QIcon(":/new/prefix1/favicon.ico"));
    this->setMinimumSize(800, 450);

    geryLabel->setStyleSheet("QLabel{ background-color: rgb(140, 140, 140);border-radius: 3px;}");
    geryLabel->setMaximumSize(7,320);//设置一个灰色的圆柱

    QHBoxLayout *mainLayout = new QHBoxLayout(this);// 创建主布局（水平布局）
    QVBoxLayout *buttonsLayout = new QVBoxLayout();// 创建按钮布局（垂直布局）
    QVBoxLayout *projectLayout= new QVBoxLayout();//创建项目布局

    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(openSource);// 将按钮添加到按钮布局
    buttonsLayout->addWidget(newProject);
    buttonsLayout->addWidget(openProject);
    buttonsLayout->addWidget(exit);
    buttonsLayout->addStretch(1);


    for(int i=1;i<=6;i++){
        QPushButton *pushbutton=new QPushButton(this);
        pushbutton->setMinimumHeight(60);
        QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Preferred);//修改决策来使其自动拓宽
        pushbutton->setSizePolicy(policy);
        pushbutton->setText(jc.getInJson(jc.getProjectRootPath()+"/ProjectSave.json","save"+QString::number(i)));
        projectLayout->addWidget(pushbutton,1);

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
