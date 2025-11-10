#include "pagemain.h"
#include "qforeach.h"
#include "changechoicewidget.h"
#include "enteremitlineedit.h"

PageMain::PageMain(QWidget *parent ,QString loadjsonPath ):
    QWidget(parent)

    ,m_loadjsonPath(loadjsonPath)
    ,m_group(new QParallelAnimationGroup(this))
    ,mainLayout(new QHBoxLayout(this))
    ,labelsLayout(new QHBoxLayout(this))
    ,sceneLayout(new QHBoxLayout(this))
    ,m_scene(new QGraphicsScene(this))
    ,m_view(new QGraphicsView(m_scene,this))
    ,havePixampCItem(false)
    ,m_menu(new QMenu(this))
    ,addPoint(new QAction())
    ,deletePoint(new QAction())
    ,addSelects(new QAction())
    ,changeChoice(new QAction())
    ,viewPoint(new QAction())
    ,deleteView(new QAction())
    ,addAnimation(new QAction())
    ,m_animationMenu(new QMenu(this))
    ,fadeInAction(new QAction())
    ,fadeOutAction(new QAction())
    ,clearAnimation(new QAction())
    ,m_animationCharacter(new QMenu(this))
    ,in(new QAction())
    ,out(new QAction())
    ,m_animationCharacterIn(new QMenu(this))
    ,m_animationCharacterOut(new QMenu(this))
    ,leftIn(new QAction())
    ,rightIn(new QAction())
    ,noIn(new QAction())
    ,leftOut(new QAction())
    ,rightOut(new QAction())
    ,noOut(new QAction())
    ,m_setCging(false)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setLayout(mainLayout);
    sceneLayout->addWidget(m_view);
    mainLayout->addLayout(labelsLayout,4);
    mainLayout->addLayout(sceneLayout,6);


    QPixmap map(":/new/prefix1/Point.png");
    m_pixmap = map.scaled(20,20);

    QDir dir(m_loadjsonPath+"/jsons/game");
    dir.setNameFilters(QStringList()<<"*.json"<<"*.JSON");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList fileList = dir.entryInfoList();

    foreach (const QFileInfo &fileInfo, fileList) {
        QString jsonPaths = fileInfo.absoluteFilePath();
        m_filePaths.append(jc.loadPageMain(jsonPaths));
    }

    /*foreach(const filePaths &find, m_filePaths){
        if(find.viewPoint.id == "MainG0"){
            m_filePath = find;
            break;
        }
    }

    qDebug()<<m_filePaths.isEmpty();
    qDebug()<<m_filePath.filePath;*/


    selects = new SelectBox(this
                            ,{tr("背景"),"BGM",tr("角色一"),tr("角色二"),tr("角色三")
                            ,tr("对话框"),tr("对话语音"),tr("对话内容")}
                            ,m_filePath.viewPoint.whatHave.toVector());
    selects -> move(-11.2*selects->width(),0);



    inAnimation = new QPropertyAnimation(selects,"pos");
    m_group->addAnimation(inAnimation);
    connect(selects,&SelectBox::in,[this](){
        inAnimationSlot(inAnimation,m_group,selects);
    });

    connect(selects,&SelectBox::out,[this](){
        outAnimationSlot(inAnimation,m_group);
    });


    deletePoint = m_menu->addAction(tr("删除结点"));
    connect(deletePoint,&QAction::triggered,this,&PageMain::deletePointSlot);
    addPoint = m_menu->addAction(tr("添加结点"));
    connect(addPoint,&QAction::triggered,this,&PageMain::addPointSlot);
    addSelects = m_menu->addAction(tr("添加分支"));
    connect(addSelects,&QAction::triggered,this,&PageMain::addSelectsSlot);
    changeChoice = m_menu->addAction(tr("改变分支"));
    connect( changeChoice,&QAction::triggered,this,&PageMain:: changeChoiceSlot);
    viewPoint = m_menu->addAction(tr("设为鉴赏CG"));
    connect(viewPoint,&QAction::triggered,this,&PageMain::setCg);
    deleteView = m_menu->addAction(tr("取消鉴赏CG"));
    connect(deleteView,&QAction::triggered,this,&PageMain::deleteCg);
    connect(selects,&SelectBox::outMessage,[this](QList<QString> outList){
        setUpLabels(outList);
        changeWhatHave(outList);
    });
    addAnimation = m_menu->addAction(tr("添加动画效果"));
    connect(addAnimation,&QAction::triggered,this,&PageMain::controlAnimation);

    fadeInAction = m_animationMenu->addAction(tr("黑幕入动画"));
    fadeInAction->setCheckable(true);
    fadeInAction->setChecked(false);   // 初始未选中

    fadeOutAction = m_animationMenu->addAction(tr("黑幕出动画"));
    fadeOutAction->setCheckable(true);
    fadeOutAction->setChecked(false);   // 初始未选中

    clearAnimation = m_animationMenu->addAction(tr("清空动画"));

    in = m_animationCharacter->addAction(tr("角色入场动画"));
    in->setMenu(m_animationCharacterIn);
    out = m_animationCharacter->addAction(tr("角色离场动画"));
    out->setMenu(m_animationCharacterOut);

    leftIn = m_animationCharacterIn->addAction(tr("从左平移入场"));
    connect(leftIn,&QAction::triggered,[this](){controlCharacterAnimation("leftIn");});
    rightIn = m_animationCharacterIn->addAction(tr("从右平移入场"));
    connect(rightIn,&QAction::triggered,[this](){controlCharacterAnimation("rightIn");});
    noIn = m_animationCharacterIn->addAction(tr("无入场动画"));
    connect(noIn,&QAction::triggered,[this](){controlCharacterAnimation("nonei");});

    leftOut = m_animationCharacterOut->addAction(tr("向左平移出场"));
    connect(leftOut,&QAction::triggered,[this](){controlCharacterAnimation("leftOut");});
    rightOut = m_animationCharacterOut->addAction(tr("向右平移出场"));
    connect(rightOut,&QAction::triggered,[this](){controlCharacterAnimation("rightOut");});
    noOut = m_animationCharacterOut->addAction(tr("无出场动画"));
    connect(noOut,&QAction::triggered,[this](){controlCharacterAnimation("noneo");});
    leftIn->setCheckable(true);
    rightIn->setCheckable(true);
    noIn->setCheckable(true);
    leftOut->setCheckable(true);
    rightOut->setCheckable(true);
    noOut->setCheckable(true);

    connect(fadeInAction, &QAction::triggered, [this](){ setSceneAnimation("fadeInAction"); });
    connect(fadeOutAction, &QAction::triggered, [this](){ setSceneAnimation("fadeOutAction"); });
    connect(clearAnimation, &QAction::triggered, [this](){ setSceneAnimation("clear"); });
    addAnimation->setMenu(m_animationMenu);

    mainLayout->setContentsMargins(selects->width()*2,0,0,0);

    initScene();

    setUpLabels(m_filePathsMap["MainG0"].viewPoint.whatHave);
}

