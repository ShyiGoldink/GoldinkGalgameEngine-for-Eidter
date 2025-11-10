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
    QAction *blackWhite;
    QAction *pinkBlue;
    QAction *openProjectAction;
    QAction *englishAction;
    QAction *chineseAction;
    QAction *importItemAction;
    QAction *saveProjectAction;
    QAction *multiProjectAction;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu_F;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menuLanguage;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName("HomePage");
        HomePage->resize(630, 422);
        action_2 = new QAction(HomePage);
        action_2->setObjectName("action_2");
        blackWhite = new QAction(HomePage);
        blackWhite->setObjectName("blackWhite");
        pinkBlue = new QAction(HomePage);
        pinkBlue->setObjectName("pinkBlue");
        openProjectAction = new QAction(HomePage);
        openProjectAction->setObjectName("openProjectAction");
        englishAction = new QAction(HomePage);
        englishAction->setObjectName("englishAction");
        chineseAction = new QAction(HomePage);
        chineseAction->setObjectName("chineseAction");
        importItemAction = new QAction(HomePage);
        importItemAction->setObjectName("importItemAction");
        saveProjectAction = new QAction(HomePage);
        saveProjectAction->setObjectName("saveProjectAction");
        multiProjectAction = new QAction(HomePage);
        multiProjectAction->setObjectName("multiProjectAction");
        centralwidget = new QWidget(HomePage);
        centralwidget->setObjectName("centralwidget");
        HomePage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(HomePage);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 630, 18));
        menu_F = new QMenu(menubar);
        menu_F->setObjectName("menu_F");
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menu);
        menu_2->setObjectName("menu_2");
        menuLanguage = new QMenu(menubar);
        menuLanguage->setObjectName("menuLanguage");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        HomePage->setMenuBar(menubar);
        statusbar = new QStatusBar(HomePage);
        statusbar->setObjectName("statusbar");
        HomePage->setStatusBar(statusbar);

        menubar->addAction(menu_F->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menuLanguage->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu_F->addAction(openProjectAction);
        menu_F->addAction(importItemAction);
        menu_F->addAction(saveProjectAction);
        menu->addAction(menu_2->menuAction());
        menu_2->addAction(blackWhite);
        menu_2->addAction(pinkBlue);
        menuLanguage->addAction(englishAction);
        menuLanguage->addAction(chineseAction);
        menu_3->addAction(multiProjectAction);

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QMainWindow *HomePage)
    {
        HomePage->setWindowTitle(QCoreApplication::translate("HomePage", "HomePage", nullptr));
        action_2->setText(QCoreApplication::translate("HomePage", "\347\273\217\345\205\270\351\273\221\347\231\275", nullptr));
        blackWhite->setText(QCoreApplication::translate("HomePage", "\347\273\217\345\205\270\351\273\221\347\231\275", nullptr));
        pinkBlue->setText(QCoreApplication::translate("HomePage", "\345\217\257\347\210\261\347\262\211\350\223\235", nullptr));
        openProjectAction->setText(QCoreApplication::translate("HomePage", "\346\211\223\345\274\200\351\241\271\347\233\256", nullptr));
        englishAction->setText(QCoreApplication::translate("HomePage", "English", nullptr));
        chineseAction->setText(QCoreApplication::translate("HomePage", "\347\256\200\344\275\223\344\270\255\346\226\207", nullptr));
        importItemAction->setText(QCoreApplication::translate("HomePage", "\345\257\274\345\205\245\347\264\240\346\235\220", nullptr));
        saveProjectAction->setText(QCoreApplication::translate("HomePage", "\344\277\235\345\255\230", nullptr));
        multiProjectAction->setText(QCoreApplication::translate("HomePage", "\345\274\200\345\247\213\346\270\262\346\237\223", nullptr));
        menu_F->setTitle(QCoreApplication::translate("HomePage", "\346\226\207\344\273\266&F", nullptr));
        menu->setTitle(QCoreApplication::translate("HomePage", "\351\246\226\351\200\211\351\241\271&H", nullptr));
        menu_2->setTitle(QCoreApplication::translate("HomePage", "\347\225\214\351\235\242\351\243\216\346\240\274", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("HomePage", "Language", nullptr));
        menu_3->setTitle(QCoreApplication::translate("HomePage", "\346\270\262\346\237\223", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
