#include "catheing.h"
#include "ui_catheing.h"

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

    //将位置移动到左下角
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
}

void Catheing::start(){
    ui->bg->setVisible(true);
}

void Catheing::step1(){
    ui->cat1->setVisible(true);
}

void Catheing::step2(){
    ui->cat2->setVisible(true);
}

void Catheing::step3(){
    ui->cat3->setVisible(true);
}

void Catheing::step4(){
    ui->cat4->setVisible(true);
}

void Catheing::finish(){
    soundEffect->play();
    connect(soundEffect, &QSoundEffect::playingChanged, this, [this](){
        if(!soundEffect->isPlaying()) {
            this->close();
        }
    });
}
