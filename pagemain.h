#ifndef PAGEMAIN_H
#define PAGEMAIN_H

//这是用于控制游戏内容的页面

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QMap>
#include <QGraphicsView>
#include <QMenu>
#include <QRandomGenerator>
#include <QColor>
#include <QVector>
#include <QInputDialog>
#include "clickablepixmapitem.h"
#include "recivelabel.h"
#include "selectbox.h"
#include "jsoncontorller.h"
#include "Structs.h"
#include "addselectwidget.h"

class ChangeChoiceWidget;

class PageMain : public QWidget
{
    Q_OBJECT
public:
    explicit PageMain(QWidget *parent = nullptr,QString loadjsonPath = "");
    QList<filePaths> getMFilePaths();
    QString generateUniqueId(const QString& baseId);
    filePaths createNewNode(const QString& parentId);
    void changeXY(QString objectName,float x,float y);
    void onSaveTriggered();
    void updateCharacterAnimation(const QString& witchCharacter);
signals:
    void updateMeida(QList<QString> dataList);
    void updateData(QString filePath,QString whatDo,QString whatBeDone,QVariant dataList);
    void updataScreen(filePaths filepath);
    void updatePoint(filePaths filepath);
    void updateText(QString texts);
private:
    void inAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group,QWidget *widget);
    void outAnimationSlot(QPropertyAnimation *animation,QParallelAnimationGroup *group);
    void setUpLabels(QList<QString> labelNames);
    void safeClearLayout(QLayout* layout);
    QVector<QColor> initColors(int n);
    void initMap();//建立映射
    void initPoint(const QString& nodeId,float x,float y,int color);
    int initPointPrefer(const QString& nodeId);
    void initPointSlot();
    void createPointMiddle(float x,float y,const QString& nodeId);
    void initLine(float xi,float yi,float x,float y,int i);
    void initScene();
    filePaths newFilePaths(const QString& lastNodeId );
    void addPointSlot();
    void deletePointSlot();
    void addSelectsSlot();
    void addSelectsMiddle(int i,QList<QString> selects);
    void changeChoiceSlot();
    void changeChoiceMiddle(int i,QList<QString> selects,QList<QString> id);
    void changeWhatHave(QList<QString> whatHave);
    void setCg();
    void deleteCg();
    void setMessages(pointMessages msg);
    void setSceneAnimation(const QString& animationName);
    void controlAnimation();
    void controlCharacterAnimation(const QString& animationName);




    QString m_loadjsonPath;
    QPropertyAnimation *inAnimation;
    QParallelAnimationGroup *m_group;
    SelectBox *selects;
    JsonContorller jc;
    QHBoxLayout *mainLayout;
    QHBoxLayout *labelsLayout;
    QHBoxLayout *sceneLayout;
    QList<ReciveLabel*> m_labels;
    QList<filePaths> m_filePaths;//存储的本地的filePaths，修改之后便于重新加载
    filePaths m_filePath;//正在修改的m_filePaths
    QMap<QString,filePaths> m_filePathsMap;
    QMap<QString,ClickablePixmapItem*> m_itemMap;
    QMap<QString,tree> m_tree;
    QMap<QString,int>m_divide;
    QList<QString>m_ids;
    QVector<QColor>m_colors;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QPixmap m_pixmap;
    QGraphicsPixmapItem *m_pixmapCItem;
    bool havePixampCItem;
    QString m_IdNow;

    QMenu *m_menu;
    QAction* addPoint;
    QAction* deletePoint;
    QAction* addSelects;
    QAction* changeChoice;
    QAction* viewPoint;
    QAction* deleteView;
    QAction* addAnimation;

    QMenu *m_animationMenu;
    QAction *fadeInAction;
    QAction *fadeOutAction;
    QAction *clearAnimation;

    QMenu *m_animationCharacter;
    QAction *in;
    QAction *out;
    QMenu *m_animationCharacterIn;
    QMenu *m_animationCharacterOut;
    QAction *leftIn;
    QAction *rightIn;
    QAction *noIn;
    QAction *leftOut;
    QAction *rightOut;
    QAction *noOut;

    QList<mainSave> m_saves;
    bool m_setCging;
    QString m_cgId;
    QString m_selectedCharacter;
};

#endif // PAGEMAIN_H
