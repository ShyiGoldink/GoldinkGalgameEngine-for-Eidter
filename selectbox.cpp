#include "selectbox.h"

SelectBox::SelectBox(QWidget *parent,const QVector<QString> &vector,const QVector<QString> &haveClicked)
    : QWidget{parent}
    ,m_vector(vector)
    ,m_haveClicked(haveClicked)
    ,ensure(new QPushButton(tr("确定")))
    ,cancle(new QPushButton(tr("取消")))
    ,inandout(new QPushButton(">>"))
    ,centerWidget(new QWidget(this))
{

    this->resize(parent->width() * 0.4, parent->height());
    centerWidget->resize(parent->width(), parent->height());
    setupUI();
}

void SelectBox::setupUI(){

    for(const QString &string:m_haveClicked){
        m_list.append(string);
    }

    inandout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    inandout->setFixedWidth(this->width()*0.75);

    lock = true;
    connect(inandout,&QPushButton::clicked,[this](){
        if(lock){
            emit in();
            lock = false;
            inandout -> setText("<<");
        }
        else{
            emit out();
            lock = true;
            inandout -> setText(">>");
            }

    });

    QVBoxLayout *buttons = new QVBoxLayout();
    buttons->addWidget(ensure);
    connect(ensure,&QPushButton::clicked,[this](){
        emit outMessage(m_list);
        qDebug()<<"signal"<<m_list;
    });

    buttons->addWidget(cancle);
    connect(cancle,&QPushButton::clicked,[this](){
        lock = true;
        inandout -> setText(">>");
        emit out();
    });

    int i = m_vector.count();
    if(i <= 4&&i>0){
        QVBoxLayout *selects = new QVBoxLayout();
        for(int j=0;j<i;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);

            if(m_haveClicked.contains(m_vector[j])){
                checkbox->setChecked(true);
            }

            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects -> addWidget(checkbox);
        }
        QHBoxLayout *mainLayout = new QHBoxLayout(centerWidget);
        mainLayout->addLayout(selects,21);
        mainLayout->addLayout(buttons,6);
        mainLayout->addWidget(inandout,2);
    }
    else if(i<=8){
        QVBoxLayout *selects1 = new QVBoxLayout();
        QVBoxLayout *selects2 = new QVBoxLayout();
        for(int j=0;j<4;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);
            if(m_haveClicked.contains(m_vector[j])){
                checkbox->setChecked(true);
            }
            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects1 -> addWidget(checkbox);
        }
        for(int j=4;j<i;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);
            if(m_haveClicked.contains(m_vector[j])){
                checkbox->setChecked(true);
            }
            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects2 -> addWidget(checkbox);
        }
        QHBoxLayout *mainLayout = new QHBoxLayout(centerWidget);
        mainLayout->addLayout(selects1,9);
        mainLayout->addLayout(selects2,12);
        mainLayout->addLayout(buttons,6);
        mainLayout->addWidget(inandout,2);
    }
    else{
        QVBoxLayout *selects1 = new QVBoxLayout();
        QVBoxLayout *selects2 = new QVBoxLayout();
        QVBoxLayout *selects3 = new QVBoxLayout();
        for(int j=0;j<5;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);
            if(m_haveClicked.contains(m_vector[j])){
                    checkbox->setChecked(true);            }
            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects1 -> addWidget(checkbox);
        }
        for(int j=5;j<10;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);
            if(m_haveClicked.contains(m_vector[j])){
                checkbox->setChecked(true);
            }
            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects2 -> addWidget(checkbox);
        }
        for(int j=10;j<i;j++){
            QCheckBox *checkbox = new QCheckBox(m_vector[j]);
            if(m_haveClicked.contains(m_vector[j])){
               checkbox->setChecked(true);
            }
            connect(checkbox,&QCheckBox::toggled,[this, j](bool toggled){
                if(toggled){
                    if(m_list.contains(m_vector[j])){
                        ;
                    }
                    else{
                        m_list.append(m_vector[j]);
                    }
                }
                else{
                    if(m_list.contains(m_vector[j])){
                        m_list.removeOne(m_vector[j]);
                    }
                    else{
                        ;
                    }
                }

            });
            selects3 -> addWidget(checkbox);
        }
        QHBoxLayout *mainLayout = new QHBoxLayout(centerWidget);
        mainLayout->addLayout(selects1,6);
        mainLayout->addLayout(selects2,6);
        mainLayout->addLayout(selects3,9);
        mainLayout->addLayout(buttons,6);
        mainLayout->addWidget(inandout,2);
    }
}


void SelectBox::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->resize(parentWidget()->width()*0.4,parentWidget()->height());
    centerWidget->resize(this->width(),this->height());
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    inandout->resize(this->width()*0.2,this->height());

}

void SelectBox::ensureSlot(){
    emit outMessage(m_vector);
}

void SelectBox::cancleSlot(){
    emit out();
}
