#include "showmultimedia.h"
#include "meidias.h"

ShowMultimediaList::ShowMultimediaList(QWidget *parent)
    :QListWidget(parent)
{

    setUniformItemSizes(true);//确定每一个项目的高度都相同，便于加载每一行，提高运行速度
    setDragEnabled(true);//使其中的文件可以拖拽
    setStyle("pinkBlue");//默认是粉蓝风格

    //实现点击时的素材显示
    connect(this,&QListWidget::itemClicked,this,&ShowMultimediaList::showSlot);

    //通过修改上下文策略来实现右键调用showMenu函数
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&QListWidget::customContextMenuRequested,this,&ShowMultimediaList::showMenu);

}

//析构函数，防止内存泄露
ShowMultimediaList::~ShowMultimediaList(){
    ;
}

void ShowMultimediaList::setProjectFilePath(const QString &filePath){
    projectFilePath = filePath;
    key =QString(QCryptographicHash::hash(filePath.toUtf8(), QCryptographicHash::Md5).toHex());
}
void ShowMultimediaList::loadItem(){

    QSettings settings;
    QStringList list = settings.value(key).toStringList();

    clear();

    for(const QString &list : list){
        if(QFile::exists(list)){
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(list).fileName());
            item->setData(Qt::UserRole,list);
            addItem(item);
        }
    }
}
void ShowMultimediaList::setStyle(QString witchStyle){
    if(witchStyle=="blackWhite"){
        this->setStyleSheet(
            "background-color:#fcfcfc;"
            "color:black;"
            );
    }
    else if(witchStyle=="pinkBlue"){
        this->setStyleSheet(
            "background-color: #f3feff;"
            "color: rgb(83, 83, 255);"
            );
    }

}
void ShowMultimediaList::showSlot(QListWidgetItem *item){
    QVariant data = item->data(Qt::UserRole);
    QPoint globalCursorPos = QCursor::pos();//获取鼠标的坐标用于后续窗口的显示
    QString filePath = data.value<QString>();

    Meidias *me = new Meidias(nullptr, filePath, globalCursorPos.x(), globalCursorPos.y());
    me->show();
    me->setAttribute(Qt::WA_DeleteOnClose);
}

void ShowMultimediaList::showMenu(const QPoint &point) {

    QMenu* menu = new QMenu(nullptr);
    QAction* upItem= menu->addAction("上移");
    QAction* downItem = menu->addAction("下移");
    QAction* topItem = menu->addAction("置顶");
    QAction* bottomItem = menu->addAction("置底");
    QAction* deleteItem = menu->addAction("删除");
    QAction* cancleItem = menu->addAction("取消");

    QListWidgetItem *item = itemAt(point);
    if (!item) return;

    // 动态启用/禁用动作（实时检查）
    upItem->setEnabled(row(item) > 0);
    downItem->setEnabled(row(item) < count() - 1);
    int clickedRow = row(item);

    // 删除操作
    connect(deleteItem, &QAction::triggered, this, [this, clickedRow]() {
        if (auto* item = this->item(clickedRow)) {
            QFile::remove(item->data(Qt::UserRole).toString());
            delete item;
        }
    });

    // 置顶操作
    connect(topItem, &QAction::triggered, this, [this, clickedRow]() {
        if (clickedRow > 0 && clickedRow < count()) {
            if (auto* item = takeItem(clickedRow)) {
                insertItem(0, item);
                setCurrentRow(0);
            }
        }
    });

    // 置底操作
    connect(bottomItem, &QAction::triggered, this, [this, clickedRow]() {
        if (clickedRow < count() - 1) {
            if (auto* item = takeItem(clickedRow)) {
                insertItem(count() - 1, item);
                setCurrentRow(count() - 1);
            }
        }
    });

    // 上移操作
    connect(upItem, &QAction::triggered, this, [this, clickedRow]() {
        if (clickedRow > 0) {
            if (auto* item = takeItem(clickedRow)) {
                insertItem(clickedRow - 1, item);
            }
        }
    });

    // 下移操作
    connect(downItem, &QAction::triggered, this, [this, clickedRow]() {
        if (clickedRow < count() - 1) {
            if (auto* item = takeItem(clickedRow)) {
                insertItem(clickedRow+1, item);
            }
        }
    });

    menu->exec(viewport()->mapToGlobal(point));
}

void ShowMultimediaList::saveItem(){

    QStringList list;
    for(int i=0;i<count();i++){
        list<<item(i)->data(Qt::UserRole).toString();
    }
    QSettings settings;
    settings.setValue(key,list);
    settings.sync();

    if (!settings.isWritable()) {
        qWarning() << "QSettings is not writable!";
    }
}

void ShowMultimediaList::importItem(const QString &filePath) {
    QString fileName = QFileInfo(filePath).fileName();
    QString targetFilePath = projectFilePath + "/source/" + fileName;

    // 检查是否已存在相同文件名的item（使用目标路径检查）
    for (int i = 0; i < count(); i++) {
        if (item(i)->data(Qt::UserRole).toString() == targetFilePath) {
            return; // 已存在，直接返回
        }
    }


    QDir sourceDir(projectFilePath + "/source");
    if (!sourceDir.exists()) {
        if (!sourceDir.mkpath(".")) {
            qWarning() << "无法创建source目录:" << sourceDir.absolutePath();
            return;
        }
    }


    if (sourceDir.exists(fileName)) {
        QListWidgetItem *newItem = new QListWidgetItem(fileName);
        newItem->setData(Qt::UserRole,projectFilePath + "/source/" +fileName );
        addItem(newItem);
        return;
    }


    if (!QFile::copy(filePath, targetFilePath)) {
        return;
    }


    QListWidgetItem *newItem = new QListWidgetItem(fileName);
    newItem->setData(Qt::UserRole, targetFilePath);  // 存储复制后的路径
    addItem(newItem);


}

void ShowMultimediaList::startDrag(Qt::DropActions supportedActions)
{
    QListWidgetItem *item = currentItem();
    if (!item) return;

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(item->data(Qt::UserRole).toString());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(supportedActions);
}
