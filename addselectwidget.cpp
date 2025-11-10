#include "addselectwidget.h"

addSelectWidget::addSelectWidget(const int i, QWidget* parent)
    : QWidget(parent)
    , m_mainLayout(new QVBoxLayout(this))  // 这个应该是顶级布局
    , m_pushbuttonLayout(new QHBoxLayout())
    , m_labelLayout(new QGridLayout())
    , m_ensure(new QPushButton(tr("确定"), this))
    , m_cancle(new QPushButton(tr("取消"), this))
    , m_nums(i)
{
    // 设置窗口属性
    this->setMinimumSize(500, 500);
    this->move(500, 500);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    // 设置为应用程序模态（最严格的模态）
    this->setWindowModality(Qt::ApplicationModal);

    // 创建标签和输入框
    for(int j = 0; j < i; j++){
        QLabel *label = new QLabel(tr("分支") + QString::number(j + 1) + ":", this);
        m_labelLayout->addWidget(label, j, 0);

        QLineEdit *textEdit = new QLineEdit(this);
        m_labelLayout->addWidget(textEdit, j, 1);
        m_edits.append(textEdit);
    }

    // 按钮布局
    m_pushbuttonLayout->addWidget(m_ensure);
    m_pushbuttonLayout->addWidget(m_cancle);

    // 正确设置布局层级
    m_mainLayout->addLayout(m_labelLayout, 1);  // 添加拉伸因子为1，让这部分可以伸缩
    m_mainLayout->addLayout(m_pushbuttonLayout);

    // 设置顶级布局
    this->setLayout(m_mainLayout);

    // 连接信号槽
    connect(m_ensure, &QPushButton::clicked, this, &addSelectWidget::ensureSlot);
    connect(m_cancle, &QPushButton::clicked, [this](){
        emit giveCancle();
        this->close();
    });
}

void addSelectWidget::ensureSlot(){
    QList<QString> results;

    for(int i = 0; i < m_nums; i++){
        QString getText = m_edits[i]->text();
        results.append(getText);
    }

    if(results.contains("")){
        emit giveCancle();
    } else {
        emit giveSelects(m_nums, results);
    }

    this->close();
}
