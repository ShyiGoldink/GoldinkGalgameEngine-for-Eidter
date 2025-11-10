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
    ,ensureLive2D(new QPushButton(this))
    ,cancel(new QPushButton(this))
    ,m_isDefault(isDefault)


{
    ui->setupUi(this);

    //这是主体布局
    QVBoxLayout *mainLayout =new QVBoxLayout(this);

    QFont font;
    font.setPointSize(15);

    //先处理labels
    QVBoxLayout *labelsLayout=new QVBoxLayout();
    name1->setFont(font);
    name1->setText(tr("项目名称:"));
    name1->setStyleSheet("color: #00BFFF;border: 2px;");
    name2->setFont(font);
    name2->setText(tr("项目位置:"));
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
    browse->setText(tr("浏览"));
    browseLayout->addStretch(20);
    browseLayout->addWidget(browse);
    browseLayout->addStretch(6);
    connect(browse,&QPushButton::clicked,this,&NewProject::browseSlot);

    //处理buttons
    QHBoxLayout *buttonsLaout=new QHBoxLayout();
    ensure->setText(tr("确定"));
    connect(ensure,&QPushButton::clicked,this,&NewProject::ensureSlot);
    ensureLive2D->setText(tr("确定（live2d模式）"));
    //设置live2D的相关内容
    cancel->setText(tr("取消"));
    connect(cancel,&QPushButton::clicked,this,[this](){
        emit exit();
        if(!m_isDefault){
            this->close();}
    });
    buttonsLaout->addStretch(2);
    buttonsLaout->addWidget(ensure,2);
    buttonsLaout->addWidget(ensureLive2D,2);
    buttonsLaout->addWidget(cancel,2);
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

    if(a.isEmpty()){
        QMessageBox::warning(this,"warning",tr("请输入项目名称"));
        return;
    }

    if(b.isEmpty()){
        QMessageBox::warning(this,"warning",tr("请选择项目路径"));
        return;
    }

    Catheing::start();

    QDir dir;
    dir.mkdir(b+"/"+a);
    QFile file(b+"/"+a+"/rootFile.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,"warning",tr("根目录文件创建失败"));
        Catheing::finish();
        return;
    }
    QTextStream out(&file);
    out << "#GoldinkGalgameEngine\n"+a; // 写入内容
    file.close();
    Catheing::step1();


    dir.mkdir(b+"/"+a+"/source");
    Catheing::step2();

    dir.mkdir(b+"/"+a+"/codes");
    Catheing::step3();

    dir.mkdir(b+"/"+a+"/jsons");
    QFile opedJson(b+"/"+a+"/jsons/oped.json");
    if(!opedJson.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"warning",tr("项目储存文件创建失败"));
        Catheing::finish();
        return;
    };
    opedJson.close();
    QFile indexJson(b+"/"+a+"/jsons/index.json");
    if(!indexJson.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"warning",tr("项目储存文件创建失败"));
        Catheing::finish();
        return;
    };
    indexJson.close();
    QDir dir2;
    // 创建目录
    dir2.mkdir(b+"/"+a+"/jsons/game");

    // 直接使用 addFilePathsToMainJson 创建文件并写入内容
    filePaths newFP;
    newFP.isView = true;
    newFP.filePath = "";
    newFP.objectName = "MainScene";
    newFP.viewPoint.id = "MainG0";
    newFP.viewPoint.childId = {};
    newFP.viewPoint.linkNum = 0;
    newFP.viewPoint.whatHave = QStringList{};
    newFP.viewPoint.messages = QList<pointMessages>{};
    newFP.viewPoint.selects = QStringList{};
    newFP.viewPoint.Cg = "";
    newFP.viewPoint.isChoices = false;

    QString mainJsonPath = b+"/"+a+"/jsons/game/main.json";
    bool success = js.addFilePathsToMainJson(mainJsonPath, newFP);

    if (!success) {
        QMessageBox::warning(this,"warning",tr("main.json 文件创建失败"));
        Catheing::finish();
        return;
    }

    QFile saveJson(b+"/"+a+"/jsons/save.json");
    if(!saveJson.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"warning",tr("项目储存文件创建失败"));
       Catheing::finish();
        return;
    };
    saveJson.close();
    QFile setJson(b+"/"+a+"/jsons/set.json");
    if(!setJson.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,"warning",tr("项目储存文件创建失败"));
       Catheing::finish();
        return;
    };
    setJson.close();
    //后续可能还有要增加的
    Catheing::step4();

    js.projectSaveControl(b+"/"+a);
    emit newFinish(b+"/"+a);
    Catheing::finish();

}
void NewProject::setStyle(QString witchStyle){
    if(witchStyle=="blackWhite"){
        this->setStyleSheet(
            "background-color:#fcfcfc;"
            "color:black;"
            );
    }
    else if(witchStyle=="pinkBlue"){
        this->setStyleSheet(
            "background-color: rgb(255, 230, 229);"
            "color: rgb(83, 83, 255);"
            );
    }

}

void NewProject::uiFresh(bool isEnglish){
    if(isEnglish){
        name1->setText("Project Name");
        name2->setText("Project Address");
        ensure->setText("Ensure");
        ensureLive2D->setText("Ensure(Live2D!)");
        cancel->setText("Cancel");
        browse->setText("Browse");
    }
    else{
        name1->setText("项目名称");
        name2->setText("项目地址");
        ensure->setText("确定");
        ensureLive2D->setText("确定（live2D模式！）");
        cancel->setText("取消");
        browse->setText("浏览");
    }
}
