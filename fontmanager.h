#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QString>
#include <QFont>
#include <QMap>
#include "jsoncontorller.h"

class FontManager
{
public:
    FontManager();
    static void init();
    static void addFont(const QString& filePath, const QString& fontName, bool saveToConfig);
    static QFont getFont(const QString& fontName);
    static void setCurrentFont(const QString& fontName);
    static QStringList getAvailableFonts();
    static QString getCurrentFontFilePath();
    static QString getCurrentFontName();

    static QMap<QString, QFont> m_maps;
    static QFont m_fontNow;

private:
    static JsonContorller jc;  // 如果需要的话，或者直接使用静态方法
    static QMap<QString, QString> m_fontPaths;
    static QString m_currentFontName;
};
#endif // FONTMANAGER_H
