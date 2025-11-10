#include <QGraphicsPixmapItem>
#include <QObject>
#include <QToolTip>
#include <QPainter>

class MovablePixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit MovablePixmapItem(const QPixmap &pixmap, const QString &name = "", QGraphicsItem *parent = nullptr);

    void setObjectName(const QString &name);
    QString objectName() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        // 先绘制原始图片
        QGraphicsPixmapItem::paint(painter, option, widget);

        // 再绘制边框（红色，2像素宽）
        painter->setPen(QPen(Qt::blue, 2));
        painter->drawRect(boundingRect());
    }


signals:
    void positionChanged(const QPointF &oldPos, const QPointF &newPos, const QString &name);
    void show();
    void unshow();
    void selectedCharacterN(const QString& objectName);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)override;

private:
    QString m_name;
    QPointF m_lastPos;
    bool m_isMoving = false;
};
