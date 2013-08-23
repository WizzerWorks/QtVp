QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtVp
TEMPLATE = app

INCLUDEPATH = include

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/auruler.cpp \
    src/augraphicsview.cpp \
    src/aucolor.cpp \
    src/auutil.cpp \
    src/vpcoord.cpp \
    src/vpgc.cpp \
    src/vpviewport.cpp \
    src/vpgraphics2d.cpp \
    src/vpgrid.cpp \
    src/simpleviewport.cpp \
    src/vpgriddialog.cpp \
    src/vpgraphicsview.cpp

HEADERS += include/mainwindow.h \
    include/auruler.h \
    include/augraphicsview.h \
    include/aucolor.h \
    include/auutil.h \
    include/autypes.h \
    include/vpcoord.h \
    include/vpgc.h \
    include/vpviewport.h \
    include/vpgraphics2d.h \
    include/vpgrid.h \
    include/simpleviewport.h \
    include/vpgriddialog.h \
    include/vpgraphicsview.h

FORMS   += src/mainwindow.ui \
    src/vpgriddialog.ui

RESOURCES += \
    QtVp.qrc
