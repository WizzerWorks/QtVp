QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtVp
TEMPLATE = app

INCLUDEPATH = include

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/vpruler.cpp \
    src/vpcolor.cpp \
    src/vputil.cpp \
    src/vpcoord.cpp \
    src/vpgc.cpp \
    src/vpviewport.cpp \
    src/vpgraphics2d.cpp \
    src/vpgrid.cpp \
    src/vpgriddialog.cpp \
    src/vpgraphicsview.cpp

HEADERS += include/mainwindow.h \
    include/vpcoord.h \
    include/vpgc.h \
    include/vpviewport.h \
    include/vpgraphics2d.h \
    include/vpgrid.h \
    include/vpgriddialog.h \
    include/vpgraphicsview.h \
    include/vputil.h \
    include/vptypes.h \
    include/vpruler.h \
    include/vpcolor.h

FORMS   += src/mainwindow.ui \
    src/vpgriddialog.ui

RESOURCES += \
    QtVp.qrc
