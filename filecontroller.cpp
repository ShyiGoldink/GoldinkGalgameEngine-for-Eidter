#include "filecontroller.h"


FileController::FileController() {}

bool FileController::deleteFile(const QString &filePath){
    QFile file(filePath);
    if(file.remove()){
        return true;
    }
    else return false;
}
