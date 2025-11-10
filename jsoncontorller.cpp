#include "jsoncontorller.h"

JsonContorller::JsonContorller() {}

QString JsonContorller::getProjectRootPath(){

    QDir dir(QCoreApplication::applicationDirPath());
    while (!dir.exists("rootFile.txt")) {
        if (!dir.cdUp()) {
            return "";
        }
    }
    return dir.absolutePath();

};

void JsonContorller::writeToJson(const QString &filePath,const QString &WhatTo,const QString &writeWhat) {
    QJsonObject root;
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull()) {
            root = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            return;
        }

    } else {
        QMessageBox::warning(nullptr,"warning","文件不存在或无法打开");
        return;
    }


    root[WhatTo] = writeWhat;


    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::warning(nullptr,"warning","无法打开文件进行写操作");
    }
}

QString JsonContorller::getInJson(const QString &filePath,const QString &whatTo) {
    QJsonObject root;
    QFile file(filePath);

    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull()) {
            root = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            return QString();
        }
    } else {
        file.close();
        QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
        return QString();
    }

    if (root.contains(whatTo)) {
        QJsonValue value = root.value(whatTo);
        if (value.isString()) {
            return value.toString();
        } else if (value.isDouble()) {
            return QString::number(value.toDouble());
        } else if (value.isBool()) {
            return value.toBool() ? "true" : "false";
        } else if (value.isNull()) {
            return "null";
        } else if (value.isObject() || value.isArray()) {
            QJsonDocument doc(value.isObject() ? value.toObject() : QJsonValue(value.toArray()).toObject());
            return doc.toJson(QJsonDocument::Compact);
        }
    } else {
        qDebug()<<"wrong in jsoncontroller.cpp at getInJson.";
    }

    return QString(); // 默认返回空字符串
}

void JsonContorller::projectSaveControl(const QString &newFilePath) {
    QDir dir(newFilePath);
    if (dir.isEmpty()) {
        QMessageBox::warning(nullptr, "warning", "项目不存在！");
        return;
    }

    QString jsonPath = getProjectRootPath() + "/ProjectSave.json";
    QJsonObject obj;

    // 读取JSON文件
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull()) {
            obj = doc.object();
        } else {
            QMessageBox::warning(nullptr, "warning", "Json解析失败，可能已损坏");
            return;
        }
    } else {
        QMessageBox::warning(nullptr, "warning", "文件不存在或无法打开");
        return;
    }



    // 如果路径已在save1，直接返回
    if (obj["save1"].toString() == newFilePath) {
        return;
    }

    bool needSave = false;

    // 检查路径是否存在于save2~save6
    for (int i = 6; i >= 2; i--) {
        QString key = "save" + QString::number(i);
        if (obj[key].toString() == newFilePath) {
            // 移动save1~save(i-1)到save2~savei
            for (int j = i; j > 1; j--) {
                obj["save" + QString::number(j)] = obj["save" + QString::number(j - 1)];
            }
            obj["save1"] = newFilePath;
            needSave = true;
            break;
        }
    }

    // 如果是全新路径，整体后移并插入到save1
    if (!needSave) {
        for (int i = 6; i > 1; i--) {
            obj["save" + QString::number(i)] = obj["save" + QString::number(i - 1)];
        }
        obj["save1"] = newFilePath;
        needSave = true;
    }

    // 保存修改
    if (needSave && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
    } else if (needSave) {
        QMessageBox::warning(nullptr, "warning", "无法保存文件");
    }
}

void JsonContorller::userSettingControl(const QString &whatTo,const QString &writeWhat){
    QString jsonPath=getProjectRootPath()+"/UserSetting.json";
    QJsonObject obj;
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            obj = doc.object();
        } else {
            QMessageBox::warning(nullptr,"warning","Json解析失败，可能已损坏");
            file.close();
            return;
        }
        file.close();
    } else {
        file.close();
        QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
        return;
    }
    obj[whatTo]=writeWhat;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    } else {
        file.close();
        QMessageBox::warning(nullptr,"warning","无法打开文件进行写操作");
    }
};