void PageMain::inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget){
    //可能的防呆设计还未设计

    widget->raise();
    widget->setVisible(true);
    widget->move(-widget->width()*0.91,0);
    animation->setDuration(300);
    animation->setStartValue(QPoint(-widget->width()*0.91,0));
    animation->setEndValue(QPoint(0,0));
    group->start();
}

void PageMain::outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group){
    //可能的防呆设计还未设计


    animation->setDuration(300);
    animation->setStartValue(QPoint(0,0));
    animation->setEndValue(QPoint(-selects->width()*0.91,0));
    group->start();
}

void PageMain::initMap(){
    m_filePathsMap.clear();
    foreach(const filePaths &fp,m_filePaths){
        if(!fp.viewPoint.id.isEmpty()){
            m_filePathsMap[fp.viewPoint.id] = fp;
        }
    }
}

void PageMain::safeClearLayout(QLayout* layout)
{
    if (!layout) return;

    // 停止所有动画
    if (m_group->state() == QAbstractAnimation::Running) {
        m_group->stop();
    }

    // 递归清除布局
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QLayout* childLayout = item->layout()) {
            safeClearLayout(childLayout);
            childLayout->deleteLater();
        }
        else if (QWidget* widget = item->widget()) {
            widget->disconnect(); // 断开所有连接
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}

