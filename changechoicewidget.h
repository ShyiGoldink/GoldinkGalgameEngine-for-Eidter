// changechoicewidget.h 补充声明
#ifndef CHANGECHOICEWIDGET_H
#define CHANGECHOICEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QString>

class QPushButton;
class QHBoxLayout;
class QLineEdit;
class QGridLayout;
class QVBoxLayout;

class ChangeChoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChangeChoiceWidget(QWidget *parent = nullptr, QList<QString> selects = {},QList<QString> ids = {});

signals:
    void outMessage(int i, QList<QString> selects,QList<QString> ids);

private slots:
    void onEnsureClicked();

private:
    void initLayout();
    void addLineEdit(const QString &text);
    void removeLineEdit(int index);

private:
    QPushButton* m_ensure;
    QPushButton* m_cancle;
    QHBoxLayout* m_buttons;
    QList<QLineEdit*> m_lines;
    QGridLayout* m_eidts;
    QVBoxLayout* m_main;
    QList<QString> m_ids;
};

#endif // CHANGECHOICEWIDGET_H
