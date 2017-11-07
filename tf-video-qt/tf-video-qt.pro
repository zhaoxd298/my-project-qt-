#-------------------------------------------------
#
# Project created by QtCreator 2017-02-22T11:57:41
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tf-video-qt
TEMPLATE = app


SOURCES +=\
    board/Board.cpp \
    message/Cmd.cpp \
    Controller.cpp \
    display/Display.cpp \
    Main.cpp \
    media/Media.cpp \
    message/Net.cpp \
    play_prog/PlayProg.cpp \
    play_prog/ProgList.cpp \
    message/Usb.cpp \
    TestKey.cpp \
    play_prog/Frame.cpp \
    play_prog/BackGround.cpp \
    media/Animat.cpp

HEADERS  += \
    display/IDisplay.h \
    board/Board.h \
    message/Cmd.h \
    Controller.h \
    display/Display.h \
    media/Media.h \
    message/Net.h \
    play_prog/PlayProg.h \
    play_prog/ProgList.h \
    Types.h \
    message/Usb.h \
    Log.h \
    play_prog/IPlayProg.h \
    TestKey.h \
    play_prog/Frame.h \
    play_prog/BackGround.h \
    media/Animat.h \
    play_prog/PlayStatus.h \
    media/Image.h