void PageMain::setUpLabels(QList<QString> labelNames){

    safeClearLayout(labelsLayout);


    for(int i=0;i<labelNames.count();i+=4){
        QVBoxLayout *insideLayout = new QVBoxLayout();
        for(int j = 0;j<4 && (i+j)<labelNames.count();j++){
            if(labelNames[i+j] == "对话内容"){
                EnterEmitLineEdit* eele = new EnterEmitLineEdit(this);
                connect(eele,&EnterEmitLineEdit::enterPressed,[this](QString Texts){
                    pointMessages msg = {"对话内容",208,1144,"","",Texts};
                    setMessages(msg);

                });
                insideLayout->addWidget(eele);
                continue;
            }
            ReciveLabel *label = new ReciveLabel(this,labelNames[i+j]);
            connect(label,&ReciveLabel::message,[this](QString whatTo,QString path){
                //emit updateData(m_loadjsonPath+"/jsons/oped.json","updata",whatTo, QVariant(path));
                pointMessages msg = {whatTo,0,0,"","",path};
                setMessages(msg);

            });
            insideLayout->addWidget(label);
        }
        labelsLayout->addLayout(insideLayout,2);

    }
    selects->raise();
    this->update();
}

void PageMain::setMessages(pointMessages msg){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    int i =currentFP.viewPoint.messages.count();

    bool find =false;

    for(int  j = 0;j<i;j++){
        if(currentFP.viewPoint.messages[j].whatisthis ==  msg.whatisthis){
            find = true;
            i = j;
            break;
        }
    }
    if(find){
        currentFP.viewPoint.messages[i] = msg;
    }else{
        currentFP.viewPoint.messages.append(msg);
    }

    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            if(find){
                fp.viewPoint.messages[i] = msg;
            }else{
                fp.viewPoint.messages.append(msg);
            }
            break;
        }
    }
    emit updataScreen(m_filePathsMap[m_IdNow]);

}

void PageMain::changeXY(QString objectName,float x,float y){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    int i =currentFP.viewPoint.messages.count();


    for(int  j = 0;j<i;j++){
        if(currentFP.viewPoint.messages[j].whatisthis == objectName){
            currentFP.viewPoint.messages[j].x = x;
            currentFP.viewPoint.messages[j].y = y;
            i = j;
            break;
        }
    }


    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.messages[i].x = x;
            fp.viewPoint.messages[i].y = y;
            break;
        }
    }
    //emit updataScreen(m_filePathsMap[m_IdNow]);

}

QVector<QColor> PageMain:: initColors(int n){
    QVector<QColor> colors;
    if(n<=1){
        colors.append(QColor(255,0,0));
        return colors;
    }
    for (int i = 0; i < n; ++i) {
        float hue = (i * 360.0f) / n;
        QColor color = QColor::fromHslF(hue / 360.0f, 1.0f, 0.5f);
        colors.append(color);
    }

    return colors;
}

int PageMain::initPointPrefer(const QString& nodeId){
    if (m_divide.contains(nodeId)) {
        return m_divide[nodeId];
    }

    if (m_filePathsMap[nodeId].viewPoint.linkNum == 0) {
        m_divide[nodeId] = 40;  // 缓存结果
        return 40;
    } else if (m_filePathsMap[nodeId].viewPoint.linkNum == 1) {
        int result = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[0]);
        m_divide[nodeId] = result;  // 缓存单链节点
        return result;
    } else {
        int total = 0;
        foreach (const QString& childid, m_filePathsMap[nodeId].viewPoint.childId) {
            total += initPointPrefer(childid);
        }
        m_divide[nodeId] = total;
        return total;
    }
}

void PageMain::initPointSlot(){
    m_divide.clear(); // 清空旧数据
    // 只调用一次initPointPrefer，递归计算所有分支节点的m_divide
    int rootTotal = initPointPrefer("MainG0");
    m_colors.clear();
    // 计算m_colors大小：rootTotal/40 +1（每个基础偏移量40对应一个颜色）
    m_colors = initColors(rootTotal / 40 + 1);
    initPoint("MainG0", 0, 0, 0); // 从根节点开始创建
}

