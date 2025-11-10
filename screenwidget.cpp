#include "screenwidget.h"
#include "qapplication.h"
#include "fontmanager.h"

ScreenWidget::ScreenWidget(QWidget *parent)
    : QWidget{parent}
    ,isMain(false)
    ,m_player(nullptr)
    ,m_audioOutput(nullptr)
    ,m_videoWidget(nullptr)
    ,m_currentIndex(0)
    ,m_widget(new QWidget(this))
{
    setupUi();
}

void ScreenWidget::setupUi(){
    setAttribute(Qt::WA_OpaquePaintEvent);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_widget);
    m_mainLayout = new QVBoxLayout(m_widget);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
}

void ScreenWidget::loadMedias(QList<filePaths> files){
    unloadMeidas();

    filePaths start;

    m_filePaths = files;
    if(!files.empty()){
    start = m_filePaths[0];
    }else{return;}
    //这是当第一次使用时，必然为空，此时进行的操作
    //进行update时也要进行这样的操作

    if(m_currentContent)
    m_currentContent->unload();

    loadContents(start);

    update();
}

void ScreenWidget::loadContents(const filePaths &filePath){
    //当filePath为空的时候，则说明此对象为pointMessage，此时创建非pointWidget
    if(m_currentContent) {
        m_mainLayout->removeWidget(m_currentContent.get());
        m_currentContent->unload();
        m_currentContent.reset();
    }

    if(!filePath.isView){
        QFileInfo info(filePath.filePath);
        QString fileType = info.suffix().toLower();
        if(fileType == "jpg" || fileType == "png"){
            m_currentContent = std::make_unique<ImageContent>(m_widget);
            m_mainLayout->addWidget(m_currentContent.get());
            m_currentContent->load(filePath);
        }
        else if(fileType == "mp4" || fileType == "mov"){
            m_currentContent = std::make_unique<VideoContent>(m_widget);
            m_mainLayout->addWidget(m_currentContent.get());
            m_currentContent->load(filePath);
        }
        else if(fileType == "mp3"){
            m_currentContent = std::make_unique<MusicContent>(m_widget);
            m_mainLayout->addWidget(m_currentContent.get());
            m_currentContent->load(filePath);
        }
    }
    else{
        qDebug()<<"have start create point view";
        //这里创建pointWidget，不过暂时还没有到这一步，先缓缓
        m_currentContent = std::make_unique<PointContent>(m_widget);
        m_mainLayout->addWidget(m_currentContent.get());
        m_currentContent->load(filePath);
        connect(m_currentContent.get(),&MediaContent::outMessage,[this](QPointF oldPos,QPointF newPos,QString objectName){
            emit updateData("","updata",objectName+"X",QString::number(newPos.x()));
            emit updateData("","updata",objectName+"Y",QString::number(newPos.y()));
            emit updateGameMain(objectName,newPos.x(),newPos.y());
            updateSceneList(objectName,newPos);
        });
    }

    connect(m_currentContent.get(),&PointContent::toPageMain,this,&ScreenWidget::toPageMain);

    update();
}

void ScreenWidget::unloadMeidas(){
    if(m_currentContent) {
        m_mainLayout->removeWidget(m_currentContent.get());
        m_currentContent->unload();
        m_currentContent.reset();
    }
    //if(m_currentContent)m_mainLayout->removeWidget(m_currentContent.get());
    m_filePaths.clear();
    m_currentIndex = 0;
    //作为大切换，理论来说就是要删除m_filePaths
}

void ScreenWidget::prefer() {
    if (m_filePaths.isEmpty()) return; // 直接返回
    if (m_currentIndex == 0) {
        m_currentIndex = m_filePaths.size() - 1;
    } else {
        m_currentIndex--;
    }
    loadContents(m_filePaths[m_currentIndex]);
}

void ScreenWidget::next(){
    if (m_filePaths.isEmpty()) return; // 直接返回
    if (m_currentIndex == m_filePaths.size() - 1) {
        m_currentIndex = 0;
    } else {
        m_currentIndex++;
    }
    loadContents(m_filePaths[m_currentIndex]);
}

