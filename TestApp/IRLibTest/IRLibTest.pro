#-------------------------------------------------
#
# Project created by QtCreator 2017-07-10T00:15:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRLibTest
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../IRLib/Remotes/RmtAux1.cpp \
    ../../IRLib/Remotes/RmtChigo1.cpp \
    ../../IRLib/Remotes/RmtLG1.cpp \
    ../../IRLib/Remotes/RmtTcl1.cpp \
    ../../IRLib/Remotes/RmtChigo2.cpp \
    ../../IRLib/Common.cpp \
    ../../IRLib/Decoder.cpp \
    ../../IRLib/Encoder.cpp \
    ../../IRLib/RemoteFinder.cpp

HEADERS  += mainwindow.h \
    ../../IRLib/Remotes/RmtAux1.h \
    ../../IRLib/Remotes/RmtChigo1.h \
    ../../IRLib/Remotes/RmtLG1.h \
    ../../IRLib/Remotes/RmtTcl1.h \
    ../../IRLib/Remotes/RmtChigo2.h \
    ../../IRLib/Common.h \
    ../../IRLib/Decoder.h \
    ../../IRLib/Encoder.h \
    ../../IRLib/json.hpp \
    ../../IRLib/RemoteFinder.h

FORMS    += mainwindow.ui