void PageMain::createPointMiddle(float x,float y,const QString& nodeId){
    ClickablePixmapItem* item = new ClickablePixmapItem(m_pixmap);
    item->setPos(x, y);
    item->setData(0, nodeId);

    connect(item, &ClickablePixmapItem::mousePress, [this, x, y, item](bool isRight){
        if(isRight){
            controlAnimation();
            QPoint point = m_view->mapToGlobal(m_view->mapFromScene(x, y-30));
            m_menu->exec(point);
        }
        m_pixmapCItem->setVisible(true);
        m_pixmapCItem->setPos(x-1, y-1);
        m_IdNow = item->data(0).toString();
        qDebug() << "Selected node:" << m_IdNow;
        if(!(m_IdNow == ""))emit updatePoint(m_filePathsMap[m_IdNow]);
        setUpLabels(m_filePathsMap[m_IdNow].viewPoint.whatHave);
        if(m_filePathsMap[m_IdNow].viewPoint.linkNum <=1){
            addSelects->setEnabled(true);
            changeChoice->setEnabled(false);
        }else{
            addSelects->setEnabled(false);
            changeChoice->setEnabled(true);
        }

    });

    m_itemMap[nodeId] = item;
    m_scene->addItem(item);
}

void PageMain::initLine(float xi,float yi,float x,float y,int color){
    QGraphicsLineItem* line = new QGraphicsLineItem(xi+10,yi+10,x+10,y+10);
    QPen pen(m_colors[color]);
    pen.setWidth(2);
    pen.setCapStyle(Qt::RoundCap);
    line->setPen(pen);
    line->setZValue(-1); // 确保线条在节点下面
    m_scene->addItem(line);
}

void PageMain::initPoint(const QString& nodeId,float x,float y,int color){
    if(!m_filePathsMap.contains(nodeId)){
        qWarning() << "Node not found:" << nodeId;
        return;
    }

    int i = m_filePathsMap[nodeId].viewPoint.linkNum;

    if(i == 0){

        createPointMiddle(x,y,nodeId);

    }else if(i == 1){
        createPointMiddle(x,y,nodeId);
        initLine(x,y,x+40,y,color);
        initPoint(m_filePathsMap[nodeId].viewPoint.childId[0],x+40,y,color);
    }else if(i%2 == 0){
        createPointMiddle(x,y,nodeId);
        int j = i/2;
        QList<float> l;
        for(int k = 0;k<j;k++){
            if(l.isEmpty()){
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x,y,x+40,y-ff/2,color+k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k],x+40,y-ff/2,color+k);
                l.append(ff);
            }else{
                float m = 0.0f;
                foreach(const float& n,l){
                    m=m+n;
                }
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x,y,x+40,y-ff/2-m,color+k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k],x+40,y-ff/2-m,color+k);
                l.append(ff);
            }

        }
        QList<float> o;
        for(int k = j;k<i;k++){
            if(o.isEmpty()){
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x,y,x+40,y+ff/2,color+k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k],x+40,y+ff/2,color+k);
                o.append(ff);
            }else{
                float m = 0.0f;
                foreach(const float& n,o){
                    m=m+n;
                }
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x,y,x+40,y+ff/2+m,color+k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k],x+40,y+ff/2+m,color+k);
                o.append(ff);
            }
        }
    }

    else{
        createPointMiddle(x, y, nodeId);
        int j = (i - 1) / 2; // 上方j个，中间1个，下方j个
        QList<float> l;
        // 上方节点（0~j-1）
        for (int k = 0; k < j; k++) {
            if (l.isEmpty()) {
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x, y, x + 40, y - ff / 2-10, color + k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k], x + 40, y - ff / 2-10, color + k);
                l.append(ff);
            } else {
                float m = 0.0f;
                foreach (const float& n, l) {
                    m += n;
                }
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x, y, x + 40, y - ff / 2 - m-10, color + k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k], x + 40, y - ff / 2 - m-10, color + k);
                l.append(ff);
            }
        }


        initLine(x, y, x + 40, y, color + j);
        initPoint(m_filePathsMap[nodeId].viewPoint.childId[j], x + 40, y, color + j);


        QList<float> o;
        for (int k = j + 1; k < i; k++) { // 起始索引修复：j+1
            if (o.isEmpty()) {
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x, y, x + 40, y + ff / 2+10, color + k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k], x + 40, y + ff / 2+10, color + k);
                o.append(ff);
            } else {
                float m = 0.0f;
                foreach (const float& n, o) {
                    m += n;
                }
                float ff = initPointPrefer(m_filePathsMap[nodeId].viewPoint.childId[k]);
                initLine(x, y, x + 40, y + ff / 2 + m+10, color + k);
                initPoint(m_filePathsMap[nodeId].viewPoint.childId[k], x + 40, y + ff / 2+m+10, color + k);
                o.append(ff);
            }
        }
    }

}

