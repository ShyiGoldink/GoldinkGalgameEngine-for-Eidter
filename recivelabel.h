#ifndef RECIVELABEL_H
#define RECIVELABEL_H

//这是一个可以接受拖动来实现获取信息的Label，继承了旧版本的思想

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPixmap>
#include <QFileInfo>
#include <QMessageBox>


class ReciveLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ReciveLabel(QWidget* parent = nullptr,QString objectName = "");
signals:
    void message(QString whatTo,const QString &path);
protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event)override;
    void paintEvent(QPaintEvent* event)override;
    void resizeEvent(QResizeEvent* event)override;
private:
    QPixmap m_pixmap;
};

#endif // RECIVELABEL_H
