#include <QApplication>
#include <QDebug>


#include "Controller.h"
#include "Log.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;

    qDebug() << "init success!";

    //controller.m_playProg->playProg();

    return a.exec();
}

//#ifdef Q_OS_WIN32

//qDebug()<<"win32";
//#else
//qDebug()<<"linux";
//#endif

//Q_OS_LINUX
