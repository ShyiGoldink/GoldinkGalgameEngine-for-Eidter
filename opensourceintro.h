#ifndef OPENSOURCEINTRO_H
#define OPENSOURCEINTRO_H

//这个项目是特地为了介绍开源而做的

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class OpenSourceIntro;
}

class OpenSourceIntro : public QWidget
{
    Q_OBJECT

signals:
    void exit();

protected:
    //重写一个事件监听器，用于保证本页面的大小与landingpage的大小有一个相对比例
    bool eventFilter(QObject *watched,QEvent *event)override;

public:
    explicit OpenSourceIntro(QWidget *parent = nullptr);
    ~OpenSourceIntro();
    void setStyle(QString witchStyle);
    void uiFresh(bool isEnglish);

private:
    Ui::OpenSourceIntro *ui;
    QLabel *label;
    QWidget *widget;
    QPushButton *pushButton;

};

#endif // OPENSOURCEINTRO_H
