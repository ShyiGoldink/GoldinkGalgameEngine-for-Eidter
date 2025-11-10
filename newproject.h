#ifndef NEWPROJECT_H
#define NEWPROJECT_H

//本类是用于新建项目的，用于引导页和主页面

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "catheing.h"
#include "jsoncontorller.h"

namespace Ui {
class NewProject;
}

class NewProject : public QWidget
{
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = nullptr,bool isDefault = false);//如果ture则意味着在引导页，要将大小修改为引导页当时的大小
    ~NewProject();
    void loadLanding();
    void setStyle(QString witchStyle);
    void uiFresh(bool isEnglish);

protected:
    //重写一个事件监听器，用于保证本页面的大小与landingpage的大小有一个相对比例
    bool eventFilter(QObject *watched,QEvent *event)override;


signals:
    void exit();
    void newFinish(QString filePath);

public slots:
    void browseSlot();//这是用于获取项目位置的信息
    void ensureSlot();

private:
    Ui::NewProject *ui;
    QString newFilePath;//用于储存新建项目的位置
    QLabel *name1;//提示用户在哪里输入项目名称
    QLabel *name2;//提示用户在哪里输入地址名称
    QLineEdit *projectName;//输入项目名称的地方
    QLineEdit *projectPath;//输入项目地址的地方
    QPushButton *browse;//浏览目录的地方
    QPushButton *ensure;//确定
    QPushButton *ensureLive2D;//live2D模式，启动！
    QPushButton *cancel;//取消
    bool m_isDefault;//保存normal
    Catheing *catheing;
    JsonContorller js;
};

#endif // NEWPROJECT_H