void JsonContorller::projectSaveSearch() {
    QString jsonPath = getProjectRootPath() + "/ProjectSave.json";
    QJsonObject obj;

    // 读取JSON文件
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull()) {
            obj = doc.object();
        } else {
            QMessageBox::warning(nullptr, "warning", "Json解析失败，可能已损坏");
            return;
        }
    } else {
        QMessageBox::warning(nullptr, "warning", "文件不存在或无法打开");
        return;
    }

    // 1. 检查路径是否存在
    for (int i = 1; i <= 6; i++) {
        QString key = "save" + QString::number(i);
        QString path = obj[key].toString();
        if (!path.isEmpty() && !QFile::exists(path)) {
            obj[key] = "";
        }
    }

    // 2. 压缩非空项（类似数组压缩算法）
    int writePos = 1;
    for (int readPos = 1; readPos <= 6; readPos++) {
        QString key = "save" + QString::number(readPos);
        if (!obj[key].toString().isEmpty()) {
            if (writePos != readPos) {
                obj["save" + QString::number(writePos)] = obj[key].toString();
                obj[key] = "";
            }
            writePos++;
        }
    }

    // 3. 将修改写回文件
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();

    } else {
        QMessageBox::warning(nullptr, "warning", "无法保存修改后的JSON文件");
    }
}

QVector<QString> JsonContorller::getWhatHaveVector(const QString &jsonPath){
    QJsonObject root;
    QFile file(jsonPath);
    if(file.exists()&&file.open(QFile::ReadOnly)){
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if(!doc.isNull()){
            root = doc.object();
        }else{
                return {};
        }
    }else{
        file.close();
        QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
        return {};
    }

    QVector<QString> result;
    QJsonArray whatHaveArray = root["WhatHave"].toArray();

    for (const QJsonValue &value : whatHaveArray) {
        if (value.isString()) {
            result.append(value.toString());
        } else {
            QMessageBox::warning(nullptr, "Warning", "'WhatHave' 数组包含非字符串值");
            return {};
        }
    }

    return result;
}

void JsonContorller::mixControl(const QString &filePath, const QString &doWhat, const QString &whatBeDone, QVariant data){
    QFile file(filePath);
    if(file.fileName()=="main.json"){
        ;//由于游戏主体的内容的操作与其它部分不同
    }else{
        QJsonObject root;
        //先将文件改为obj
        if(file.exists()&&file.open(QFile::ReadOnly)){
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            if(!doc.isNull()){
                root = doc.object();
            }else{
                ;
            }
        }else{
            file.close();
            QMessageBox::warning(nullptr,"warning","文件不存在或者无法打开");
        }
        //再依据做什么来进行修改
        if(doWhat == "add"){
            QJsonValue value = changeToJsonValue(data);
            root[whatBeDone] = value;
        }
        else if(doWhat == "updata"){
                if(!root.contains(whatBeDone)){
                ;
                }
                QJsonValue value = changeToJsonValue(data);
                root[whatBeDone] = value;
        }
        else if(doWhat == "delete"){
            if(root.contains(whatBeDone)){
                root.remove(whatBeDone);
            }
        }
        //最后写入文件
        if(file.open(QFile::WriteOnly | QFile::Truncate)){
            QJsonDocument doc(root);
            file.write(doc.toJson());
            file.close();
        }
        else{
            QMessageBox::warning(nullptr, "警告", "无法写入文件");
        }

    }
}

QJsonValue JsonContorller::changeToJsonValue(QVariant variant)
{
    const QMetaType metaType = variant.metaType();
    const QMetaType::Type typeId = static_cast<QMetaType::Type>(metaType.id());

    if (typeId == QMetaType::QStringList) {
        QJsonArray array;
        QStringList list = variant.toStringList();
        for (const QString &str : list) {
            array.append(QJsonValue(str));
        }
        return QJsonValue(array);
    }

    switch (typeId) {
    case QMetaType::Bool:
        return QJsonValue(variant.toBool());
    case QMetaType::Double:
        return QJsonValue(variant.toDouble());
    case QMetaType::Int:
        return QJsonValue(variant.toInt());
    case QMetaType::LongLong:
        return QJsonValue(variant.toLongLong());
    case QMetaType::QString:
        return QJsonValue(variant.toString());
    case QMetaType::QVariantMap: {
        QJsonObject obj;
        QVariantMap map = variant.toMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            obj[it.key()] = changeToJsonValue(it.value());
        }
        return QJsonValue(obj);
    }
    case QMetaType::QVariantList: {
        QJsonArray array;
        QVariantList list = variant.toList();
        for (const auto &value : list) {
            array.append(changeToJsonValue(value));
        }
        return QJsonValue(array);
    }
    default:
        return QJsonValue();
    }
}

