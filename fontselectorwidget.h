#ifndef FONTSELECTORWIDGET_H
#define FONTSELECTORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include "textbutton.h"
#include "fontmanager.h"
#include "jsoncontorller.h"

class FontSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FontSelectorWidget(QWidget *parent = nullptr);
    void refreshFontList();

private slots:
    void onFontButtonClicked();

private:
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QVBoxLayout *m_scrollLayout;
    QLabel *m_currentFontLabel;
    JsonContorller jc;

    void setupUI();
    void clearFontButtons();
};

#endif // FONTSELECTORWIDGET_H
