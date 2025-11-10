#include "workbench.h"
#include "pageindex.h"
#include <QTimer>

WorkBench::WorkBench(QWidget *parent,QString filePath)
    : QWidget{parent}
    , m_filePath(filePath)
    , mainWidget(new QWidget(this))
    , qsw(new QStackedWidget(this))
    , pageLoading(new PageLoading(this,m_filePath))
    , pageIndex(new PageIndex(this,m_filePath))
    , pageGame(new PageMain(this,m_filePath))
    , pageSave(new QWidget(this))
    , pageSetting(new QWidget(this))
    , loadingPage(new QPushButton(this))
    , indexPage(new QPushButton(this))
    , gamePage(new QPushButton(this))
    , savePage(new QPushButton(this))
    , settingPage(new QPushButton(this))
    , prefer(new QPushButton(this))
    , next(new QPushButton(this))
    , yesorno(new QLabel(this))
    , layoutWidget(new ScreenWidget(mainWidget))
{
    qsw->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *labelLayout = new QVBoxLayout();//首先对QLaybel进行管理，QLabel是用于表示是否可以运行的，当转为绿色时可以，红色时不行
    labelLayout->addStretch(9);
    labelLayout->addWidget(yesorno,1);
    yesorno->setStyleSheet("background-color:green;");

    qsw->addWidget(pageLoading);
    //更新屏幕上现有的内容
    connect(pageLoading,&PageLoading::updateMeida,[this](QList<QString> list){
        yesorno->setStyleSheet("background-color:red;");
        QList<filePaths> lists;
        for(const QString &objectName:list){
            qDebug()<<objectName;
            if(jc.getInJson(m_filePath+"/jsons/oped.json",objectName).isEmpty()){
                continue;
            }
            filePaths a;
            a.objectName=objectName;
            a.filePath=jc.getInJson(m_filePath+"/jsons/oped.json",objectName);
            lists.append(a);
        }
        layoutWidget->loadMedias(lists);
        yesorno->setStyleSheet("background-color:green;");

    });
    //保存本页面有什么
    connect(pageLoading,&PageLoading::updateData,[this](QString filePath,QString whatDo,QString whatBeDone,QVariant data){
        yesorno->setStyleSheet("background-color:red;");
        emit sendMessage(filePath,whatDo,whatBeDone,data);
        qDebug()<<"later"<<data;

        yesorno->setStyleSheet("background-color:green;");

    });
    connect(pageLoading,&PageLoading::updataScreen,[this](QString objectName,QString filePath){
        filePaths file;
        file.filePath = filePath;
        file.objectName = objectName;
        layoutWidget->updateMeidas(file);

    });
    //pageIndex->setStyleSheet("background-color:black;");
    qsw->addWidget(pageIndex);
    connect(pageIndex,&PageIndex::updateMeida,[this](QList<QString> list){
        yesorno->setStyleSheet("background-color:red;");
        filePaths a;
        a.isView = true;

        for(const QString &objectName:list){
        if(jc.getInJson(m_filePath+"/jsons/index.json",objectName).isEmpty()){
            continue;
        }
        a.viewPoint.whatHave.append(objectName);
        pointMessages b;
        b.whatisthis = objectName;
        b.filePath = jc.getInJson(m_filePath+"/jsons/index.json",objectName);
        b.x = jc.getInJson(m_filePath+"/jsons/index.json",objectName+"X").toFloat();
        b.y = jc.getInJson(m_filePath+"/jsons/index.json",objectName+"Y").toFloat();
        //涉及到xy的储存方式，稍后进行设计
        a.viewPoint.messages.append(b);

        }
        QList<filePaths> newNow;
        newNow.append(a);
        layoutWidget->loadMedias(newNow);
        yesorno->setStyleSheet("background-color:green;");

    });

    connect(pageIndex,&PageIndex::updateData,[this](QString filePath,QString whatDo,QString whatBeDone,QVariant data){
        yesorno->setStyleSheet("background-color:red;");
        emit sendMessage(filePath,whatDo,whatBeDone,data);

        yesorno->setStyleSheet("background-color:green;");

    });

    connect(pageIndex,&PageIndex::updataScreen,[this](QString objectName,QString filePath){
        filePaths file;
        file.filePath = filePath;
        file.objectName = objectName;
        layoutWidget->updateScene(objectName,filePath);

    });
    qsw->addWidget(pageGame);
    connect(pageGame,&PageMain::updatePoint,layoutWidget,&ScreenWidget::loadContents);
    connect(pageGame,&PageMain::updataScreen,layoutWidget,&ScreenWidget::loadContents);
    connect(layoutWidget,&ScreenWidget::updateGameMain,pageGame,&PageMain::changeXY);
    connect(layoutWidget,&ScreenWidget::toPageMain,pageGame,&PageMain::updateCharacterAnimation);

    qsw->addWidget(pageSave);
    qsw->addWidget(pageSetting);

    QVBoxLayout *buttonsLayout = new QVBoxLayout();//对按钮进行管理
    loadingPage->setText(tr("首尾管理"));
    connect(loadingPage,&QPushButton::clicked,[this](){
        qsw->setCurrentWidget(pageLoading);
        layoutWidget->loadMedias(pageLoading->getMFilePaths());
    });
    indexPage->setText(tr("主页管理"));
    connect(indexPage,&QPushButton::clicked,[this](){
        qsw->setCurrentIndex(1);
        layoutWidget->loadMedias(pageIndex->getMFilePaths());
    });
    gamePage->setText(tr("游戏主体"));
    connect(gamePage,&QPushButton::clicked,[this](){
        qsw->setCurrentIndex(2);
    });
    savePage->setText(tr("保存界面"));
    connect(savePage,&QPushButton::clicked,[this](){
        qsw->setCurrentIndex(3);
    });
    settingPage->setText(tr("设置界面"));
    connect(settingPage,&QPushButton::clicked,[this](){
        qsw->setCurrentIndex(4);
    });
    buttonsLayout->addWidget(loadingPage);
    buttonsLayout->addWidget(indexPage);
    buttonsLayout->addWidget(gamePage);
    buttonsLayout->addWidget(savePage);
    buttonsLayout->addWidget(settingPage);

    QHBoxLayout *secondLayout = new QHBoxLayout;//复结构
    secondLayout -> addLayout(buttonsLayout,4);
    secondLayout -> addWidget(qsw,27);
    secondLayout -> addLayout(labelLayout,1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainWidget,4);
    mainLayout->addLayout(secondLayout,1);

    layoutWidget->loadMedias(pageLoading->getMFilePaths());
    for(const filePaths &path:pageLoading->getMFilePaths()){
        qDebug()<<"have done"<<path.filePath<<path.objectName;
    }

    prefer->setText("<<");
    next->setText(">>");

    connect(prefer,&QPushButton::clicked,layoutWidget,&ScreenWidget::prefer);
    connect(next,&QPushButton::clicked,layoutWidget,&ScreenWidget::next);
    connect(layoutWidget,&ScreenWidget::updateData,[this](QString filePath,QString whatDo,QString whatBeDone,QVariant data){
        yesorno->setStyleSheet("background-color:red;");
        emit sendMessage(m_filePath+"/jsons/index.json",whatDo,whatBeDone,data);
        yesorno->setStyleSheet("background-color:green;");
    });

    mainWidget->installEventFilter(this);
    loadLayoutWidget();

    this->installEventFilter(this);
    setStyle("pinkBlue");//默认粉蓝风格


    qsw->raise();  // 确保在最上层

}

