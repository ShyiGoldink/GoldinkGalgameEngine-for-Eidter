
/*
 * This software uses:
 * - SDL2 (zlib License)
 * - VLC (GPLv2/LGPLv2.1)
 * - Qt (LGPLv3)
 * - JSON for Modern C++ (MIT License)
 *
 * See LICENSE file for details.
 */


#include "homepage.h"
#include "fontmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("GoldinkWorkroom");
    QCoreApplication::setApplicationName("GoldinkGalgameEngine");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "GoldinkGalgameEngine_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    FontManager::init();

    HomePage w;
    w.setWindowTitle("GoldinkGalgameEngine");
    w.showMaximized();


    return a.exec();
}
