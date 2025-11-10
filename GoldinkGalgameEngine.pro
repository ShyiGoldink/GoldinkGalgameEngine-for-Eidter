QT       += core gui multimedia multimediawidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addselectwidget.cpp \
    catheing.cpp \
    changechoicewidget.cpp \
    clickablepixmapitem.cpp \
    enteremitlineedit.cpp \
    filecontroller.cpp \
    fontmanager.cpp \
    fontselectorwidget.cpp \
    jsoncontorller.cpp \
    landingpage.cpp \
    main.cpp \
    homepage.cpp \
    meidias.cpp \
    movablepixmapitem.cpp \
    newproject.cpp \
    openproject.cpp \
    opensourceintro.cpp \
    package.cpp \
    pageindex.cpp \
    pageloading.cpp \
    pagemain.cpp \
    recivelabel.cpp \
    screenwidget.cpp \
    selectbox.cpp \
    showmultimedia.cpp \
    tabs.cpp \
    textbutton.cpp \
    txtsearch.cpp \
    workbench.cpp

HEADERS += \
    Structs.h \
    addselectwidget.h \
    catheing.h \
    changechoicewidget.h \
    clickablepixmapitem.h \
    enteremitlineedit.h \
    filecontroller.h \
    fontmanager.h \
    fontselectorwidget.h \
    homepage.h \
    jsoncontorller.h \
    landingpage.h \
    meidias.h \
    movablepixmapitem.h \
    newproject.h \
    openproject.h \
    opensourceintro.h \
    package.h \
    pageindex.h \
    pageloading.h \
    pagemain.h \
    recivelabel.h \
    screenwidget.h \
    selectbox.h \
    showmultimedia.h \
    tabs.h \
    textbutton.h \
    txtsearch.h \
    workbench.h

FORMS += \
    catheing.ui \
    homepage.ui \
    landingpage.ui \
    newproject.ui

TRANSLATIONS += \
    translation_zh.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    GoldinkGalgamrEngine_zh.qm

RESOURCES += \
    source/icons.qrc
