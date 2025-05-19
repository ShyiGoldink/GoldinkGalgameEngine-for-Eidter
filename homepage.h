#ifndef HOMEPAGE_H
#define HOMEPAGE_H

//这个类就是主类了，就是工作界面

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class HomePage;
}
QT_END_NAMESPACE

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    Ui::HomePage *ui;
};
#endif // HOMEPAGE_H