void PageMain::initScene(){
    // 先清理旧的场景和视图
    if(m_scene){
        m_scene->clear();
    }

    // 清除item映射，避免悬挂指针
    m_itemMap.clear();
    m_tree.clear();

    QPixmap mapC(":/new/prefix1/PointC.png");
    m_pixmapCItem = new QGraphicsPixmapItem();
    m_pixmapCItem->setPixmap(mapC.scaled(23,23));
    m_pixmapCItem->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    m_pixmapCItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_pixmapCItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    m_scene->addItem(m_pixmapCItem);
    m_pixmapCItem->setVisible(false);

    initMap();

    // 从根节点开始递归创建场景
    if (m_filePathsMap.contains("MainG0")) {
        initPointSlot();
    } else {
        qWarning() << "Root node MainG0 not found!";
        qDebug() << "Available keys:" << m_filePathsMap.keys();
        return;
    }

    m_scene->update();
    qDebug()<<m_filePathsMap.keys();
}


filePaths PageMain::newFilePaths(const QString &lastNodeId){

    QString newId;
    do {
        newId = m_filePathsMap[lastNodeId].viewPoint.id.left(4) +
                QString::number(QRandomGenerator::global()->bounded(10000, 99999));
    } while (m_ids.contains(newId));

    filePaths newFP;
    newFP.isView = true;
    newFP.filePath = "";
    newFP.objectName = newId;
    newFP.viewPoint.id = newId;
    newFP.viewPoint.childId = {};
    newFP.viewPoint.linkNum = 0;
    newFP.viewPoint.whatHave = QStringList{};
    newFP.viewPoint.messages = QList<pointMessages>{};
    newFP.viewPoint.selects = QStringList{};
    newFP.viewPoint.Cg = "";
    newFP.viewPoint.isChoices = false;
    return newFP;
}

void PageMain::addPointSlot(){
    if (m_IdNow.isEmpty()) {
        qDebug() << "No node selected!";
        return;
    }

    if (!m_filePathsMap.contains(m_IdNow)) {
        qDebug() << "m_IdNow not found in map:" << m_IdNow;
        return;
    }

    filePaths &currentFP = m_filePathsMap[m_IdNow];

    if(currentFP.viewPoint.linkNum == 0){
        filePaths newFP = newFilePaths(m_IdNow);
        QString newId = newFP.viewPoint.id;
        // 更新当前节点的子节点指向新节点
        m_filePathsMap[m_IdNow].viewPoint.childId.append(newId);
        m_filePathsMap[m_IdNow].viewPoint.linkNum = 1;

        // 同时更新 m_filePaths 中的对应项
        for (auto &fp : m_filePaths) {
            if (fp.viewPoint.id == m_IdNow) {
                fp.viewPoint.childId.append(newId);
                fp.viewPoint.linkNum = 1;
                break;
                }
            }

        // 添加到数据结构
        m_filePathsMap[newId] = newFP;
        m_filePaths.append(newFP);
        m_ids.append(newId);

        // 保存操作
        mainSave MS = {"point", "addPoint", {newId}};
        m_saves.append(MS);
    }
    else if(currentFP.viewPoint.linkNum == 1){
        filePaths newFP = newFilePaths(m_IdNow);
        QString newId = newFP.viewPoint.id;

        newFP.viewPoint.linkNum = 1;
        newFP.viewPoint.childId.append(currentFP.viewPoint.childId[0]);

        m_filePathsMap[m_IdNow].viewPoint.childId[0]=newId;
        for (auto &fp : m_filePaths) {
            if (fp.viewPoint.id == m_IdNow) {
                fp.viewPoint.childId[0]=newId;
                break;
            }
        }

        m_filePathsMap[newId] = newFP;
        m_filePaths.append(newFP);
        m_ids.append(newId);

        mainSave MS = {"point", "addPoint", {newId}};
        m_saves.append(MS);
    }
    else{
        filePaths newFP = newFilePaths(m_IdNow);
        QString newId = newFP.viewPoint.id;
        int i = currentFP.viewPoint.linkNum;

        newFP.viewPoint.linkNum = i;
        for(int j = 0;j<i;j++){
            newFP.viewPoint.childId.append(currentFP.viewPoint.childId[j]);
        }

        m_filePathsMap[m_IdNow].viewPoint.childId.clear();
        m_filePathsMap[m_IdNow].viewPoint.childId.append(newId);
        m_filePathsMap[m_IdNow].viewPoint.linkNum = 1;
        for (auto &fp : m_filePaths) {
            if (fp.viewPoint.id == m_IdNow) {
                fp.viewPoint.childId.clear();
                fp.viewPoint.childId.append(newId);
                fp.viewPoint.linkNum = 1;
                break;
            }
        }

        m_filePathsMap[newId] = newFP;
        m_filePaths.append(newFP);
        m_ids.append(newId);

        mainSave MS = {"point", "addPoint", {newId}};
        m_saves.append(MS);

    }

    // 重新初始化场景
    initScene();
}


