#ifndef MEIDIAS_H
#define MEIDIAS_H

//这是用于展示素材的，在List中的每个ITEM都可以被展示，主要是区分一下图片和音乐和视频

#include <QWidget>
#include <QDir>
#include <QPushButton>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMessageBox>
#include <QMouseEvent>
#include <QLabel>



class Meidias : public QWidget {
    Q_OBJECT
public:
    explicit Meidias(QWidget *parent = nullptr, QString filePath = "", int x = 0, int y = 0);
    ~Meidias();

protected:
    void mousePressEvent(QMouseEvent *event) override;


private:
    bool isPicture;
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QVideoWidget *m_videoWidget;
    QLabel *m_imageLabel;

};

#endif // MEIDIAS_H
