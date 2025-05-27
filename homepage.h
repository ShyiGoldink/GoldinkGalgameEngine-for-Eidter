#ifndef HOMEPAGE_H
#define HOMEPAGE_H

//这个类是主界面，也就是用于控制中心

#include <QMainWindow>
#include <landingpage.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class HomePage;
}
QT_END_NAMESPACE

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    void setStyleAll(QString whichStyle);//为全部的界面设置风格
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    Ui::HomePage *ui;
    LandingPage *lp;
};
#endif // HOMEPAGE_H
