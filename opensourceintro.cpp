#include "opensourceintro.h"
#include "ui_opensourceintro.h"

OpenSourceIntro::OpenSourceIntro(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OpenSourceIntro)
    ,label(new QLabel(this))
    ,pushButton(new QPushButton(this))
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout=new QVBoxLayout(this);//主布局
    QHBoxLayout *buttonLayout=new QHBoxLayout();//按钮布局

    buttonLayout->addStretch(3);
    pushButton->setText("我知道了");
    connect(pushButton,&QPushButton::clicked,this,&OpenSourceIntro::exit);
    buttonLayout->addWidget(pushButton);
    buttonLayout->addStretch(1);

    QFont font;
    font.setPointSize(15);

    label->setFont(font);
    label->setText("       本软件完全开源\n       由翔佑独立制作\n       感谢您的使用和回馈！");

    mainLayout->addWidget(label,5);
    mainLayout->addLayout(buttonLayout,2);
    mainLayout->addStretch(1);


    this->setVisible(false);//在引导页应该是看不到的
    ui->widget->lower();
    ui->widget->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
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
        ui->widget->lower();
        ui->widget->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);


        emit exit();
    }

    return QWidget::eventFilter(watched,event);
}
