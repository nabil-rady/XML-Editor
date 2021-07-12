QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lib/Graph.cpp \
    lib/check.cpp \
    lib/fix_xml.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    lib/Graph.hpp \
    lib/check.hpp \
    lib/fix_xml.hpp \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \
    resource.qrc

DISTFILES += \
    Redo.ico \
    Undo.ico \
    fix-errors-on-my-pc.jpg \
    json.ico \
    openfile.ico \
    paste.ico \
    quit.ico
