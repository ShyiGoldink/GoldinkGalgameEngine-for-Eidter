#include "package.h"
#include <QDir>
#include <QFileInfo>
#include "catheing.h"
#include "fontmanager.h"

package::package() :
    m_pics(0)
    ,m_videos(0)
    ,m_mps(0)
{
    // 重置计数器，确保每次打包都从0开始
    m_pics = 0;
    m_videos = 0;
    m_mps = 0;
}


//projectPath 是项目的根目录，projectPath/下是我们需要的具体目录，比如jsons等
void package::packageIt(const QString& projectPath, const QString &aimPath){
    Catheing::start();
    QDir dir;

    // 重置计数器
    m_pics = 0;
    m_videos = 0;
    m_mps = 0;

    // 创建完整的目录结构
    dir.mkpath(aimPath+"/jsons");
    dir.mkpath(aimPath+"/codes");
    dir.mkpath(aimPath+"/sources");

    // 初始化镜像映射
    m_mirror.clear();

    // 处理index.json
    QVector<QString> indexWhatHave = jc.getWhatHaveVector(projectPath+"/jsons/index.json");
    QJsonArray save1;
    QJsonObject obj1;
    for(auto name:indexWhatHave){
        QString it = jc.getInJson(projectPath+"/jsons/index.json",name);
        if(!it.isEmpty()){
            save1.append(name);
            // 关键修改：在检查之前先标准化路径
            QString normalizedIt = QDir::cleanPath(it);
            if(!m_mirror.keys().contains(normalizedIt)){
                m_mirror[normalizedIt] = divide(normalizedIt);
            }
            obj1[name] = m_mirror[normalizedIt];
            obj1[name+"X"] = jc.getInJson(projectPath+"/jsons/index.json",name + "X");
            obj1[name+"Y"] = jc.getInJson(projectPath+"/jsons/index.json",name + "Y");
        }
    }
    obj1["WhatHave"] = save1;
    jc.writeJsonObject(aimPath+"/jsons/index.json",obj1);  // 这里会创建文件
    Catheing::step1();

    // 处理oped.json
    QVector<QString> opedWhatHave = jc.getWhatHaveVector(projectPath+"/jsons/oped.json");
    QJsonArray save2;
    QJsonObject obj2;
    for(auto name:opedWhatHave){
        QString it = jc.getInJson(projectPath+"/jsons/oped.json",name);
        if(!it.isEmpty()){
            save2.append(name);
            // 关键修改：在检查之前先标准化路径
            QString normalizedIt = QDir::cleanPath(it);
            if(!m_mirror.keys().contains(normalizedIt)){
                m_mirror[normalizedIt] = divide(normalizedIt);
            }
            obj2[name] = m_mirror[normalizedIt];
            // 添加X和Y坐标字段（如果存在）
            QString xValue = jc.getInJson(projectPath+"/jsons/oped.json",name + "X");
            QString yValue = jc.getInJson(projectPath+"/jsons/oped.json",name + "Y");
            if(!xValue.isEmpty()) obj2[name+"X"] = xValue;
            if(!yValue.isEmpty()) obj2[name+"Y"] = yValue;
        }
    }
    obj2["WhatHave"] = save2;
    jc.writeJsonObject(aimPath+"/jsons/oped.json",obj2);  // 这里会创建文件
    Catheing::step2();

    // 处理game目录下的JSON文件
    QDir gameDir(projectPath+"/jsons/game");
    if (!gameDir.exists()) {
        qWarning() << "game目录不存在:" << projectPath+"/jsons/game";
        Catheing::finish();
        return;
    }

    QStringList jsonFiles = gameDir.entryList(QStringList() << "*.json", QDir::Files);

    // 在目标位置创建game目录
    dir.mkpath(aimPath+"/jsons/game");

    foreach(const QString& jsonFile, jsonFiles){
        QString sourceFilePath = gameDir.absoluteFilePath(jsonFile);
        QString targetFilePath = aimPath + "/jsons/game/" + jsonFile;

        QList<filePaths> saved = jc.loadPageMain(sourceFilePath);
        QJsonArray save3;

        for(filePaths& filepaths : saved){
            for(auto &it : filepaths.viewPoint.messages){
                if(!it.filePath.isEmpty()){
                    // 关键修改：在检查之前先标准化路径
                    QString normalizedPath = QDir::cleanPath(it.filePath);
                    if(!m_mirror.keys().contains(normalizedPath)){
                        m_mirror[normalizedPath] = divide(normalizedPath);
                    }
                    it.filePath = m_mirror[normalizedPath];
                }
            }
            save3.append(jc.filePathsToJson(filepaths));
        }

        // 使用writeJsonArray创建目标JSON文件
        jc.writeJsonArray(targetFilePath, save3);
    }

    Catheing::step3();

    // 处理资源文件 - 先标准化路径映射
    QMap<QString, QString> normalizedMirror;
    for (auto it = m_mirror.begin(); it != m_mirror.end(); ++it) {
        QString normalizedKey = QDir::cleanPath(it.key());
        normalizedMirror[normalizedKey] = it.value();
    }
    m_mirror = normalizedMirror;  // 替换为标准化后的映射

    QDir sourceDir(projectPath + "/source");
    if (!sourceDir.exists()) {
        qWarning() << "sources目录不存在:" << sourceDir.absolutePath();
        Catheing::finish();
        return;
    }

    // 获取sources目录中的所有文件
    QStringList resourceFiles = sourceDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // 创建文件映射表（文件名 -> 标准化完整路径）
    QMap<QString, QString> fileMapping;
    for (auto it = m_mirror.begin(); it != m_mirror.end(); ++it) {
        QString normalizedPath = it.key();
        QFileInfo fileInfo(normalizedPath);
        QString fileName = fileInfo.fileName();
        fileMapping[fileName] = normalizedPath;
    }

    // 创建一个反向映射（标准化路径 -> 重命名后的文件名）
    QMap<QString, QString> reverseMirror;
    for (auto it = m_mirror.begin(); it != m_mirror.end(); ++it) {
        reverseMirror[it.key()] = it.value();
    }

    foreach (const QString& originalFileName, resourceFiles) {
        QString sourceFilePath = sourceDir.absoluteFilePath(originalFileName);

        // 查找该文件在m_mirror中的映射
        QString mappedFileName = originalFileName; // 默认使用原文件名

        // 通过文件映射表查找
        if (fileMapping.contains(originalFileName)) {
            QString originalPath = fileMapping[originalFileName];
            if (reverseMirror.contains(originalPath)) {
                QFileInfo mappedInfo(reverseMirror[originalPath]);
                mappedFileName = mappedInfo.fileName();
            }
        }

        QString targetFilePath = aimPath + "/sources/" + mappedFileName;

        // 确保目标目录存在
        QDir targetDir(aimPath + "/sources");
        if (!targetDir.exists()) {
            targetDir.mkpath(".");
        }

        // 如果目标文件已存在，先删除
        if (QFile::exists(targetFilePath)) {
            QFile::remove(targetFilePath);
        }

        // 复制文件
        if (QFile::copy(sourceFilePath, targetFilePath)) {
            qDebug() << "资源文件复制成功:" << originalFileName << "->" << mappedFileName;
        } else {
            qWarning() << "资源文件复制失败:" << originalFileName << "->" << mappedFileName;
            qWarning() << "源路径:" << sourceFilePath;
            qWarning() << "目标路径:" << targetFilePath;
        }
    }

    QString fontFilePath = FontManager::getCurrentFontFilePath();
    if (!fontFilePath.isEmpty() && QFile::exists(fontFilePath)) {
        QString targetFontPath = aimPath + "/sources/Userfont.TTF";

        // 确保目标目录存在
        QDir targetDir(aimPath + "/sources");
        if (!targetDir.exists()) {
            targetDir.mkpath(".");
        }

        // 如果目标文件已存在，先删除
        if (QFile::exists(targetFontPath)) {
            QFile::remove(targetFontPath);
        }

        // 复制字体文件
        if (QFile::copy(fontFilePath, targetFontPath)) {
            qDebug() << "用户字体文件复制成功:" << fontFilePath << "->" << targetFontPath;

            // 将字体文件路径添加到镜像映射中，以便在JSON中引用
            m_mirror[fontFilePath] = "Userfont.TTF";
        } else {
            qWarning() << "用户字体文件复制失败:" << fontFilePath << "->" << targetFontPath;
        }
    } else {
        qDebug() << "未找到用户字体文件或使用系统默认字体，无需复制";
    }

    Catheing::step4();

    processIconAndExecutable(projectPath, aimPath);

    //最后处理代码
    Catheing::finish();
}


