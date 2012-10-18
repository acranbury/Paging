#-------------------------------------------------
#
# Project created by QtCreator 2012-10-17T08:39:02
#
#-------------------------------------------------

QT       += core gui

TARGET = Paging
TEMPLATE = app


SOURCES += main.cpp \
    senderwindow.cpp \
    receiverwindow.cpp \
    Playback.cpp \
    heappriority.c \
    rs232.cpp

HEADERS  += senderwindow.h \
    receiverwindow.h \
    playback.h \
    rs232.h

FORMS    += senderwindow.ui \
    receiverwindow.ui

#LIBS    += C:\\Windows\\system32\\winmm.lib
