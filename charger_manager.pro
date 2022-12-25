QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    global.cpp \
    main.cpp \
    main_menu.cpp \
    mainwindow.cpp

HEADERS += \
    global.h \
    main_menu.h \
    mainwindow.h

FORMS += \
    main_menu.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

# unix:!macx: LIBS += -L$$PWD/libbuild/libevdev/ -levdev

# INCLUDEPATH += $$PWD/lib/libevdev/libevdev
# DEPENDPATH += $$PWD/lib/libevdev/libevdev
