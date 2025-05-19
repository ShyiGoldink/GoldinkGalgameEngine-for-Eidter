/********************************************************************************
** Form generated from reading UI file 'landingpage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LANDINGPAGE_H
#define UI_LANDINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_LandingPage
{
public:

    void setupUi(QDialog *LandingPage)
    {
        if (LandingPage->objectName().isEmpty())
            LandingPage->setObjectName("LandingPage");
        LandingPage->resize(400, 300);
        LandingPage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 239, 238);\n"
"color: rgb(255, 213, 205);"));

        retranslateUi(LandingPage);

        QMetaObject::connectSlotsByName(LandingPage);
    } // setupUi

    void retranslateUi(QDialog *LandingPage)
    {
        LandingPage->setWindowTitle(QCoreApplication::translate("LandingPage", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LandingPage: public Ui_LandingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LANDINGPAGE_H
