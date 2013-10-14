#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T11:45:54
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebKugou
TEMPLATE = app


SOURCES += main.cpp \
    webkugouwidget.cpp \
    aboutdialog.cpp

HEADERS  += \
    webkugouwidget.h \
    aboutdialog.h

RESOURCES += \
    icon.qrc
RC_FILE+=\
    info.rc
OTHER_FILES += \
    info.rc
