#ifndef CATHEING_H
#define CATHEING_H

//这是设置一个用于表示加载中的类

#include <QDialog>
#include <QScreen>
#include <QSoundEffect>

namespace Ui {
class Catheing;
}

class Catheing : public QDialog
{
    Q_OBJECT

public:
    explicit Catheing(QWidget *parent = nullptr);
    void start();//开始
    void step1();//其余加载动画
    void step2();
    void step3();
    void step4();
    void finish();//结束，播放音乐
    ~Catheing();

private:
    Ui::Catheing *ui;
    QSoundEffect *soundEffect;//用于结束时的猫叫
};

#endif // CATHEING_H
