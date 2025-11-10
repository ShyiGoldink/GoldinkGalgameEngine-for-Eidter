#ifndef HOMEPAGE_H
#define HOMEPAGE_H

//这个类是主界面，也就是用于控制中心


#include <QMainWindow>
#include <QTabWidget>
#include <QTranslator>

#include "landingpage.h"
#include "catheing.h"
#include "jsoncontorller.h"
#include "tabs.h"
#include "txtsearch.h"
#include "Structs.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class HomePage;
}
QT_END_NAMESPACE

class HomePage : public QMainWindow
{
    Q_OBJECT

signals:
    void changeStyle(QString witchStyle);
    void tabsNum(int num);

public:
    void setStyleAll(QString whichStyle,bool isStart);//为全部的界面设置风格 如果是start则需要无论内容如何都必须进行调用
    void openProject(QString filePath);//打开界面
    void languageChange(bool isEnglish);
    void save();//保存所有文件
    void importItem(const QString &filePath);//导入素材
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

protected slots:
    void tabClosSlot(int i);

private:
    Ui::HomePage *ui;
    LandingPage *lp;
    Catheing *cat;
    JsonContorller jc;
    QTabWidget *tabs;
    QList<QString> projects;
    QList<saves> m_saves;
    QString style;//这是用于储存风格，以便于生成tabs时可以成为非默认风格
    QTranslator *translator;
};
#endif // HOMEPAGE_H
