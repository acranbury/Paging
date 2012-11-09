/*  Coded Messaging System - ITCT Semester 3, Project 2
  Authors: Alan Cranbury, Mark Mahony
  October 19, 2012
  Github: www.github.com/acranbury/Paging.git
*/

#include <QtGui/QApplication>
#include <QMessageBox>
#include <cctype>
#include "senderwindow.h"
#include "receiverwindow.h"

// include the multi media library
#pragma comment (lib, "winmm")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SenderWindow s;
    ReceiverWindow r;

    if(argc > 1)
    {
        // get command line arguments
        //if(toupper(*argv[1]) == 'S')
            //s.show();
        if(toupper(*argv[1]) == 'R')
            r.show();
    }
    else
    {
        QMessageBox::information(NULL, "Defaulting...", "Defaulting to Sender");
        r.show();
        r.StartPoller();
    }
    return a.exec();
}