void PageMain::deletePointSlot(){
    if (m_IdNow.isEmpty() || m_IdNow == "MainG0") {
        return;
    }

    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }

    filePaths &currentFP = m_filePathsMap[m_IdNow];
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.childId.contains(m_IdNow)) {
            if(fp.viewPoint.linkNum ==1){
                fp.viewPoint.linkNum = currentFP.viewPoint.linkNum;
                fp.viewPoint.childId = currentFP.viewPoint.childId;
            }
            else{
                fp.viewPoint.linkNum = fp.viewPoint.linkNum+currentFP.viewPoint.linkNum-1;
                for(int i = 0;i < fp.viewPoint.childId.count();i++) {
                    if(fp.viewPoint.childId[i] == m_IdNow){
                        fp.viewPoint.childId.remove(i);
                    }
                }
                fp.viewPoint.childId.append(currentFP.viewPoint.childId);
            }

            break;
        }
    }

    mainSave MS = {"point", "deletePoint", {m_IdNow}};
    m_saves.append(MS);

    initScene();
}

void PageMain::addSelectsSlot(){
    bool ok;

    // 正确使用 QInputDialog 的静态方法
    int intValue = QInputDialog::getInt(this,
                                        "输入数字",
                                        "请输入分支数量 (2-16):",
                                        2, 2, 16, 1, &ok);

    if (ok) {
        // 直接创建分支选择窗口
        addSelectWidget *asd = new addSelectWidget(intValue, nullptr);
        connect(asd, &addSelectWidget::giveSelects, this, &PageMain::addSelectsMiddle);
        connect(asd, &addSelectWidget::giveCancle, this, [asd](){
            asd->deleteLater(); // 确保窗口被清理
        });

        // 显示窗口（根据需要选择显示方式）
        asd->show(); // 或者 asd->exec() 如果改为 QDialog
        qDebug()<<"running!";
    }
}

void PageMain::addSelectsMiddle(int i,QList<QString> selects){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }

    filePaths &currentFP = m_filePathsMap[m_IdNow];
    m_filePathsMap[m_IdNow].viewPoint.linkNum = i;
    filePaths newFP = newFilePaths(m_IdNow);
    newFP.viewPoint.id = QString("%1%2%3%4")
                             .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                             .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                             .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                             .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))+newFP.viewPoint.id.right(5);
    if(!currentFP.viewPoint.childId.isEmpty()){newFP.viewPoint.childId.append(currentFP.viewPoint.childId[0]);};
    m_filePathsMap[m_IdNow].viewPoint.childId.clear();
    m_filePathsMap[m_IdNow].viewPoint.isChoices = true;
    m_filePathsMap[m_IdNow].viewPoint.selects = selects;
    m_filePathsMap[m_IdNow].viewPoint.childId.append(newFP.viewPoint.id);
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.childId.clear();
            fp.viewPoint.linkNum = i;
            fp.viewPoint.selects = selects;
            break;
        }
    }
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.childId.append(newFP.viewPoint.id);
            break;
        }
    }

    // 添加到数据结构
    m_filePathsMap[newFP.viewPoint.id] = newFP;
    m_filePaths.append(newFP);
    m_ids.append(newFP.viewPoint.id);

    for(int j = 1;j<i;j++){
        filePaths newFP = newFilePaths(m_IdNow);
        newFP.viewPoint.id = QString("%1%2%3%4")
                                 .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                 .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                 .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                 .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))+newFP.viewPoint.id.right(5);
        m_filePathsMap[m_IdNow].viewPoint.childId.append(newFP.viewPoint.id);

        for (auto &fp : m_filePaths) {
            if (fp.viewPoint.id == m_IdNow) {
                fp.viewPoint.childId.append(newFP.viewPoint.id);
                break;
            }
        }

        m_filePathsMap[newFP.viewPoint.id] = newFP;
        m_filePaths.append(newFP);
        m_ids.append(newFP.viewPoint.id);
    }

    initScene();
    m_scene->update();
}

