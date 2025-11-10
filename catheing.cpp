#include "catheing.h"
#include "ui_catheing.h"

Catheing* Catheing::m_instance = nullptr;

Catheing* Catheing::instance() {
    if (!m_instance) {
        m_instance = new Catheing();
    }
    return m_instance;
}

Catheing::Catheing(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Catheing)
    ,soundEffect(new QSoundEffect(this))
{
    ui->setupUi(this);

    //首先去除边框，并且修改背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QDialog { background: transparent; }");

    //将位置移动到右下角
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->move(screenWidth-this->width(),screenHeight-this->height());

    ui->bg->setVisible(false);
    ui->cat1->setVisible(false);
    ui->cat2->setVisible(false);
    ui->cat3->setVisible(false);
    ui->cat4->setVisible(false);

    soundEffect->setSource(QUrl::fromLocalFile(":/new/prefix1/catVoice.WAV"));
}

Catheing::~Catheing()
{
    delete ui;
    m_instance = nullptr;
}

void Catheing::start(){
    instance()->show();
    instance()->raise(); // 确保窗口在最前面
    instance()->activateWindow(); // 激活窗口
    instance()->ui->bg->setVisible(true);
}

void Catheing::step1(){
    instance()->ui->cat1->setVisible(true);
}

void Catheing::step2(){
    instance()->ui->cat2->setVisible(true);
}

void Catheing::step3(){
    instance()->ui->cat3->setVisible(true);
}

void Catheing::step4(){
    instance()->ui->cat4->setVisible(true);
}

void Catheing::finish(){
    instance()->soundEffect->play();
    connect(instance()->soundEffect, &QSoundEffect::playingChanged, instance(), [](){
        if(!instance()->soundEffect->isPlaying()) {
            instance()->close();
        }
    });
}
