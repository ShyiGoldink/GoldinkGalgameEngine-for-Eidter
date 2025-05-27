QT       += core gui multimedia multimediawidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    catheing.cpp \
    jsoncontorller.cpp \
    landingpage.cpp \
    main.cpp \
    homepage.cpp \
    newproject.cpp \
    openproject.cpp \
    opensourceintro.cpp \
    textbutton.cpp

HEADERS += \
    catheing.h \
    homepage.h \
    jsoncontorller.h \
    landingpage.h \
    newproject.h \
    openproject.h \
    opensourceintro.h \
    textbutton.h

FORMS += \
    catheing.ui \
    homepage.ui \
    landingpage.ui \
    newproject.ui \
    opensourceintro.ui

TRANSLATIONS += \
    GoldinkGalgameEngine_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    source/icons.qrc
