#ifndef SELECTBOX_H
#define SELECTBOX_H

//根据输入文件动态生成选择块

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QEvent>
#include <jsoncontorller.h>

class SelectBox : public QWidget
{
    Q_OBJECT
public:
    explicit SelectBox(QWidget *parent = nullptr,const QVector<QString> &vector = {},const QVector<QString> &haveClicked = {});//vector是用于生成图标的
    QVector<QString> selected();
    void setupUI();
    void ensureSlot();
    void cancleSlot();
    void resizeEvent(QResizeEvent *event) override;
signals:
    void outMessage(QList<QString> outmessage);//这里的传输信号目的就是为了传输修改的内容，但是现在还不给予保存，暂时先作为提交到需要保存的位置
                                               //之后做好保存功能之后直接将栈中存储的信息全部用于保存
    void out();//用于关闭界面不做任何处理
    void in();//用于进入页面
private:
    QVector<QString> m_vector;
    QVector<QString> m_haveClicked;
    QList<QString>m_list;
    QPushButton *ensure;
    QPushButton *cancle;
    QPushButton *inandout;//用于进入或者退出
    QWidget* centerWidget;
    bool lock;//用于锁信号，保证inandout按键是正常的逻辑

};

#endif // SELECTBOX_H
