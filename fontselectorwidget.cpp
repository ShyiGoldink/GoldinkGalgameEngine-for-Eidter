#include "fontselectorwidget.h"
#include <QDebug>
#include <QMap>

FontSelectorWidget::FontSelectorWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshFontList();
}

void FontSelectorWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);

    // 当前字体显示
    m_currentFontLabel = new QLabel(this);
    m_currentFontLabel->setText("当前字体: " + FontManager::m_fontNow.family());
    m_currentFontLabel->setFont(FontManager::m_fontNow);
    m_currentFontLabel->setStyleSheet("QLabel { padding: 10px; background-color: #f0f0f0; border-radius: 5px; }");
    m_mainLayout->addWidget(m_currentFontLabel);

    // 字体列表区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_scrollContent = new QWidget();
    m_scrollLayout = new QVBoxLayout(m_scrollContent);
    m_scrollLayout->setAlignment(Qt::AlignTop);

    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);
}

void FontSelectorWidget::refreshFontList()
{
    clearFontButtons();

    // 获取所有字体
    QMap<QString, QString> fontMap = jc.getAllFontsFromJson();

    if (fontMap.isEmpty()) {
        QLabel *noFontLabel = new QLabel("没有可用的字体", m_scrollContent);
        noFontLabel->setAlignment(Qt::AlignCenter);
        noFontLabel->setStyleSheet("QLabel { color: #666; padding: 20px; }");
        m_scrollLayout->addWidget(noFontLabel);
        return;
    }

    // 为每种字体创建按钮
    for (auto it = fontMap.begin(); it != fontMap.end(); ++it) {
        QString fontName = it.key();

        // 创建字体按钮
        TextButton *fontButton = new TextButton(m_scrollContent, fontName);

        // 设置按钮字体为对应的字体
        QFont buttonFont = FontManager::getFont(fontName);
        fontButton->setFont(buttonFont);

        // 设置按钮属性
        fontButton->setProperty("fontName", fontName);
        fontButton->setMinimumHeight(40);
        fontButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // 如果是当前字体，高亮显示
        if (fontName == jc.getFontUserUse()) {
            fontButton->setStyleSheet(
                "QPushButton { "
                "color: rgb(0,115,175); "
                "border: 2px solid rgb(0,115,175); "
                "border-radius: 5px; "
                "background-color: #e6f3ff; "
                "padding: 8px; "
                "}"
                "QPushButton:hover { "
                "color: rgb(27,41,79); "
                "background-color: #d0e7ff; "
                "}"
                );
        }

        // 连接信号槽
        connect(fontButton, &TextButton::clicked, this, &FontSelectorWidget::onFontButtonClicked);

        m_scrollLayout->addWidget(fontButton);
    }

    // 添加弹簧
    m_scrollLayout->addStretch();
}

void FontSelectorWidget::onFontButtonClicked()
{
    TextButton *clickedButton = qobject_cast<TextButton*>(sender());
    if (!clickedButton) return;

    QString fontName = clickedButton->property("fontName").toString();

    if (!fontName.isEmpty()) {
        // 设置当前字体
        FontManager::setCurrentFont(fontName);

        // 更新当前字体显示
        m_currentFontLabel->setText("当前字体: " + FontManager::m_fontNow.family());
        m_currentFontLabel->setFont(FontManager::m_fontNow);

        // 刷新字体列表以更新高亮状态
        refreshFontList();

        qDebug() << "字体已切换为:" << fontName;

        // 发出信号通知字体已改变（如果需要）
        // emit fontChanged(fontName);
    }
}

void FontSelectorWidget::clearFontButtons()
{
    // 清除所有字体按钮
    QLayoutItem *item;
    while ((item = m_scrollLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}
