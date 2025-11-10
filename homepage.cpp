#include "homepage.h"
#include "ui_homepage.h"
#include "fontmanager.h"
#include "fontselectorwidget.h"
#include "package.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::HomePage)
    ,lp(new LandingPage(this))
    ,translator(new QTranslator(this))
{
    ui->setupUi(this);
    setMinimumSize(1000,450);

    Catheing::instance();

    setStyleAll(jc.getInJson(jc.getProjectRootPath()+"/UserSetting.json","style"),true);
    this->setWindowTitle("GoldinkGalgameEngine");
    this->setWindowIcon(QIcon(":/new/prefix1/favicon.ico"));
    this->setWhatsThis("GoldinkGalgameEngine");
    //
    QTranslator translator;
    if (!translator.load(":/new/prefix1/translation_zh.qm")) {
        qDebug() << "Load failed. Error:";
    }

    lp->raise();//引导页最上层展示
    lp->show();
    connect(lp,&LandingPage::openProjectSiganl,this,&HomePage::openProject);
    connect(lp,&LandingPage::exExit,[this](){
        lp->close();
        this->close();
    });//退出程序
    connect(lp,&LandingPage::newFinish,this,&HomePage::openProject);

    connect(ui->openProjectAction,&QAction::triggered,[this](){
        QString showPath = QFileDialog::getExistingDirectory(this, "选择文件路径", "/home");
        openProject(showPath);
    });
    connect(ui->saveProjectAction,&QAction::triggered,this,&HomePage::save);
    connect(ui->importItemAction,&QAction::triggered,[this](){
        QWidget *currentTab = tabs->currentWidget();//获取当前的页面
        QString where;
        if (auto *myWidget = qobject_cast<Tabs*>(currentTab)) {
            where = myWidget->getSmlPath()+"/source";
        }
        QString filePath = QFileDialog::getOpenFileName(this, "选择一个文件",where, "*.jpg *.png *.mov *.mp3 *.mp4 *.m4a *.ico");
        importItem(filePath);
    });
    connect(ui->newProjectAction,&QAction::triggered,[this](){
        NewProject *np = new NewProject(this,false);
        np->show();
        connect(np,&NewProject::newFinish,[this,np](const QString& road){
            openProject(road);
            np->close();
        });
    });

    connect(ui->blackWhite,&QAction::triggered,[this](){
        setStyleAll("blackWhite",false);
    });
    connect(ui->pinkBlue,&QAction::triggered,[this](){
        setStyleAll("pinkBlue",false);
    });

    //设置语言
    connect(ui->chineseAction,&QAction::triggered,[this](){
        languageChange(false);
        for (int i = 0; i < tabs->count(); ++i) {
            if (auto *tab = qobject_cast<Tabs*>(tabs->widget(i))) {
                tab->uiFresh(false);  // 调用每个标签页的刷新方法
            }
        }
    });
    connect(ui->englishAction,&QAction::triggered,[this](){
        languageChange(true);
        for (int i = 0; i < tabs->count(); ++i) {
            if (auto *tab = qobject_cast<Tabs*>(tabs->widget(i))) {
                tab->uiFresh(true);  // 调用每个标签页的刷新方法
            }
        }
    });
    connect(ui->loadFont, &QAction::triggered, [this](){
        QString filePath = QFileDialog::getOpenFileName(this, "选择字体文件", "/home", "字体文件 (*.ttf *.otf *.ttc)");
        if(!filePath.isEmpty() && filePath != "/home"){
            QFileInfo fileInfo(filePath);
            QString fontName = fileInfo.baseName(); // 获取不带扩展名的文件名
            FontManager::addFont(filePath, fontName,true);
            QMessageBox::information(this, "成功", QString("字体 '%1' 加载成功").arg(fontName));
        }
    });

    connect(ui->changeFont,&QAction::triggered,[this](){
        FontSelectorWidget *fontSelector = new FontSelectorWidget(this);

        // 可以设置为对话框
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("选择字体");
        dialog->setMinimumSize(400, 500);
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        layout->addWidget(fontSelector);
        dialog->exec();
    });

    // 在MainWindow构造函数中
    QWidget *centralWidget = new QWidget(this);  // 创建中央部件
    this->setCentralWidget(centralWidget);      // 设置为中央部件
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);  // 中央部件的布局

    tabs = new QTabWidget(centralWidget);
    tabs->setTabsClosable(true);
    connect(tabs,&QTabWidget::tabCloseRequested,this,&HomePage::tabClosSlot);

    mainLayout->addWidget(tabs);  // 将TabWidget添加到布局

    projects.resize(10);//projects用于储存已经有的项目，所以不会开启已经开启的项目

    //加载语言包
    languageChange(jc.getInJson(jc.getProjectRootPath()+"/UserSetting.json","Language")=="true");

    ui->importItemAction->setEnabled(false);
    ui->saveProjectAction->setEnabled(false);
    ui->multiProjectAction->setEnabled(false);

    connect(this,&HomePage::tabsNum,[this](int num){
        if(num == 0){
            ui->importItemAction->setEnabled(false);
            ui->saveProjectAction->setEnabled(false);
            ui->multiProjectAction->setEnabled(false);
        }
        else{
            ui->importItemAction->setEnabled(true);
            ui->saveProjectAction->setEnabled(true);
            ui->multiProjectAction->setEnabled(true);
        }
    });

    connect(ui->multiProjectAction,&QAction::triggered,[this](){
        QWidget *currentTab = tabs->currentWidget();//获取当前的页面
        if (auto *myWidget = qobject_cast<Tabs*>(currentTab)) {
            package pak;
            QString savedPath = QFileDialog::getExistingDirectory(this, "选择文件路径", "/home");
            qDebug()<<myWidget->getSmlPath()<<"ha?";
            pak.packageIt(myWidget->getSmlPath(),savedPath);
        }
    });
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setStyleAll(QString witchStyle,bool isStart){
    if(witchStyle=="pinkBlue"&&isStart){

        this->setStyleSheet("QMainWindow{"
                            "background-color:rgb(255, 239, 238);"
                            "color: rgb(83, 83, 255);"
                            "}"
                            "QMenu{"
                            "color:rgb(87, 177, 255);"
                            "}"
                           );
        centralWidget()->setStyleSheet("background-color: rgb(255, 230, 229);");
        return;
    }
    else if(witchStyle=="pinkBlue"){
        if(witchStyle==jc.getInJson(jc.getProjectRootPath()+"/UserSetting.json","style")&&!isStart){
            //如果没有发生任何改变，那么直接return
            return;
        }
        else{
            Catheing::start();
            this->setStyleSheet("QMainWindow{"
                                "background-color:rgb(255, 239, 238);"
                                "color: rgb(83, 83, 255);"
                                "}"
                                "QMenu{"
                                "color:rgb(83, 83, 255);"
                                "}"
                                );
            Catheing::step1();
            lp->setStyle("pinkBlue");
            Catheing::step2();
            emit changeStyle(witchStyle);
            style=witchStyle;
            Catheing::step3();
            centralWidget()->setStyleSheet("background-color: rgb(255, 230, 229);");
            Catheing::step4();
            jc.userSettingControl("style","pinkBlue");
            Catheing::finish();
        }
    }
    else if(witchStyle=="blackWhite"){
        if(witchStyle==jc.getInJson(jc.getProjectRootPath()+"/UserSetting.json","style")&&!isStart){
            //如果没有发生任何改变，那么直接return
            return;
        }
        else{

            Catheing::start();

            this->setStyleSheet("QMainWindow{"
                                "background-color:white;"
                                "color: black;"
                                "}"
                                "QMenu{"
                                "color:black;"
                                "}"
                                );
            Catheing::step1();
            lp->setStyle("blackWhite");
            Catheing::step2();
            style=witchStyle;
            emit changeStyle(witchStyle);
            Catheing::step3();
            centralWidget()->setStyleSheet("background-color: white;");
            Catheing::step4();
            jc.userSettingControl("style","blackWhite");
            Catheing::finish();
        }
    }

}