void WorkBench::loadLayoutWidget() {
    if (mainWidget->width() > mainWidget->height()) {
        layoutWidget->resize(mainWidget->height() / 9 * 16, mainWidget->height());
    } else {
        layoutWidget->resize(mainWidget->width(), mainWidget->width() / 16 * 9);
    }
    prefer->move(prefer->width()/4,(mainWidget->height() - prefer->height()) / 2);
    next->move(mainWidget->width() - next->width(),(mainWidget->height() - next->height()) / 2);
    layoutWidget->move((mainWidget->width() - layoutWidget->width()) / 2, (mainWidget->height() - layoutWidget->height()) / 2);

}

bool WorkBench::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::Resize) {
        loadLayoutWidget();
    }
    return QWidget::eventFilter(watched, event);
}

void WorkBench::setStyle(QString witchStyle){
    if(witchStyle=="blackWhite"){
        this->setStyleSheet(
            "background-color:#fcfcfc;"
            "color:black;"
            );
    }
    else if(witchStyle=="pinkBlue"){
        this->setStyleSheet(
            "background-color: #f3feff;"
            "color: rgb(83, 83, 255);"
            );
    }

}

void WorkBench::uiFresh(bool isEnglish){
    if(isEnglish){
        loadingPage->setText(tr("Op/Ed"));
        indexPage->setText(tr("Home"));
        gamePage->setText(tr("Main"));
        savePage->setText(tr("Save"));
        settingPage->setText(tr("Setting"));
    }
    else{
        loadingPage->setText(tr("首尾管理"));
        indexPage->setText(tr("主页管理"));
        gamePage->setText(tr("游戏主体"));
        savePage->setText(tr("保存界面"));
        settingPage->setText(tr("设置界面"));
    }

}

void WorkBench::setFilePath(const QString &filePath){
    m_filePath = filePath;
}

void WorkBench::save(){
    pageGame -> onSaveTriggered();
}