void ScreenWidget::updateMeidas(const filePaths &files) {
    qDebug() << files.filePath << files.objectName << "updateMeidas";
    bool found = false;

    if(m_filePaths.isEmpty()) {
        m_filePaths.append(files);
        m_currentIndex = 0;  // 设置当前索引为0
        loadContents(m_filePaths[m_currentIndex]);  // 立即加载第一个素材
        update();
        return;
    }

    for (int i = 0; i < m_filePaths.count(); ++i) {
        if (m_filePaths[i].objectName == files.objectName) {
            m_filePaths[i].filePath = files.filePath;
            found = true;
            // 如果更新的是当前显示的素材，立即重新加载
            if (i == m_currentIndex) {
                loadContents(m_filePaths[m_currentIndex]);
            }
            break;
        }
    }

    if (!found) {
        m_filePaths.append(files);
    }

    qDebug() << m_filePaths.count();
    update();
}

void ScreenWidget::updateScene(const QString &whatIsThis, const QString &filePath) {
    // 1. 检查索引有效性
    if (m_currentIndex < 0 || m_currentIndex >= m_filePaths.size()) return;

    auto &current = m_filePaths[m_currentIndex].viewPoint;

    // 2. 更新或添加 whatHave
    if (!current.whatHave.contains(whatIsThis)) {
        current.whatHave.append(whatIsThis);
    }

    // 3. 更新或添加 message
    bool messageFound = false;
    for (pointMessages &msg : current.messages) {
        if (msg.whatisthis == whatIsThis) {
            msg.filePath = filePath;
            messageFound = true;
            break;
        }
    }

    if (!messageFound) {
        current.messages.append({whatIsThis, 0, 0, "null", "null", filePath});
    }

    // 4. 刷新界面
    loadContents(m_filePaths[m_currentIndex]);
    update();
}

void ScreenWidget::updateSceneList(QString name,QPointF pos){
    m_filePaths[m_currentIndex].viewPoint.setXY(pos.x(),pos.y(),name);
    qDebug()<<"位置修改这一块";
}

void ScreenWidget::enterEvent(QEnterEvent* event){
    if(!m_filePaths.empty() && !m_filePaths[m_currentIndex].objectName.isEmpty()){
        QToolTip::showText(QCursor::pos(),m_filePaths[m_currentIndex].objectName);
        QWidget::enterEvent(event);
    }
}

void ScreenWidget::leaveEvent(QEvent* event){
    if(!m_filePaths.empty() && !m_filePaths[m_currentIndex].objectName.isEmpty()){
        QToolTip::hideText();
        QWidget::leaveEvent(event);
    }
}

MediaContent::MediaContent(QWidget *parent):
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    parent->installEventFilter(this);
}


ImageContent::ImageContent(QWidget* parent)
    : MediaContent(parent),
    m_imageLabel(new QLabel(this))
{
    // 关键设置1：QLabel配置
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // 关键设置2：容器配置
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 布局设置
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_imageLabel);
}

void ImageContent::load(const filePaths& filePath) {
    m_originalPixmap = QPixmap(filePath.filePath);
    if(!m_originalPixmap.isNull()) {
        updateImageSize(); // 初始缩放
    }else{
        qDebug()<<"wrong in ImageContent::load";
    }
}

void ImageContent::unload() {
    m_imageLabel->clear();
    m_imageLabel->setStyleSheet("background-color:black;");
}
bool ImageContent::eventFilter(QObject* watched,QEvent* event) {
    if(watched == parentWidget()&& event->type() == QEvent::Resize)
    updateImageSize(); // 窗口大小变化时重新缩放

    return QWidget::eventFilter(watched, event);
}

void ImageContent::updateImageSize() {

    this->resize(parentWidget()->width(),parentWidget()->height());
    if(!m_originalPixmap.isNull()) {
        // 计算保持比例的缩放
        QPixmap scaled = m_originalPixmap.scaled(
            this->size(),
            Qt::KeepAspectRatioByExpanding,  // 关键变化
            Qt::SmoothTransformation
            );

        // 居中显示
        m_imageLabel->setPixmap(scaled);
        m_imageLabel->setFixedSize(scaled.size()); // 防止QLabel自动拉伸
    }
}

VideoContent::VideoContent(QWidget* parent)
    : MediaContent(parent),
    m_player(new QMediaPlayer(this)),
    m_output(new QAudioOutput(this)),
    m_videoWidget(new QVideoWidget(this))
{
    m_player->moveToThread(this->thread());

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    m_videoWidget->setStyleSheet("background:transparent;");

    m_player->setAudioOutput(m_output);
    m_player->setVideoOutput(m_videoWidget);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_videoWidget);
}