void PageMain::changeChoiceSlot(){
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    ChangeChoiceWidget *widget = new ChangeChoiceWidget(this,currentFP.viewPoint.selects);
    connect(widget, &ChangeChoiceWidget::outMessage, [this](int v, QList<QString> result,QList<QString> ids) {
            changeChoiceMiddle(v,result,ids);
    });
    widget->show();
}

void PageMain::changeChoiceMiddle(int i,QList<QString> selects,QList<QString> ids){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];

    currentFP.viewPoint.childId.clear();
    currentFP.viewPoint.childId.append(ids);
    if(ids.count() == i || ids.count() > i){
        if(i == 1){
            currentFP.viewPoint.isChoices = false;
        }
        currentFP.viewPoint.linkNum = i;
        currentFP.viewPoint.selects = selects;
    }else{
        currentFP.viewPoint.linkNum = i;
        currentFP.viewPoint.selects = selects;
        currentFP.viewPoint.childId = ids;
        int k = i - ids.count();
        for(int j = 0;j<k;j++){
            filePaths newFP = newFilePaths(m_IdNow);
            newFP.viewPoint.id = QString("%1%2%3%4")
                                     .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                     .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                     .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))
                                     .arg(QChar('a' + QRandomGenerator::global()->bounded(26)))+newFP.viewPoint.id.right(5);
            currentFP.viewPoint.childId.append(newFP.viewPoint.id);
            m_filePathsMap[newFP.viewPoint.id] = newFP;
            m_filePaths.append(newFP);
            m_ids.append(newFP.viewPoint.id);

        }
    }

    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.childId =  currentFP.viewPoint.childId;
            fp.viewPoint.linkNum = i;
            fp.viewPoint.selects = currentFP.viewPoint.selects;
            fp.viewPoint.isChoices = currentFP.viewPoint.isChoices;
            break;
        }
    }

    initScene();
    m_scene->update();
}

void PageMain::setCg(){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    if(m_setCging){
        currentFP.viewPoint.Cg = m_cgId;
        m_cgId.clear();
        m_setCging = false;
    }else{
        m_cgId = currentFP.viewPoint.id;
        currentFP.viewPoint.Cg = m_cgId;
        m_setCging = true;
    }
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.Cg = currentFP.viewPoint.Cg;
            break;
        }
    }
}

void PageMain::deleteCg(){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    QString cgid = currentFP.viewPoint.Cg;
    currentFP.viewPoint.Cg = "";
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.Cg = "";
            break;
        }
    }
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.Cg == cgid) {
            fp.viewPoint.Cg = "";
            m_filePathsMap[fp.viewPoint.id].viewPoint.Cg = "";
            break;
        }
    }
}

void PageMain::changeWhatHave(QList<QString> whatHave){
    if (m_IdNow.isEmpty() ) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    currentFP.viewPoint.whatHave = whatHave;
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.viewPoint.whatHave = whatHave;
            break;
        }
    }
}

void PageMain::onSaveTriggered() {
    QString saveDir = m_loadjsonPath + "/jsons/game";
    bool success = jc.savePageMain(saveDir, m_filePaths);

    if (success) {
        QMessageBox::information(this, "成功", "数据已按ID前5字符分组保存");
    } else {
        QMessageBox::warning(this, "警告", "部分文件保存失败");
    }
}

