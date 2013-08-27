QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtVp
TEMPLATE = lib

DEFINES += QTVP_LIBRARY

INCLUDEPATH = include

SOURCES += src/vpruler.cpp \
    src/vpcolor.cpp \
    src/vputil.cpp \
    src/vpcoord.cpp \
    src/vpgc.cpp \
    src/vpviewport.cpp \
    src/vpgraphics2d.cpp \
    src/vpgrid.cpp \
    src/vpgriddialog.cpp \
    src/vpgraphicsview.cpp

HEADERS += include/vpcoord.h \
    include/vpgc.h \
    include/vpviewport.h \
    include/vpgraphics2d.h \
    include/vpgrid.h \
    include/vpgriddialog.h \
    include/vpgraphicsview.h \
    include/vputil.h \
    include/vptypes.h \
    include/vpruler.h \
    include/vpcolor.h \
    include/qtvp_global.h

FORMS   += src/vpgriddialog.ui

RESOURCES +=

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
