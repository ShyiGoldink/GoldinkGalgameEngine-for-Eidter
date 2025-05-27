#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
    ,lp(new LandingPage(this))
{
    ui->setupUi(this);
    lp->raise();//引导页最上层展示
    lp->show();
    connect(lp,&LandingPage::exExit,[this](){
        lp->close();
        this->close();
    });//退出程序

    //记忆点：需要做修改整体配色的功能在“首选项”中

}

HomePage::~HomePage()
{
    delete ui;
}
