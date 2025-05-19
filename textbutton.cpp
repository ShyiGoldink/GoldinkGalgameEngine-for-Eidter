#include "textbutton.h"

TextButton::TextButton(QWidget *parent,QString textOnButton)
    : QPushButton(parent)
{   this->setText(textOnButton);
    this->setFlat(true);
    this->setStyleSheet("QPushButton { border: none; }");
    this->setColor("rgb(0,115,175)","rgb(27,41,79)");
    QFont font;
    font.bold();
    font.setPixelSize(17);
    this->setFont(font);
}

//修改按钮颜色
void TextButton::setColor(const QString &rgb,const QString &hrgb)
{
    this->setStyleSheet(QString(
                            "QPushButton { color: %1; border: none; }"
                            "QPushButton:hover { color: %2; }"
                            ).arg(rgb,hrgb));
}
