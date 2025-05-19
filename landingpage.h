#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

//这是引导页面，目的是为了引导用户对本游戏引擎使用

#include <QDialog>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

#include "textbutton.h"
#include "jsoncontorller.h"

namespace Ui {
class LandingPage;
}

class LandingPage : public QDialog
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr,bool isStart = true);//构造函数中，isStart是为了方便用户的使用，
                                                                               //如果是在使用前，则传入true，正常使用
                                                                               //如果是在使用中打开引导页寻求帮助，则有一些功能无法使用
    ~LandingPage();

private:
    Ui::LandingPage *ui;
    bool m_isStart;
    TextButton *openSource;
    TextButton *newProject;
    TextButton *openProject;
    TextButton *exit;
    QLabel *geryLabel;
    JsonContorller jc;


};

#endif // LANDINGPAGE_H
