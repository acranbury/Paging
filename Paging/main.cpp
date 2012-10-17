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
            r.show();
    }
    else
    {
        QMessageBox::information(NULL, "Defaulting...", "Defaulting to Sender");
        s.show();
    }

    return a.exec();
}
