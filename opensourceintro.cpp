#include "opensourceintro.h"
#include "ui_opensourceintro.h"

OpenSourceIntro::OpenSourceIntro(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::OpenSourceIntro)
    ,label(new QLabel(this))
    ,widget(new QWidget(this))
    ,pushButton(new QPushButton(widget))
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout=new QVBoxLayout(this);//主布局
    QHBoxLayout *buttonLayout=new QHBoxLayout();//按钮布局

    buttonLayout->addStretch(3);
    pushButton->setText(tr("我知道了"));
    connect(pushButton,&QPushButton::clicked,this,&OpenSourceIntro::exit);
    buttonLayout->addWidget(pushButton);
    buttonLayout->addStretch(1);

    QFont font;
    font.setPointSize(15);

    label->setFont(font);
    label->setText(tr("* This software uses:/n* - SDL3 (zlib Licenses/n - VLC (GPLv2/LGPLv2.1) - Qt (LGPLv3) - JSON for Modern C++ (MIT License)/n * See LICENSE file for details./n"
                      "欢迎使用GoldinkGalgameMaker0.0.1测试版本！/n本版本尚未完善，使用时有以下注意事项：/n"
                      "1.大小限制为2560*1440/n2.op/ed务必至少一个视频一张图片/n3.提前准备一张.ico用于渲染图标/n4.得到的游戏程序可能存在偶尔闪退，属于正常情况/n5.其它问题关注B站up主“翔佑”喵！"));

    mainLayout->addWidget(label,5);
    mainLayout->addLayout(buttonLayout,2);
    mainLayout->addStretch(1);


    this->setVisible(false);//在引导页应该是看不到的
    widget->lower();
    widget->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
    this->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    //调用事件监听器
    parent->installEventFilter(this);
}

OpenSourceIntro::~OpenSourceIntro()
{
    delete ui;
}

bool OpenSourceIntro::eventFilter(QObject *watched,QEvent *event){

    if(watched == parent()&&event->type() == QEvent::Resize){

        ui->setupUi(this);
        this->setVisible(false);//在引导页应该是看不到的
        widget->lower();
        widget->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);


        emit exit();
    }

    return QWidget::eventFilter(watched,event);
}


void OpenSourceIntro::setStyle(QString witchStyle){
    if(witchStyle=="blackWhite"){
        widget->setStyleSheet(
            "background-color:#fcfcfc;"
            "color: black;"
            );
        pushButton->setStyleSheet(
            "background-color:#fcfcfc;"
            "color: black;"
            );
        this->setStyleSheet(
            "background-color:#fcfcfc;"
            "color: black;"
            );
        label->setStyleSheet(
            "background-color:#fcfcfc;"
            "color: black;"
            );

    }
    else if(witchStyle=="pinkBlue"){
        widget->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );
        pushButton->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );
        this->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );
        label->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );

    }

}

void OpenSourceIntro::uiFresh(bool isEnglish){
    if(isEnglish){
        label->setText(tr("       This Software is Free\n       Mading by Shyi\n       Thanks for your using！"));
        pushButton->setText(tr("OK"));
    }
    else{
    label->setText(tr("* This software uses:\n* - SDL3 (zlib Licenses\n - VLC (GPLv2/LGPLv2.1) - Qt (LGPLv3) - JSON for Modern C++ (MIT License)\n * See LICENSE file for details.\n"
                          "欢迎使用GoldinkGalgameMaker0.0.1测试版本！\n本版本尚未完善，使用时有以下注意事项：\n"
                          "1.大小限制为2560*1440\n2.op/ed务必至少一个视频一张图片\n3.提前准备一张.ico用于渲染图标\n4.得到的游戏程序可能存在偶尔闪退，属于正常情况\n5.其它问题关注B站up主“翔佑”喵！"));
    pushButton->setText(tr("我知道了"));
    }
}
