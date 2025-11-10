#include "fontmanager.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFontDatabase>

// 静态成员初始化
QMap<QString, QFont> FontManager::m_maps;
QFont FontManager::m_fontNow;
JsonContorller FontManager::jc;
QMap<QString, QString> FontManager::m_fontPaths;
QString FontManager::m_currentFontName;

FontManager::FontManager()
{
}

void FontManager::init()
{
    // 初始化默认字体（使用Qt系统默认字体）
    m_fontNow = QApplication::font();

    // 优化：只读取一次字体配置
    QMap<QString, QString> fontConfig = jc.getAllFontsFromJson();
    for(const QString& fontName : fontConfig.keys()) {
        QString filePath = fontConfig[fontName];
        addFont(filePath, fontName, false);  // 不重复保存到配置文件
    }

    // 设置用户上次使用的字体
    QString userFontName = jc.getFontUserUse();
    if(!userFontName.isEmpty() && m_maps.contains(userFontName)) {
        m_fontNow = m_maps[userFontName];
        qDebug() << "已恢复用户上次使用的字体:" << userFontName;
    } else {
        qDebug() << "使用系统默认字体:" << m_fontNow.family();
    }
}

void FontManager::addFont(const QString& filePath, const QString& fontName, bool saveToConfig)
{
    if (!QFile::exists(filePath)) {
        qWarning() << "Font file does not exist:" << filePath;
        return;
    }

    int fontId = QFontDatabase::addApplicationFont(filePath);
    if (fontId == -1) {
        qWarning() << "Failed to load font from:" << filePath;
        return;
    }

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (fontFamilies.isEmpty()) {
        qWarning() << "No font families found in:" << filePath;
        return;
    }

    QFont font(fontFamilies.at(0));

    if (saveToConfig) {
        jc.fontFielpathContol(filePath, fontName);
    }

    m_maps[fontName] = font;
    m_fontPaths[fontName] = filePath;  // 保存路径映射

    qDebug() << "Font added successfully:" << fontName << "Family:" << fontFamilies.at(0);
}

QFont FontManager::getFont(const QString& fontName)
{
    if (m_maps.contains(fontName)) {
        return m_maps[fontName];
    } else {
        qWarning() << "Font not found:" << fontName << ", returning default font";
        return m_fontNow;
    }
}

void FontManager::setCurrentFont(const QString& fontName)
{if (m_maps.contains(fontName)) {
        m_fontNow = m_maps[fontName];
        m_currentFontName = fontName;  // 记录当前字体名称
        jc.saveFontUserUse(fontName);
        qDebug() << "当前字体已设置为:" << fontName;
    } else {
        qWarning() << "字体不存在:" << fontName;
    }
}

QStringList FontManager::getAvailableFonts()
{
    return m_maps.keys();
}

QString FontManager::getCurrentFontFilePath()
{
    if (!m_currentFontName.isEmpty() && m_fontPaths.contains(m_currentFontName)) {
        return m_fontPaths[m_currentFontName];
    }
    return QString();  // 返回空字符串表示使用系统默认字体
}

// 新增方法：获取当前字体名称
QString FontManager::getCurrentFontName()
{
    return m_currentFontName;
}
