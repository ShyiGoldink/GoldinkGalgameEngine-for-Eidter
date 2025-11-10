#include "meidias.h"

Meidias::Meidias(QWidget *parent, QString filePath, int x, int y)
    : QWidget(parent),
    isPicture(false),
    m_player(nullptr),
    m_audioOutput(nullptr),
    m_videoWidget(nullptr),
    m_imageLabel(nullptr)
{
    QString fileType = QFileInfo(filePath).suffix().toLower();

    // 设置窗口位置
    this->move(x, y);

    //取消边框
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    if(fileType == "png" || fileType == "jpg" || fileType == "ico") {
        // 图片处理
        m_imageLabel = new QLabel(this);
        isPicture = true;

        QPixmap pixmap(filePath);
        if(pixmap.isNull()) {
            QMessageBox::warning(this, "图片加载失败", "无法加载指定的图片文件");
            return;
        }

        // 缩放图片并保持宽高比
        pixmap = pixmap.scaledToWidth(300, Qt::SmoothTransformation);
        m_imageLabel->setPixmap(pixmap);
        this->resize(pixmap.size());

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(m_imageLabel);
    }
    else if(fileType == "mp3" || fileType == "wav") {
        // 音频处理
        this->resize(200, 60);
        setAttribute(Qt::WA_TranslucentBackground);

        m_player = new QMediaPlayer(this);
        m_audioOutput = new QAudioOutput(this);
        m_player->setAudioOutput(m_audioOutput);
        m_player->setSource(QUrl::fromLocalFile(filePath));

        // 创建控制界面
        QPushButton *pushButton = new QPushButton(this);
        connect(pushButton,&QPushButton::clicked,[this](){
            m_player->stop();
            this->close();
        });
        pushButton->setText("X");
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout -> addStretch(2);
        layout -> addWidget(pushButton,1);
        layout -> addStretch(2);

        m_player->play();
    }
    else if(fileType == "mp4") {
        // 视频处理
        m_player = new QMediaPlayer(this);
        m_audioOutput = new QAudioOutput(this);
        m_videoWidget = new QVideoWidget(this);

        // 视频显示设置
        m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
        m_videoWidget->setStyleSheet("background: transparent;");

        m_player->setAudioOutput(m_audioOutput);
        m_player->setVideoOutput(m_videoWidget);
        m_player->setSource(QUrl::fromLocalFile(filePath));

        // 关闭按钮
        QPushButton *pushButton = new QPushButton("X", this);
        pushButton->setFixedSize(30, 30);
        connect(pushButton, &QPushButton::clicked, this, &Meidias::close);

        // 主布局
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // 视频区域
        QHBoxLayout *videoLayout = new QHBoxLayout();
        videoLayout->addWidget(m_videoWidget);
        mainLayout->addLayout(videoLayout, 1);

        // 控制按钮区域
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(pushButton);
        buttonLayout->addStretch();
        mainLayout->addLayout(buttonLayout);


        m_player->play();
    }
    else {
        QMessageBox::warning(this, "不支持的格式", "不支持的文件格式: " + fileType);

    }
}

Meidias::~Meidias()
{
    if(m_player) {
        m_player->stop();
    }
}

void Meidias::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton && isPicture){
        this -> close();
    }
    QWidget::mousePressEvent(event);
}