VideoContent::~VideoContent(){
    ;
}

void VideoContent::load(const filePaths& filePath) {
    connect(m_player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error){
        qWarning() << "Media error:" << error;
    });

    m_player->setSource(QUrl::fromLocalFile(filePath.filePath));
    m_player->play();

    // 保持宽高比
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](){
        if(m_player->mediaStatus() == QMediaPlayer::LoadedMedia) {
            updateVideoSize();
        }
    });
}

void VideoContent::unload(){
    m_player->stop();
}

bool VideoContent::eventFilter(QObject* watched,QEvent* event){
    if(watched == parentWidget()&& event->type() == QEvent::Resize)
        updateVideoSize();

    return QWidget::eventFilter(watched, event);
}

void VideoContent::updateVideoSize(){
    this->resize(parentWidget()->width(),parentWidget()->height());
    if(!m_player->source().isEmpty()){
        m_videoWidget->resize(parentWidget()->width(),parentWidget()->height());
    }

}

MusicContent::MusicContent(QWidget* parent)
    : MediaContent(parent),
    m_player(new QMediaPlayer(this)),
    m_output(new QAudioOutput(this)),
    m_widget(new QWidget(this))
{
    m_player->moveToThread(this->thread());

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_widget->setStyleSheet("background-color:black;");

    m_player->setAudioOutput(m_output);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_widget);
}

MusicContent::~MusicContent(){
   ;
}

void MusicContent::load(const filePaths& filePath) {
    m_player->setSource(QUrl::fromLocalFile(filePath.filePath));
    m_player->play();
}

void MusicContent::unload(){
    m_player->stop();
}

bool MusicContent::eventFilter(QObject* watched,QEvent* event){
    if(watched == parentWidget()&& event->type() == QEvent::Resize)
        updateMusicSize();

    return QWidget::eventFilter(watched, event);
}

void MusicContent::updateMusicSize(){
    this->resize(parentWidget()->width(),parentWidget()->height());
    if(!m_player->source().isEmpty()){
        m_widget->resize(parentWidget()->width(),parentWidget()->height());
    }

}

PointContent::PointContent(QWidget *parent)
    : MediaContent(parent),
    m_widget(new QWidget(this)),
    m_scene(new QGraphicsScene(this)),  // 改为以this为父对象
    m_view(nullptr)                     // 初始化m_view
{
    // 设置主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_widget);

    // 设置widget的布局
    QHBoxLayout *widgetLayout = new QHBoxLayout(m_widget);
    widgetLayout->setContentsMargins(0, 0, 0, 0);
    m_widget->setContentsMargins(0,0,0,0);

}

PointContent::~PointContent()
{
    unload();  // 确保资源释放
    // m_scene会被Qt自动删除，因为它的父对象是this
}

