// changechoicewidget.cpp 完整实现
#include "changechoicewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

ChangeChoiceWidget::ChangeChoiceWidget(QWidget *parent, QList<QString> selects, QList<QString> ids)
    : QWidget{parent}
    , m_ensure(new QPushButton(tr("确定"), this))
    , m_cancle(new QPushButton(tr("取消"), this))
    , m_buttons(new QHBoxLayout())
    , m_eidts(new QGridLayout())
    , m_main(new QVBoxLayout(this))
    , m_ids(ids)
{
    // 设置窗口属性
    this->setMinimumSize(500, 500);
    this->move(500, 500);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->setWindowTitle(tr("编辑选项"));

    // 设置为应用程序模态
    this->setWindowModality(Qt::ApplicationModal);

    // 初始化布局
    initLayout();

    // 添加初始数据
    for (const QString &text : selects) {
        addLineEdit(text);
    }

    // 如果没有初始数据，至少添加一个输入框
    if (m_lines.isEmpty()) {
        addLineEdit("");
    }

    // 连接信号槽
    connect(m_ensure, &QPushButton::clicked, this, &ChangeChoiceWidget::onEnsureClicked);
    connect(m_cancle, &QPushButton::clicked, this, &QWidget::close);
}

void ChangeChoiceWidget::initLayout()
{
    // 创建添加按钮
    QPushButton *addBtn = new QPushButton(tr("+ 添加选项"), this);
    connect(addBtn, &QPushButton::clicked, this, [this]() {
        addLineEdit("");
    });

    // 按钮布局
    m_buttons->addWidget(addBtn);
    m_buttons->addStretch();
    m_buttons->addWidget(m_ensure);
    m_buttons->addWidget(m_cancle);

    // 设置网格布局间距
    m_eidts->setSpacing(10);

    // 主布局
    m_main->addLayout(m_eidts);
    m_main->addLayout(m_buttons);
    m_main->setContentsMargins(20, 20, 20, 20);
    m_main->setSpacing(20);

    setLayout(m_main);
}

void ChangeChoiceWidget::addLineEdit(const QString &text)
{
    int row = m_lines.count();

    // 创建输入框
    QLineEdit *lineEdit = new QLineEdit(text, this);
    m_lines.append(lineEdit);

    // 创建删除按钮
    QPushButton *delBtn = new QPushButton(tr("删除"), this);
    connect(delBtn, &QPushButton::clicked, this, [this, row]() {
        removeLineEdit(row);
    });

    // 添加到网格布局
    m_eidts->addWidget(new QLabel(QString("选项 %1:").arg(row + 1), this), row, 0);
    m_eidts->addWidget(lineEdit, row, 1);
    m_eidts->addWidget(delBtn, row, 2);
}

void ChangeChoiceWidget::removeLineEdit(int index)
{
    if (index < 0 || index >= m_lines.count()) {
        return;
    }

    // 至少保留一个输入框
    if (m_lines.count() <= 1) {
        QMessageBox::information(this, "提示", tr("至少保留一个选项"));
        return;
    }


    if(m_ids.count()>=index){
        m_ids.removeAt(index);
    }

    // 移除控件
    QLineEdit *lineEdit = m_lines.takeAt(index);
    if (lineEdit) {
        m_eidts->removeWidget(lineEdit);
        delete lineEdit;
    }

    // 重新排列剩余控件
    QLayoutItem *item;
    while ((item = m_eidts->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // 重新添加剩余的输入框
    QList<QString> remainingTexts;
    for (QLineEdit *le : m_lines) {
        remainingTexts.append(le->text());
    }
    m_lines.clear();

    for (const QString &text : remainingTexts) {
        addLineEdit(text);
    }
}

void ChangeChoiceWidget::onEnsureClicked()
{
    QList<QString> result;
    for (QLineEdit *lineEdit : m_lines) {
        QString text = lineEdit->text().trimmed();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "警告", tr("选项内容不能为空"));
            return;
        }
        result.append(text);
    }

    // 发送结果信号并关闭窗口
    emit outMessage(result.count(),result,m_ids);
    close();
}