QList<filePaths> JsonContorller::loadPageMain(const QString &jsonPath) {
    QJsonArray jsonArray;
    QFile file(jsonPath);

    // 文件检查和读取（优化错误处理）
    if (!file.exists()) {
        QMessageBox::warning(nullptr, "警告", "文件不存在: " + jsonPath);
        return {};
    }

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(nullptr, "警告", "无法打开文件: " + jsonPath);
        return {};
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(nullptr, "警告", "JSON文件格式错误或不是数组");
        return {};
    }

    jsonArray = doc.array();
    QList<filePaths> result;

    for (const QJsonValue& value : jsonArray) {
        if (!value.isObject()) {
            qWarning() << "跳过非对象元素";
            continue;
        }

        QJsonObject obj = value.toObject();
        filePaths filepaths;

        // 使用value()方法提供默认值，避免字段不存在时的问题
        filepaths.filePath = obj.value("filePath").toString();
        filepaths.isView = obj.value("isView").toBool(true);
        filepaths.objectName = obj.value("objectName").toString();

        // 解析viewPoint对象（添加空值检查）
        if (obj.contains("viewPoint") && obj["viewPoint"].isObject()) {
            QJsonObject inner = obj["viewPoint"].toObject();

            // 解析messages数组
            if (inner.contains("messages") && inner["messages"].isArray()) {
                QJsonArray inest = inner["messages"].toArray();
                QList<pointMessages> innerResult;

                for (const QJsonValue& innervalue : inest) {
                    if (innervalue.isObject()) {
                        QJsonObject innerobj = innervalue.toObject();
                        pointMessages inestPointMessages;

                        inestPointMessages.filePath = innerobj.value("filePath").toString();
                        inestPointMessages.inAnimation = innerobj.value("inAnimation").toString("null");
                        inestPointMessages.outAnimation = innerobj.value("outAnimation").toString("null");
                        inestPointMessages.whatisthis = innerobj.value("whatisthis").toString();
                        inestPointMessages.x = innerobj.value("x").toDouble(0.0);
                        inestPointMessages.y = innerobj.value("y").toDouble(0.0);

                        innerResult.append(inestPointMessages);
                    }
                }
                filepaths.viewPoint.messages = innerResult;
            }

            // 解析whatHave数组（修复赋值错误）
            if (inner.contains("whatHave") && inner["whatHave"].isArray()) {
                QJsonArray inest1 = inner["whatHave"].toArray();
                QList<QString> innerString;

                for (const QJsonValue& innervalue : inest1) {
                    if (innervalue.isString()) {
                        innerString.append(innervalue.toString());
                    }
                }
                filepaths.viewPoint.whatHave = innerString;
            }

            // 解析selects数组（修复赋值错误）
            if (inner.contains("selects") && inner["selects"].isArray()) {
                QJsonArray inest2 = inner["selects"].toArray();
                QList<QString> innerString1;

                for (const QJsonValue& innervalue : inest2) {
                    if (innervalue.isString()) {
                        innerString1.append(innervalue.toString());
                    }
                }
                filepaths.viewPoint.selects = innerString1;
            }

            // 解析childId数组
            if (inner.contains("childId") && inner["childId"].isArray()) {
                QJsonArray inest3 = inner["childId"].toArray();
                QList<QString> innerString2;

                for (const QJsonValue& innervalue : inest3) {
                    if (innervalue.isString()) {
                        innerString2.append(innervalue.toString());
                    }
                }
                filepaths.viewPoint.childId = innerString2;
            }

            // 解析其他字段
            filepaths.viewPoint.Cg = inner.value("Cg").toString();
            filepaths.viewPoint.linkNum = inner.value("linkNum").toInt(1);
            filepaths.viewPoint.id = inner.value("id").toString();
            filepaths.viewPoint.isChoices = inner.value("isChoices").toBool(false);
        }

        result.append(filepaths);
    }

    return result;
}

QJsonObject JsonContorller::pointMessagesToJson(const pointMessages& msg) {
    QJsonObject obj;
    obj["whatisthis"] = msg.whatisthis;
    obj["x"] = msg.x;
    obj["y"] = msg.y;
    obj["inAnimation"] = msg.inAnimation; // 补充结构体成员
    obj["outAnimation"] = msg.outAnimation;
    obj["filePath"] = msg.filePath;
    return obj;
}

