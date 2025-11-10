#include "clickablepixmapitem.h"

ClickablePixmapItem::ClickablePixmapItem(const QPixmap& pixmap,QGraphicsItem* parent):
    QGraphicsPixmapItem(pixmap,parent)
{
    setFlag(QGraphicsPixmapItem::ItemIsSelectable);
    setFlag(QGraphicsPixmapItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

}

void ClickablePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
       emit mousePress(false);
    }
    else {
        QGraphicsPixmapItem::mousePressEvent(event);
    }
}

void ClickablePixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        emit mousePress(true);
        event->accept(); // 标记事件已处理
    }    else {
        QGraphicsPixmapItem::mouseDoubleClickEvent(event);
    }
}
