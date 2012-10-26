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

#pragma comment (lib, "winmm")



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SenderWindow s;
    ReceiverWindow r;

    if(argc > 1)
    {
        if(toupper(*argv[1]) == 'S')
            s.show();
        if(toupper(*argv[1]) == 'R')
        {
            r.show();

        }
    }
    else
    {
        QMessageBox::information(NULL, "Defaulting...", "Defaulting to Sender");
        r.show();
        r.StartPoller();
    }
    /*if(r.isVisible())
    {
        QThread *thread = new QThread;
        PollingWorker *poller = new PollingWorker(r.GetBaudRate(), r.pmsgLabel, r.ptextEdit, r.pRawData);
        poller->moveToThread(thread);
        thread->connect(thread, SIGNAL(started()), poller, SLOT(PollRS232()), Qt::QueuedConnection);
        poller->connect(poller, SIGNAL(finished()), thread, SLOT(quit()), Qt::QueuedConnection);
        poller->connect(poller, SIGNAL(finished()), poller, SLOT(deleteLater()), Qt::QueuedConnection);
        thread->connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()), Qt::QueuedConnection);
        thread->start();
    }*/
    return a.exec();
}
