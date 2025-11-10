#include "recivelabel.h"
#include "qpainter.h"

ReciveLabel::ReciveLabel(QWidget *parent,QString objectName):
    QLabel(parent)
{   this->setObjectName(objectName);
    this->setText(objectName);
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    m_pixmap = QPixmap(":/new/prefix1/addtion.png");
    this->setMaximumWidth(200);
    this->setStyleSheet("color:white;");
}

void ReciveLabel::dragEnterEvent(QDragEnterEvent* event){
    event->acceptProposedAction();
}

void ReciveLabel::dropEvent(QDropEvent* event){
    const QString path = event->mimeData()->text();
    if(!path.isEmpty()){
        if(objectName() == "op动画"||objectName() == "ed动画"){
            QFileInfo fileInfo(path);
            QString lowerFiletype = fileInfo.suffix().toLower();
            if(lowerFiletype == "mav"||lowerFiletype == "mp4"||lowerFiletype == "avi"){
                emit message(objectName(),path);
                event->acceptProposedAction();
            }else{
                QMessageBox::warning(nullptr,"warning","此类型不匹配");
                return;
            }


        }
        else if(objectName() == "Hover音效"||objectName() == "点击音效"||objectName() == "BGM"||objectName() == "对话"||objectName() =="op音频"||objectName() =="ed音频"){
            QFileInfo fileInfo(path);
            QString lowerFiletype = fileInfo.suffix().toLower();
            if(lowerFiletype == "mp3"||lowerFiletype == "wav"||lowerFiletype == "flac"){
                emit message(objectName(),path);
                event->acceptProposedAction();
            }else{
                QMessageBox::warning(nullptr,"warning","此类型不匹配");
                return;
            }


        }

        else{
            QFileInfo fileInfo(path);
            QString lowerFiletype = fileInfo.suffix().toLower();
            if(lowerFiletype == "png"||lowerFiletype == "jpg"){
                emit message(objectName(),path);
                qDebug()<<"recivelabel:recive pictures"<<objectName()<<path;
                event->acceptProposedAction();
            }else{
                QMessageBox::warning(nullptr,"warning","此类型不匹配");
                return;
            }
        }

    }else{
        event->ignore();
    }
}

void ReciveLabel::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.drawPixmap(rect(),m_pixmap,m_pixmap.rect());

    QLabel::paintEvent(event);

}

void ReciveLabel::resizeEvent(QResizeEvent* event){
    int fontSize = qBound(8, 12 + (width() / 100), 36);
    QFont font = this->font();
    font.setPixelSize(fontSize);
    setFont(font);
    QLabel::resizeEvent(event);
    update();
}
