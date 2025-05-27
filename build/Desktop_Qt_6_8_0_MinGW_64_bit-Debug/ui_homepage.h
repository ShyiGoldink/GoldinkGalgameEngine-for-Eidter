/********************************************************************************
** Form generated from reading UI file 'homepage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QAction *action_2;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu_F;
    QMenu *menu_H;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName("HomePage");
        HomePage->resize(630, 422);
        action_2 = new QAction(HomePage);
        action_2->setObjectName("action_2");
        centralwidget = new QWidget(HomePage);
        centralwidget->setObjectName("centralwidget");
        HomePage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(HomePage);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 630, 18));
        menu_F = new QMenu(menubar);
        menu_F->setObjectName("menu_F");
        menu_H = new QMenu(menubar);
        menu_H->setObjectName("menu_H");
        menu = new QMenu(menu_H);
        menu->setObjectName("menu");
        HomePage->setMenuBar(menubar);
        statusbar = new QStatusBar(HomePage);
        statusbar->setObjectName("statusbar");
        HomePage->setStatusBar(statusbar);

        menubar->addAction(menu_F->menuAction());
        menubar->addAction(menu_H->menuAction());
        menu_H->addAction(menu->menuAction());
        menu->addAction(action_2);

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QMainWindow *HomePage)
    {
        HomePage->setWindowTitle(QCoreApplication::translate("HomePage", "HomePage", nullptr));
        action_2->setText(QCoreApplication::translate("HomePage", "\347\273\217\345\205\270\351\273\221\347\231\275", nullptr));
        menu_F->setTitle(QCoreApplication::translate("HomePage", "\346\226\207\344\273\266&F", nullptr));
        menu_H->setTitle(QCoreApplication::translate("HomePage", "\351\246\226\351\200\211\351\241\271&H", nullptr));
        menu->setTitle(QCoreApplication::translate("HomePage", "\347\225\214\351\235\242\351\243\216\346\240\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