void HomePage::languageChange(bool isEnglish){


    if(isEnglish){
        if (translator->load(":/new/prefix1/translation_zh.qm","zh_CN")) {
            qApp->installTranslator(translator);
            jc.userSettingControl("Language","true");
        }
        else{
            QMessageBox::warning(this,"warning","Language loading error.");
        }
    }
    else{
        qApp->removeTranslator(translator);
        jc.userSettingControl("Language","nottrue");
    }


    ui->retranslateUi(this);
    lp->uiFresh(isEnglish);

    for (int i = 0; i < tabs->count(); ++i) {
        if (auto *tab = qobject_cast<Tabs*>(tabs->widget(i))) {
            tab->uiFresh(isEnglish);  // 调用每个标签页的刷新方法
        }
    }

}

void HomePage::openProject(QString filePath){

    txtSearch ts(filePath+"/rootFile.txt");
    if(filePath.isEmpty()){
        return;
    }//提供用户取消时的回复

    QDir dir(filePath);
    if(!dir.exists()){
        QMessageBox::warning(this,"错误",tr("路径错误！"));
        return;
    }
    if(!ts.textCheck()){
        QMessageBox::warning(this,"错误",tr("此项目并非GalgameEngine项目！"));
        return;
    }
    QString tabName=dir.dirName();
    if(projects.contains(tabName)){
        return;
    }
    else{
        if(!projects.contains("")){
            QMessageBox::warning(this,"警告",tr("开启的项目太多，请关闭试试吧！"));
            return;
        }
        projects.append(tabName);
    }
    Tabs *tab = new Tabs(this,filePath);
    qDebug()<<"test what filePath is?"<<filePath;
    connect(tab,&Tabs::sendMessage,[this](QString filePath,QString whatDo,QString whatBeDone,QVariant data){
        saves onesave;
        onesave.filePath = filePath;
        onesave.doWhat = whatDo;
        onesave.whatBeDone = whatBeDone;
        onesave.data = data;

        m_saves.append(onesave);
    });
    tab->setStyle(style);
    tab->setSmlPath(filePath);
    connect(this,&HomePage::changeStyle,tab,&Tabs::setStyle);//通过接受信号实时改风格
    tabs->addTab(tab,tabName);
    emit tabsNum(tabs->count());
    lp->close();
    jc.projectSaveControl(filePath);//对快捷存档进行修改
    tab->load();//这里的load要做两种，第一种是全数加载，也就是第一次加载，然后要做一次导入只添加一次的函数
    //这里添加的是openPROJECT的逻辑，openproject用于从文件地址获取相应的文件夹路径然后对其处理并返回合适的QWidget，数据格式等
    //tabs->addTab(,tabName);
}

void HomePage::tabClosSlot(int i){
    //这里要做询问保存的功能
    QString tabname = tabs->tabText(i);
    int index = projects.indexOf(tabname);
    if(index != -1) {
        projects.removeAt(index);
    }
    tabs->removeTab(i);
    emit tabsNum(tabs->count());
}

void HomePage::save(){
    QWidget *currentTab = tabs->currentWidget();//获取当前的页面
    if (auto *myWidget = qobject_cast<Tabs*>(currentTab)) {
        myWidget->save();
    }

    //保存功能
    for (const saves &value : m_saves) {
        jc.mixControl(value.filePath,value.doWhat,value.whatBeDone,value.data);
        qDebug()<<"final"<<value.filePath<<value.doWhat<<value.whatBeDone<<value.data;
    }
    m_saves.clear();
}

void HomePage::importItem(const QString &filePath){
    if(filePath.isEmpty()){
        return;
    }
    else{
        QWidget *currentTab = tabs->currentWidget();//获取当前的页面
        if (auto *myWidget = qobject_cast<Tabs*>(currentTab)) {
            myWidget->import(filePath);
        }
    }
}
