#ifndef ENTEREMITLINEEDIT_H
#define ENTEREMITLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class EnterEmitLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit EnterEmitLineEdit(QWidget *parent = nullptr);

signals:
    void enterPressed(const QString &text);  // 回车按下时发出的信号
    void textSubmitted(const QString &text); // 可选：更语义化的信号名

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void handleEnterKey();
    void handleShiftEnter();
};

#endif // ENTEREMITLINEEDIT_H