void PointContent::load(const filePaths &file)
{
    // 清理旧内容
    unload();

    // 先创建 QGraphicsView，然后再获取设备像素比
    m_view = new QGraphicsView(m_scene, m_widget);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setTransform(QTransform());
    m_view->setRenderHint(QPainter::SmoothPixmapTransform, false);

    // 现在可以安全地使用 m_view 获取设备像素比
    QScreen *screen = QApplication::primaryScreen();
    qreal realWidth = 1536 / m_view->devicePixelRatioF();
    qreal realHeight = 849 / m_view->devicePixelRatioF();

    // 设置场景尺寸
    m_scene->setSceneRect(0, 0, realWidth, realHeight);

    // 加载新内容
    for (const QString &itemName : file.viewPoint.whatHave) {
        for (const pointMessages &msg : file.viewPoint.messages) {
            if (itemName == msg.whatisthis) {

                if (msg.whatisthis == "对话内容") {
                    // 处理文本内容
                    if(msg.filePath.isEmpty()){continue;}
                    QString textContent = msg.filePath;
                    textContent = textContent.replace("^","\n");

                    // 创建文本图形项
                    QGraphicsTextItem *textItem = new QGraphicsTextItem();

                    // 设置字体
                    QFont font = FontManager::getFont(FontManager::m_fontNow.family());
                    font.setPixelSize(20);
                    textItem->setFont(font);

                    // 处理文本：转义HTML特殊字符，并将换行符转换为<br>
                    QString htmlContent = textContent.toHtmlEscaped().replace("\n", "<br>");

                    // 使用更简洁的HTML结构
                    QString htmlText = QString("<span style=\"line-height: 150%;\">%1</span>")
                                           .arg(htmlContent);
                    textItem->setHtml(htmlText);

                    textItem->setDefaultTextColor(Qt::white);

                    // 设置文本位置（中左下方）
                    qreal textX = realWidth * 0.1;
                    qreal textY = realHeight * 0.7;
                    textItem->setPos(textX, textY);

                    // 设置文本宽度
                    qreal textWidth = realWidth * 0.8;
                    textItem->setTextWidth(textWidth);
                    textItem->setZValue(10);


                    // 标记为对话内容
                    textItem->setData(0, "对话内容");

                    m_scene->addItem(textItem);

                } else {
                    // 原有的图片加载逻辑
                    QFileInfo fileInfo(msg.filePath);
                    if (!fileInfo.exists()) {
                        qWarning() << "File not found:" << msg.filePath;
                        continue;
                    }

                    QPixmap pixmap(msg.filePath);
                    float rate = screen->devicePixelRatio();
                    pixmap = pixmap.scaled(pixmap.width()*0.6/rate, pixmap.height()*0.6/rate);
                    if (pixmap.isNull()) {
                        qWarning() << "Failed to load pixmap:" << msg.filePath;
                        continue;
                    }

                    MovablePixmapItem *item = new MovablePixmapItem(pixmap, msg.whatisthis);
                    if(msg.whatisthis == "背景"){
                        item->setZValue(-114);
                    }
                    if(msg.whatisthis == "对话框"){
                        item->setZValue(5);
                    }
                    if(msg.whatisthis.contains("角色")){;
                        connect(item,&MovablePixmapItem::selectedCharacterN,this,&PointContent::toPageMain);
                    }
                    connect(item, &MovablePixmapItem::positionChanged, this, &PointContent::outMessage);
                    connect(item, &MovablePixmapItem::show, this, &PointContent::setVisiable);
                    connect(item, &MovablePixmapItem::unshow, this, &PointContent::setUnvisiable);
                    item->setPos(msg.x, msg.y);
                    m_scene->addItem(item);
                    m_pixmap.append(pixmap);
                }
            }
        }
    }

    // 最后添加到布局
    static_cast<QHBoxLayout*>(m_widget->layout())->addWidget(m_view);
}
void PointContent::unload()
{
    if (m_view) {
        m_view->setParent(nullptr);  // 从布局中移除
        delete m_view;               // 直接删除
        m_view = nullptr;
    }
    m_scene->clear();  // 清除所有场景项
    m_pixmap.clear();  // 清空图片缓存
}

void PointContent::setVisiable(){
    qreal width = m_scene->width();
    qreal height = m_scene->height();

    for(qreal y = 0;y<=height;y=y+20){
        QGraphicsLineItem* line = m_scene ->addLine(0,y,width,y,QPen(QColor(100,100,100,100),1));
        line->setZValue(114);
    }
    for(qreal x = 0;x<=width;x=x+20){
        QGraphicsLineItem* line = m_scene ->addLine(x,0,x,height,QPen(QColor(100,100,100,100),1));
        line->setZValue(114);
    }

    qreal realWidth = 1536/ m_view->devicePixelRatioF();
    qreal realHeight = 849 / m_view->devicePixelRatioF();

    QGraphicsLineItem* line1 = m_scene->addLine(0,0,0,realHeight,QPen(QColor(0,0,0,100),2));
    QGraphicsLineItem* line2 = m_scene->addLine(0,0,realWidth,0,QPen(QColor(0,0,0,100),2));
    QGraphicsLineItem* line3 = m_scene->addLine(realWidth,0,realWidth,realHeight,QPen(QColor(0,0,0,100),2));
    QGraphicsLineItem* line4 = m_scene->addLine(0,realHeight,realWidth,realHeight,QPen(QColor(0,0,0,100),2));

    line1 -> setZValue(514);
    line2 -> setZValue(514);
    line3 -> setZValue(514);
    line4 -> setZValue(514);

}

void PointContent::setUnvisiable() {
    QList<QGraphicsItem*> items = m_scene->items();
    for (QGraphicsItem* item : items) {
        if (item->type() == QGraphicsLineItem::Type) {  // 使用标准类型枚举
            m_scene->removeItem(item);
            delete item;
        }
    }
}
