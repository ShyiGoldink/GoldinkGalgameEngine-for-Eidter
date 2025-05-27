#include "newproject.h"
#include "ui_newproject.h"


NewProject::NewProject(QWidget *parent,bool isDefault)
    : QWidget(parent)
    , ui(new Ui::NewProject)
    ,name1(new QLabel(this))
    ,name2(new QLabel(this))
    ,projectName(new QLineEdit(this))
    ,projectPath(new QLineEdit(this))
    ,browse(new QPushButton(this))
    ,ensure(new QPushButton(this))
    ,cancel(new QPushButton(this))
    ,m_isDefault(isDefault)
    ,catheing(new Catheing(nullptr))


{
    ui->setupUi(this);

    //这是主体布局
    QVBoxLayout *mainLayout =new QVBoxLayout(this);

    QFont font;
    font.setPointSize(15);

    //先处理labels
    QVBoxLayout *labelsLayout=new QVBoxLayout();
    name1->setFont(font);
    name1->setText("项目名称:");
    name1->setStyleSheet("color: #00BFFF;border: 2px;");
    name2->setFont(font);
    name2->setText("项目位置:");
    name2->setStyleSheet("color: #00BFFF;");
    labelsLayout->addWidget(name1);
    labelsLayout->addWidget(name2);

    //处理textEdits
    QVBoxLayout *textEditsLayouot=new QVBoxLayout;
    projectName->setStyleSheet("background-color:#f3ffff;color:#464fff;");
    projectPath->setStyleSheet("background-color:#f3ffff;color:#464fff;");
    textEditsLayouot->addWidget(projectName,1);
    textEditsLayouot->addWidget(projectPath,1);

    //处理浏览键
    QVBoxLayout *browseLayout=new QVBoxLayout();
    browse->setText("浏览");
    browseLayout->addStretch(20);
    browseLayout->addWidget(browse);
    browseLayout->addStretch(6);
    connect(browse,&QPushButton::clicked,this,&NewProject::browseSlot);

    //处理buttons
    QHBoxLayout *buttonsLaout=new QHBoxLayout();
    ensure->setText("确定");
    connect(ensure,&QPushButton::clicked,this,&NewProject::ensureSlot);
    cancel->setText("取消");
    connect(cancel,&QPushButton::clicked,this,[this](){
        emit exit();
        if(!m_isDefault){
            this->close();}
    });
    buttonsLaout->addStretch(2);
    buttonsLaout->addWidget(ensure,3);
    buttonsLaout->addWidget(cancel,3);
    buttonsLaout->addStretch(2);

    //处理副结构
    QHBoxLayout *viceLayout=new QHBoxLayout;
    viceLayout->addStretch(1);
    viceLayout->addLayout(labelsLayout,2);
    viceLayout->addLayout(textEditsLayouot,6);
    viceLayout->addLayout(browseLayout,1);

    mainLayout->addStretch(2);
    mainLayout->addLayout(viceLayout,2);
    mainLayout->addStretch(2);
    mainLayout->addLayout(buttonsLaout,2);
    mainLayout->addStretch(2);

    loadLanding();

    //调用事件监听器
    parent->installEventFilter(this);
}

NewProject::~NewProject()
{
    delete ui;
}


void NewProject::loadLanding(){
    if(m_isDefault){//如果是在引导页，那么就要调用，让本页面的大小为引导页的大小的一定比例
        this->setVisible(false);//在引导页应该是看不到的
        ui->widget->lower();
        ui->widget->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setFixedSize(parentWidget()->width()*0.7,parentWidget()->rect().height());
        this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    }
    else{
        ui->widget->lower();
        ui->widget->setFixedSize(this->size());
        return ;
    }
}

bool NewProject::eventFilter(QObject *watched,QEvent *event){

    if(watched == parent()&&event->type() == QEvent::Resize){
        loadLanding();
        emit exit();
    }

    return QWidget::eventFilter(watched,event);
}

void NewProject::browseSlot(){
    QString showPath = QFileDialog::getExistingDirectory(this, "选择文件路径", "/home");
    projectPath->setText(showPath);
}

void NewProject::ensureSlot(){
    QString a = projectName->text();
    QString b = projectPath->text();

    qDebug()<<a<<"/n"<<b;

    if(a.isEmpty()){
        QMessageBox::warning(this,"warning","请输入项目名称");
        return;
    }

    if(b.isEmpty()){
        QMessageBox::warning(this,"warning","请选择项目路径");
        return;
    }

    catheing->show();
    catheing->raise();
    catheing->start();

    QDir dir;
    dir.mkdir(b+"/"+a);
    QFile file(b+"/"+a+"/rootFile.txt");
    qDebug()<<b+"/"+a;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,"warning","根目录文件创建失败");
        catheing->close();
    }
    QTextStream out(&file);
    out << "#GoldinkGalgameEngine发行\n"+a; // 写入内容
    file.close();
    catheing->step1();


    dir.mkdir(b+"/"+a+"/source");
    catheing->step2();

    dir.mkdir(b+"/"+a+"/codes");
    catheing->step3();

    dir.mkdir(b+"/"+a+"/jsons");
    //后续可能还有要增加的
    catheing->step4();

    js.projectSaveControl(b+"/"+a);
    catheing->finish();

}