QString package::divide(const QString& filePath){
    // 路径标准化：统一斜杠和大小写
    QString normalizedPath = QDir::cleanPath(filePath);

    QFileInfo fileInfo(normalizedPath);
    QString extension = fileInfo.suffix().toLower(); // 统一小写
    QString baseName;

    if(extension == "jpg" || extension == "png" || extension == "bmp" || extension == "gif" ||
        extension == "jpeg" || extension == "svg" || extension == "ico" || extension == "webp") {
        baseName = "pic" + QString::number(m_pics);
        m_pics = m_pics + 1;
    }
    else if(extension == "mp3" || extension == "wav" || extension == "ogg" || extension == "flac" ||
             extension == "m4a" || extension == "aac") {
        baseName = "audio" + QString::number(m_mps);
        m_mps = m_mps + 1;
    }
    else if(extension == "mp4" || extension == "avi" || extension == "mkv" || extension == "mov" ||
             extension == "wmv" || extension == "flv" || extension == "webm") {
        baseName = "video" + QString::number(m_videos);
        m_videos = m_videos + 1;
    }
    else {
        // 其他类型文件
        baseName = normalizedPath;
        return baseName;
    }

    return baseName + "." + extension;
}

void package::processIconAndExecutable(const QString& projectPath, const QString& aimPath) {
    // 1. 弹出窗口获取图标文件
    QString iconPath = QFileDialog::getOpenFileName(
        nullptr,
        QObject::tr("选择软件图标"),
        QDir::homePath(),
        QObject::tr("图像文件 (*.ico *.png *.jpg *.bmp)")
        );

    if (iconPath.isEmpty()) {
        qDebug() << "用户取消了图标选择，使用默认图标";
    } else {
        qDebug() << "用户选择的图标文件:" << iconPath;
    }

    // 2. 复制 orcode 目录下的所有文件到 aimPath/codes
    QDir orcodeDir(jc.getProjectRootPath() + "/orcode");
    if (!orcodeDir.exists()) {
        qWarning() << "orcode目录不存在:" << orcodeDir.absolutePath();
        return;
    }

    QDir targetCodesDir(aimPath + "/codes");
    if (!targetCodesDir.exists()) {
        targetCodesDir.mkpath(".");
    }

    // 复制 orcode 目录下的所有文件和子目录
    copyDirectory(orcodeDir.absolutePath(), targetCodesDir.absolutePath());

    // 3. 读取 rootFile.txt 获取新的可执行文件名
    QString rootFilePath = projectPath + "/rootFile.txt";
    QString newExeName = "InkCode.exe"; // 默认名称

    QFile rootFile(rootFilePath);
    if (rootFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&rootFile);
        in.readLine(); // 读取第一行（跳过）
        QString secondLine = in.readLine(); // 读取第二行
        if (!secondLine.isEmpty()) {
            newExeName = secondLine.trimmed() + ".exe";
            qDebug() << "新的可执行文件名:" << newExeName;
        }
        rootFile.close();
    } else {
        qWarning() << "无法打开 rootFile.txt:" << rootFilePath;
    }

    // 4. 重命名 InkCode.exe
    QString originalExePath = aimPath + "/codes/InkCode.exe";
    QString newExePath = aimPath + "/codes/" + newExeName;

    if (QFile::exists(originalExePath)) {
        if (QFile::exists(newExePath)) {
            QFile::remove(newExePath);
        }

        if (QFile::rename(originalExePath, newExePath)) {
            qDebug() << "可执行文件重命名成功:" << newExeName;

            // 5. 设置可执行文件图标（如果用户选择了图标）
            if (!iconPath.isEmpty()) {
                if (setExecutableIcon(newExePath, iconPath)) {
                    qDebug() << "图标设置成功";
                } else {
                    qWarning() << "图标设置失败";
                }
            }
        } else {
            qWarning() << "可执行文件重命名失败";
            // 重命名失败，使用原始路径设置图标
            if (!iconPath.isEmpty() && QFile::exists(originalExePath)) {
                setExecutableIcon(originalExePath, iconPath);
            }
        }
    } else {
        qWarning() << "原始可执行文件不存在:" << originalExePath;
    }
}

