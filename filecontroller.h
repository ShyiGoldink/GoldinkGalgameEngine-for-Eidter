#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

//专门用于文件管理，例如文件的复制，文件的删除，文件大小的缓存

#include "jsoncontorller.h"

class FileController
{
public:
    FileController();
    bool deleteFile(const QString &filePath);

private:
    JsonContorller jc;
};

#endif // FILECONTROLLER_H
