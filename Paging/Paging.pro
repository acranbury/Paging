#-------------------------------------------------
#
# Project created by QtCreator 2012-10-17T08:39:02
#
#-------------------------------------------------

QT       += core gui

TARGET = Paging
TEMPLATE = app
#CONFIG += debug
#CONFIG += console

SOURCES += main.cpp \
    senderwindow.cpp \
    receiverwindow.cpp \
    Playback.cpp \
    rs232.cpp \
    TxtMessage.c \
    pollingworker.cpp \
    poisson.c \
    checksum.c \
    huffman.c \
    bst.c


HEADERS  += senderwindow.h \
    receiverwindow.h \
    playback.h \
    rs232.h \
    TxtMessage.h \
    pollingworker.h \
    poisson.h \
    checksum.h \
    huffman.h \
    bst.h


FORMS    += senderwindow.ui \
    receiverwindow.ui