void PageMain::setSceneAnimation(const QString& animationName){
    if (m_IdNow.isEmpty() || !m_filePathsMap.contains(m_IdNow)) {
        return;
    }

    filePaths &currentFP = m_filePathsMap[m_IdNow];
    QString animations = currentFP.objectName;

    if (animationName == "clear") {
        animations = "none";
    } else {
        // 判断新动画类型
        bool isInAnimation = animationName.contains("In", Qt::CaseInsensitive);

        if (animations == "none") {
            // 初始状态，直接设置
            animations = animationName;
        } else if (animations.contains("|")) {
            // 已经有两个动画，替换对应类型的动画
            QStringList parts = animations.split("|");
            QString before = parts[0].trimmed();
            QString after = parts[1].trimmed();

            // 判断前后哪个是IN哪个是OUT
            bool beforeIsIn = before.contains("In", Qt::CaseInsensitive);
            bool afterIsIn = after.contains("In", Qt::CaseInsensitive);

            if (isInAnimation) {
                // 替换IN动画
                if (beforeIsIn) {
                    before = animationName;
                } else if (afterIsIn) {
                    after = animationName;
                } else {
                    // 如果没有IN动画，添加到前面
                    before = animationName;
                }
            } else {
                // 替换OUT动画
                if (!beforeIsIn) {
                    before = animationName;
                } else if (!afterIsIn) {
                    after = animationName;
                } else {
                    // 如果没有OUT动画，添加到后面
                    after = animationName;
                }
            }

            animations = before + "|" + after;
        } else {
            // 当前只有一个动画
            bool currentIsIn = animations.contains("In", Qt::CaseInsensitive);

            if (currentIsIn && isInAnimation) {
                // 都是IN，替换
                animations = animationName;
            } else if (!currentIsIn && !isInAnimation) {
                // 都是OUT，替换
                animations = animationName;
            } else {
                // 一个IN一个OUT，用 | 连接
                if (currentIsIn) {
                    animations = animations + "|" + animationName;  // IN|OUT
                } else {
                    animations = animationName + "|" + animations;  // IN|OUT
                }
            }
        }
    }

    // 更新数据
    currentFP.objectName = animations;
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
            fp.objectName = animations;
            break;
        }
    }

}

void PageMain::controlAnimation(){
    if (m_IdNow.isEmpty()) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }

    filePaths &currentFP = m_filePathsMap[m_IdNow];
    QString animations = currentFP.objectName;
    qDebug()<<animations;

    fadeInAction->setChecked(false);
    fadeOutAction->setChecked(false);

    if(animations.contains("fadeInAction")){
        fadeInAction->setChecked(true);
    }
    if(animations.contains("fadeOutAction")){
        fadeOutAction->setChecked(true);
    }
}

void PageMain::updateCharacterAnimation(const QString& objectName){
    if (m_IdNow.isEmpty()) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    leftIn->setChecked(false);
    rightIn->setChecked(false);
    noIn->setChecked(false);
    leftOut->setChecked(false);
    rightOut->setChecked(false);
    noOut->setChecked(false);

    filePaths &currentFP = m_filePathsMap[m_IdNow];
    for(const auto&message:currentFP.viewPoint.messages){
        if(message.whatisthis == objectName){
            if(message.inAnimation == "leftIn"){
                leftIn->setChecked(true);
            }
            if(message.inAnimation == "rightIn"){
                rightIn->setChecked(true);
            }
            if(message.inAnimation == "none"||message.inAnimation.isEmpty()){
                noIn->setChecked(true);
            }
            if(message.outAnimation == "leftOut"){
                leftOut->setChecked(true);
            }
            if(message.outAnimation == "rightOut"){
                rightOut->setChecked(true);
            }
            if(message.outAnimation == "none"||message.outAnimation.isEmpty()){
                noOut->setChecked(true);
            }
            break;
        }
    }
    m_selectedCharacter = objectName;

    m_animationCharacter->exec(QCursor::pos());
}

void PageMain::controlCharacterAnimation(const QString &animationName){
    if (m_IdNow.isEmpty()) {
        return;
    }
    if (!m_filePathsMap.contains(m_IdNow)) {
        return;
    }
    filePaths &currentFP = m_filePathsMap[m_IdNow];
    for(auto&message:currentFP.viewPoint.messages){
        if(message.whatisthis == m_selectedCharacter){
            if(animationName.contains("In")){
                message.inAnimation=animationName;
            }
            else if(animationName.contains("Out")){
                message.outAnimation=animationName;
            }
            else if(animationName == "nonei"){
                message.inAnimation = "none";
            }
            else{message.outAnimation = "none";}
            break;
        }
    }
    for (auto &fp : m_filePaths) {
        if (fp.viewPoint.id == m_IdNow) {
                fp.viewPoint.messages = currentFP.viewPoint.messages;
        }
    }

};
