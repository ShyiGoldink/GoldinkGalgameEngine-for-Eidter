#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

//这是用于展示素材的主界面

#include <QObject>
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
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QToolTip>
#include "Structs.h"
#include "movablepixmapitem.h"


//首次尝试使用抽象类来实现不同类型的转换
class MediaContent : public QWidget
{
    Q_OBJECT
public:
    MediaContent(QWidget *parent = nullptr);
    virtual ~MediaContent() = default;
    virtual void load(const filePaths& file) = 0;
    virtual void unload() = 0;
signals:
    void toPageMain(const QString& objectName);
    void outMessage(QPointF oldPos,QPointF newPos,QString objectName);

};

class ScreenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenWidget(QWidget *parent = nullptr);
    //分为大切换和小切换，当大切换直接更换QList或者是View与否，小切换则直接添加到m_list
    void loadMedias(QList<filePaths> files);//用于切换时加装Meidas的展示页面
    void loadContents(const filePaths &files);//真正用于加装Medias的函数
    void updateMeidas(const filePaths &files);//用于更新素材
    void updateScene(const QString &whatIsThis,const QString &filePath);
    void updateSceneList(QString name,QPointF newPos);//用于更新sccene中某个item的位置
    void unloadMeidas();//用于卸载Meidas的展示页面
    void loadView();//用于加装视图
    void unloadView();//用于卸掉视图
    void prefer();//上一个
    void next();//下一个

signals:
    void updateData(QString filePath,QString whatDo,QString whatBeDone,QVariant data);
    void updateGameMain(QString whatIsThis,float x,float y);//专门用于修改GameMain中某个item的position
    void toPageMain(const QString& objectName);
protected:
    void enterEvent(QEnterEvent *event)override;
    void leaveEvent(QEvent *event)override;

private:
    void setupUi();
    QList<filePaths> m_filePaths;
    bool isMain;//标识符号，用于告知现在是否是需要探查结点
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QVideoWidget *m_videoWidget;
    int m_currentIndex;

    std::unique_ptr<MediaContent> m_currentContent;//使用智能指针管理MeidaConten以便于内存的管理
    QVBoxLayout* m_mainLayout;
    QWidget* m_widget;
};



class ImageContent : public MediaContent
{
public:
    ImageContent(QWidget* parent = nullptr);
    void load(const filePaths& file) override;
    void unload()override;
    void updateImageSize();
protected:
    bool eventFilter(QObject* watched,QEvent* event)override;

private:
    QLabel* m_imageLabel;
    QPixmap m_originalPixmap;
};

class VideoContent : public MediaContent
{
public:
    VideoContent(QWidget* parent = nullptr);
    ~VideoContent();
    void load(const filePaths& file) override;
    void unload()override;
    void updateVideoSize();
protected:
    bool eventFilter(QObject* watched,QEvent* event)override;
private:
    QMediaPlayer* m_player;
    QAudioOutput* m_output;
    QVideoWidget* m_videoWidget;
};

class MusicContent : public MediaContent
{
public:
    MusicContent(QWidget* parent = nullptr);
    ~MusicContent();
    void load(const filePaths& file) override;
    void unload()override;
    void updateMusicSize();
protected:
    bool eventFilter(QObject* watched,QEvent* event)override;
private:
    QMediaPlayer* m_player;
    QAudioOutput* m_output;
    QWidget* m_widget;
};

class PointContent : public MediaContent
{
    Q_OBJECT
public:
    PointContent(QWidget* parent = nullptr);
    ~PointContent();
    void extracted();
    void setVisiable();//设置可见
    void setUnvisiable();//设置为不可见
    void load(const filePaths &file) override;
    void unload()override;

private:
    QWidget *m_widget;
    QGraphicsScene *m_scene;
    QGraphicsView* m_view;
    QList<QPixmap> m_pixmap;

};
#endif // SCREENWIDGET_H
