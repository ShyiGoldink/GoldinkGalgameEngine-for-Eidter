#include "enteremitlineedit.h"
#include <QDebug>

EnterEmitLineEdit::EnterEmitLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    // 可以设置一些初始属性
    setPlaceholderText(tr("按回车发送，^换行"));
}

void EnterEmitLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (event->modifiers() & Qt::ShiftModifier) {
            // Shift + 回车：换行
            int cursorPos = cursorPosition();
            QString currentText = text();

            // 在光标位置插入换行符
            currentText.insert(cursorPos, "^");
        } else {
            // 单独回车：发送信号
            handleEnterKey();
        }
        event->accept(); // 标记事件已处理
    } else {
        // 其他按键交给父类处理
        QLineEdit::keyPressEvent(event);
    }
}

void EnterEmitLineEdit::handleEnterKey()
{
    QString currentText = text().trimmed(); // 去除首尾空白字符

    if (!currentText.isEmpty()) {
        // 发出信号
        emit enterPressed(currentText);
        emit textSubmitted(currentText); // 可选

        qDebug() << "Enter pressed, emitting text:" << currentText;

        // 可选：清空输入框
        // clear();
    } else {
        qDebug() << "Enter pressed but text is empty";
    }
}

void EnterEmitLineEdit::handleShiftEnter()
{
    // 获取当前光标位置
    int cursorPos = cursorPosition();
    QString currentText = text();

    // 在光标位置插入换行符
    currentText.insert(cursorPos, "\n");
    setText(currentText);

    // 将光标移动到换行符后
    setCursorPosition(cursorPos + 1);

    qDebug() << "Shift+Enter: inserted newline at position" << cursorPos;
}