bool package::copyDirectory(const QString& sourcePath, const QString& destinationPath) {
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) {
        return false;
    }

    QDir destDir(destinationPath);
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }

    // 复制所有文件
    QStringList files = sourceDir.entryList(QDir::Files);
    for (const QString& file : files) {
        QString srcFilePath = sourceDir.absoluteFilePath(file);
        QString destFilePath = destDir.absoluteFilePath(file);

        if (QFile::exists(destFilePath)) {
            QFile::remove(destFilePath);
        }

        if (!QFile::copy(srcFilePath, destFilePath)) {
            qWarning() << "复制文件失败:" << srcFilePath << "->" << destFilePath;
            return false;
        }
    }

    // 递归复制所有子目录
    QStringList subDirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (const QString& subDir : subDirs) {
        QString srcSubDirPath = sourceDir.absoluteFilePath(subDir);
        QString destSubDirPath = destDir.absoluteFilePath(subDir);
        if (!copyDirectory(srcSubDirPath, destSubDirPath)) {
            return false;
        }
    }

    return true;
}

bool package::setExecutableIcon(const QString& exePath, const QString& iconPath) {
#ifdef Q_OS_WIN

    QString rceditPath = jc.getProjectRootPath() + "/rcedit-x64.exe";

    if (QFile::exists(rceditPath)) {
        QStringList arguments;
        arguments << exePath;
        arguments << "--set-icon" << iconPath;

        QProcess process;
        process.start(rceditPath, arguments);
        if (process.waitForFinished(10000) && process.exitCode() == 0) {
            qDebug() << "使用 rcedit 设置图标成功";
            return true;
        } else {
            qWarning() << "rcedit 执行失败，退出码:" << process.exitCode();
        }
    } else {
        qWarning() << "rcedit.exe 未找到在路径:" << rceditPath;
    }

    // 方法2: 使用 Resource Hacker（如果可用）
    QString resourceHackerPath = "ResourceHacker.exe";
    if (QFile::exists(resourceHackerPath)) {
        QStringList arguments;
        arguments << "-open" << exePath;
        arguments << "-save" << exePath + ".temp";
        arguments << "-action" << "delete";
        arguments << "-mask" << "ICONGROUP,,";

        QProcess process;
        process.start(resourceHackerPath, arguments);
        if (process.waitForFinished(10000)) {
            arguments.clear();
            arguments << "-open" << exePath + ".temp";
            arguments << "-save" << exePath;
            arguments << "-action" << "addoverwrite";
            arguments << "-res" << iconPath;
            arguments << "-mask" << "ICONGROUP,MAINICON,";

            process.start(resourceHackerPath, arguments);
            if (process.waitForFinished(10000)) {
                QFile::remove(exePath + ".temp");
                return true;
            }
        }
        QFile::remove(exePath + ".temp");
    }

    // 方法3: 提示用户手动操作
    qWarning() << "无法自动设置图标，请手动设置或安装 rcedit 工具";
    QMessageBox::information(nullptr,
                             QObject::tr("图标设置"),
                             QObject::tr("无法自动设置可执行文件图标。\n"
                                         "建议安装 rcedit 工具或手动设置图标。\n"
                                         "可执行文件路径: %1").arg(exePath));
#endif

    // 非Windows平台或设置失败
    qWarning() << "图标设置功能仅支持Windows平台，或需要安装相关工具";
    return false;
}