// 辅助函数：pointMessage转JSON（修正结构体名称）
QJsonObject JsonContorller::pointMessageToJson(const pointMessage& pm) {
    QJsonObject obj;

    // 转换whatHave数组
    QJsonArray whatHaveArray;
    foreach (const QString& item, pm.whatHave) {
        whatHaveArray.append(item);
    }
    obj["whatHave"] = whatHaveArray;

    // 转换messages数组
    QJsonArray messagesArray;
    foreach (const pointMessages& msg, pm.messages) {
        messagesArray.append(pointMessagesToJson(msg));
    }
    obj["messages"] = messagesArray;

    // 转换selects数组
    QJsonArray selectsArray;
    foreach (const QString& select, pm.selects) {
        selectsArray.append(select);
    }
    obj["selects"] = selectsArray;

    // 转换childId数组
    QJsonArray childIdArray;
    foreach (const QString& id, pm.childId) {
        childIdArray.append(id);
    }
    obj["childId"] = childIdArray;

    // 基本字段
    obj["Cg"] = pm.Cg;
    obj["linkNum"] = pm.linkNum;
    obj["id"] = pm.id;
    obj["isChoices"] = pm.isChoices;

    return obj;
}

// 辅助函数：filePaths转JSON
QJsonObject JsonContorller::filePathsToJson(const filePaths& fp) {
    QJsonObject obj;
    obj["isView"] = fp.isView;
    obj["objectName"] = fp.objectName;
    obj["filePath"] = fp.filePath;
    obj["viewPoint"] = pointMessageToJson(fp.viewPoint); // 对应pointMessage类型
    return obj;
}

// 核心保存方法：按id前5字符分组保存
bool JsonContorller::savePageMain(const QString& saveDir, const QList<filePaths>& data) {
    // 检查并创建保存目录
    QDir dir(saveDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        QMessageBox::critical(nullptr, "保存失败", "无法创建目录: " + saveDir);
        return false;
    }

    // 清除目录中已有的json文件（避免残留旧文件）
    QStringList oldFiles = dir.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    foreach (const QString& file, oldFiles) {
        dir.remove(file);
    }

    // 按viewPoint.id前5字符分组
    QMap<QString, QList<filePaths>> groupedData;
    foreach (const filePaths& item, data) {
        if (item.viewPoint.id.isEmpty()) continue;

        QString groupKey = item.viewPoint.id.left(4); // 取前5个字符作为分组键
        groupedData[groupKey].append(item);
    }

    // 保存每个分组到对应的JSON文件
    bool allSuccess = true;
    foreach (const QString& groupKey, groupedData.keys()) {
        QString fileName = groupKey + ".json";
        QString filePath = dir.filePath(fileName);

        // 转换为JSON数组（与main.json格式保持一致）
        QJsonArray jsonArray;
        foreach (const filePaths& item, groupedData[groupKey]) {
            jsonArray.append(filePathsToJson(item));
        }

        // 写入文件（覆盖模式）
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QMessageBox::warning(nullptr, "保存警告", "无法写入文件: " + filePath);
            allSuccess = false;
            continue;
        }

        QJsonDocument doc(jsonArray); // 直接用数组作为根元素（匹配main.json）
        file.write(doc.toJson(QJsonDocument::Indented)); // 格式化输出
        file.close();
    }

    return allSuccess;
}

bool JsonContorller::addFilePathsToMainJson(const QString& mainJsonPath, const filePaths& newData) {
    QFile file(mainJsonPath);
    QJsonArray jsonArray;

    // 如果文件已存在，先读取现有内容
    if (file.exists()) {
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(nullptr, "警告", "无法打开文件: " + mainJsonPath);
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            QMessageBox::warning(nullptr, "警告", "main.json格式错误，将创建新文件");
        } else if (doc.isArray()) {
            jsonArray = doc.array(); // 读取现有数组
        } else {
            QMessageBox::warning(nullptr, "警告", "main.json不是数组格式，将创建新文件");
        }
    }

    // 添加新数据到数组
    jsonArray.append(filePathsToJson(newData));

    // 写入文件（覆盖模式）
    if (!file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "警告", "无法写入文件: " + mainJsonPath);
        return false;
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson(QJsonDocument::Indented)); // 缩进格式，便于阅读
    file.close();

    return true;
}

void JsonContorller::fontFielpathContol(const QString& filePath, const QString& fontName){
    QString jsonPath = getProjectRootPath() + "/UserFont.json";
    QJsonObject obj;

    // 读取现有的JSON文件内容
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull() && doc.isObject()) {
            obj = doc.object();
        }
    }

    // 检查字体名称是否已存在，如果存在会覆盖，不存在会新增
    if (obj.contains(fontName)) {
        qDebug() << "字体名称已存在，将覆盖旧路径:" << fontName;
    } else {
        qDebug() << "添加新字体:" << fontName;
    }

    // 添加或更新字体路径
    obj[fontName] = filePath;

    // 写回文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "字体配置已保存:" << fontName << "->" << filePath;
    } else {
        QMessageBox::warning(nullptr, "警告", "无法保存字体配置文件");
    }
};


