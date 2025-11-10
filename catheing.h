#ifndef CATHEING_H
#define CATHEING_H

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
    static Catheing* instance(); // 单例访问方法
    static void start();//开始
    static void step1();//其余加载动画
    static void step2();
    static void step3();
    static void step4();
    static void finish();//结束，播放音乐
    ~Catheing();

private:
    explicit Catheing(QWidget *parent = nullptr);
    Ui::Catheing *ui;
    QSoundEffect *soundEffect;//用于结束时的猫叫
    static Catheing* m_instance; // 单例实例
};

#endif // CATHEING_H
