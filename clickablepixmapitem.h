#ifndef CLICKABLEPIXMAPITEM_H
#define CLICKABLEPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include "Structs.h"

class ClickablePixmapItem :public QObject, public QGraphicsPixmapItem
{   Q_OBJECT
public:
    explicit ClickablePixmapItem(const QPixmap& pixmap,QGraphicsItem* parent =nullptr);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
public slots:
signals:
    void mousePress(bool isRight);
private:
    pointMessage m_message;
};

#endif // CLICKABLEPIXMAPITEM_H
