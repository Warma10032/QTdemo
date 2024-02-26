QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dishes.cpp \
    home_tob.cpp \
    income_tob.cpp \
    login.cpp \
    main.cpp \
    mainwindow_tob.cpp \
    mainwindow_toc.cpp \
    menu_tob.cpp \
    oneorder.cpp \
    order_tob.cpp \
    secondwindow_toc.cpp \
    signup.cpp \
    store.cpp \
    thirdwindow_toc.cpp

HEADERS += \
    dishes.h \
    home_tob.h \
    income_tob.h \
    login.h \
    mainwindow_tob.h \
    mainwindow_toc.h \
    menu_tob.h \
    oneorder.h \
    order_tob.h \
    secondwindow_toc.h \
    signup.h \
    store.h \
    thirdwindow_toc.h

FORMS += \
    dishes.ui \
    home_tob.ui \
    income_tob.ui \
    login.ui \
    mainwindow_tob.ui \
    mainwindow_toc.ui \
    menu_tob.ui \
    oneorder.ui \
    order_tob.ui \
    secondwindow_toc.ui \
    signup.ui \
    store.ui \
    thirdwindow_toc.ui

TRANSLATIONS += \
    waimai_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
