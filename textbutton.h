#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

//这是一个QPushButton的子类，默认是蓝色的字体，cover之后成为深蓝色字体，并且扁平且无背景
//这个看起来就像是能点击的字符串

#include "qpushbutton.h"

class TextButton:public QPushButton
{
    Q_OBJECT
public:
    TextButton(QWidget *parent = nullptr,QString textOnButton = "");//新添加的textOnButton是方便写Button时直接添加上面的字
    void setColor(const QString &rgb,const QString &hrgb);//修改按钮的颜色
};

#endif // TEXTBUTTON_H
