#include "movablepixmapitem.h"
#include "qapplication.h"
#include "qgraphicssceneevent.h"
#include "qscreen.h"

MovablePixmapItem::MovablePixmapItem(const QPixmap &pixmap, const QString &name, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent), m_name(name) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

}

void MovablePixmapItem::setObjectName(const QString &name) {
    m_name = name;
}

QString MovablePixmapItem::objectName() const {
    return m_name;
}

void MovablePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_lastPos = pos();
    m_isMoving = true;
    QToolTip::showText(QCursor::pos(),m_name);
    QGraphicsPixmapItem::mousePressEvent(event);
    emit show();
}

void MovablePixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        emit selectedCharacterN(m_name);
    }else{
        QGraphicsPixmapItem::mouseDoubleClickEvent(event);
    }
}

void MovablePixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isMoving) {
        QGraphicsPixmapItem::mouseMoveEvent(event); // 父类处理实际移动
    }

}

void MovablePixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    // 1. 获取场景边界和Item范围

    if (!scene()) {
        QGraphicsPixmapItem::mouseReleaseEvent(event);
        return;
    }

    QScreen *screen = QApplication::primaryScreen();
    if (!screen) {
        qWarning() << "Failed to get primary screen";
        return; // 或采取默认值处理
    }
    float rate = screen->devicePixelRatio();

    const QRectF sceneBoundary(0, 0, 1536/rate, 849/rate);
    qDebug()<<1536/rate<<849/rate<<rate;
    const QRectF itemRect = mapRectToScene(boundingRect());

    qreal alignedX = qFloor(pos().x() / 20) * 20;
    qreal alignedY = qFloor(pos().y() / 20) * 20;

    // 2. 边界检查
    if (alignedX<0||alignedX>1536/rate||alignedY<0||alignedY>849/rate) {
        setPos(m_lastPos); // 越界则复位
    }
    else if (m_isMoving) {
        // 3. 网格对齐计算（20x20网格）
        QPointF alignedPos(alignedX, alignedY);

        // 4. 确保对齐后仍在边界内

        if (alignedX<0||alignedX>1536/rate||alignedY<0||alignedY>849/rate) {
            qDebug()<<alignedX<<alignedY;
            alignedPos = m_lastPos; // 对齐后越界则保持原位置
        }

        // 5. 更新状态并发射信号
        QToolTip::hideText();
        emit positionChanged(m_lastPos, alignedPos, m_name);
        setPos(alignedPos); // 应用对齐位置
        m_lastPos = alignedPos;
        m_isMoving = false;
    }

    emit unshow();
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}