// 在jsoncontorller.cpp中实现
QMap<QString, QString> JsonContorller::getAllFontsFromJson()
{
    QMap<QString, QString> fontMap;

    QFile file(getProjectRootPath()+"/UserFont.json");
    if (!file.exists()) {

        return fontMap;
    }

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(nullptr, "警告", "无法打开字体配置文件" );
        return fontMap;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(nullptr, "警告", "字体配置文件格式错误" );
        return fontMap;
    }

    QJsonObject root = doc.object();


    for (auto it = root.begin(); it != root.end(); ++it) {
        QString fontName = it.key();
        QJsonValue value = it.value();

        if (value.isString()) {
            QString filePath = value.toString();
            fontMap[fontName] = filePath;
        } else {
            qWarning() << "Invalid font path for font:" << fontName;
        }
    }


    return fontMap;
}


// 在jsoncontorller.cpp中实现
void JsonContorller::saveFontUserUse(const QString& fontName)
{
    QString jsonPath = getProjectRootPath() + "/UserSetting.json";
    QJsonObject obj;

    // 读取现有的JSON文件内容
    QFile file(jsonPath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isNull() && doc.isObject()) {
            obj = doc.object();
        }
    }

    // 更新或添加当前使用的字体
    obj["currentFont"] = fontName;

    // 写回文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "用户当前字体已保存:" << fontName;
    } else {
        QMessageBox::warning(nullptr, "警告", "无法保存用户字体设置");
    }
}

QString JsonContorller::getFontUserUse()
{
    QString jsonPath = getProjectRootPath() + "/UserSetting.json";
    QJsonObject obj;

    // 读取JSON文件
    QFile file(jsonPath);
    if (!file.exists()) {
        qDebug() << "用户设置文件不存在，返回空字体";
        return QString();
    }

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(nullptr, "警告", "无法打开用户设置文件");
        return QString();
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(nullptr, "警告", "用户设置文件格式错误");
        return QString();
    }

    obj = doc.object();

    // 获取当前字体设置
    if (obj.contains("currentFont") && obj["currentFont"].isString()) {
        QString fontName = obj["currentFont"].toString();
        qDebug() << "获取到用户当前字体:" << fontName;
        return fontName;
    } else {
        qDebug() << "用户未设置当前字体";
        return QString();
    }
}

bool JsonContorller::writeJsonObject(const QString& filePath, const QJsonObject& jsonObject) {
    QFile file(filePath);

    // 确保目录存在
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::warning(nullptr, "警告", "无法创建目录: " + dir.absolutePath());
            return false;
        }
    }

    // 打开文件进行写入
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(nullptr, "警告", "无法打开文件进行写操作: " + filePath);
        return false;
    }

    try {
        // 创建JSON文档并写入文件
        QJsonDocument doc(jsonObject);
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented); // 美化格式，便于阅读

        qint64 bytesWritten = file.write(jsonData);
        file.close();

        if (bytesWritten == -1) {
            QMessageBox::warning(nullptr, "警告", "写入文件失败: " + filePath);
            return false;
        }

        qDebug() << "成功写入JSON文件:" << filePath << "大小:" << bytesWritten << "字节";
        return true;

    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr, "警告", QString("写入JSON时发生异常: %1").arg(e.what()));
        file.close();
        return false;
    }
}

bool JsonContorller::writeJsonArray(const QString& filePath, const QJsonArray& jsonArray) {
    QFile file(filePath);

    // 确保目录存在
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::warning(nullptr, "警告", "无法创建目录: " + dir.absolutePath());
            return false;
        }
    }

    // 打开文件进行写入
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(nullptr, "警告", "无法打开文件进行写操作: " + filePath);
        return false;
    }

    try {
        // 创建JSON文档并写入文件
        QJsonDocument doc(jsonArray);
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented); // 美化格式，便于阅读

        qint64 bytesWritten = file.write(jsonData);
        file.close();

        if (bytesWritten == -1) {
            QMessageBox::warning(nullptr, "警告", "写入文件失败: " + filePath);
            return false;
        }

        qDebug() << "成功写入JSON数组文件:" << filePath << "大小:" << bytesWritten << "字节";
        return true;

    } catch (const std::exception& e) {
        QMessageBox::warning(nullptr, "警告", QString("写入JSON数组时发生异常: %1").arg(e.what()));
        file.close();
        return false;
    }
}
