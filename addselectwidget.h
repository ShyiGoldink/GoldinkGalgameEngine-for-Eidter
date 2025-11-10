#ifndef ADDSELECTWIDGET_H
#define ADDSELECTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

class addSelectWidget: public QWidget
{
    Q_OBJECT
public:
    explicit addSelectWidget(const int i = 0,QWidget *parent = nullptr);
signals:
    void giveSelects(int i,QList<QString> selects);
    void giveCancle();
private:

    void ensureSlot();

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_pushbuttonLayout;
    QGridLayout *m_labelLayout;
    QPushButton *m_ensure;
    QPushButton *m_cancle;
    QList<QLineEdit*> m_edits;
    int m_nums;
};

#endif // ADDSELECTWIDGET_H
