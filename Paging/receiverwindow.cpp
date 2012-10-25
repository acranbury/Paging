#include <process.h>
#include <QMessageBox>
#include "receiverwindow.h"
#include "pollingworker.h"
#include "ui_receiverwindow.h"
#include "playback.h"
#include "rs232.h"
#include "TxtMessage.h"


ReceiverWindow::ReceiverWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReceiverWindow)
{
    ui->setupUi(this);

    // connect audio button to playback, not final
    connect(ui->audioBtn, SIGNAL(clicked()), this, SLOT(Playback()));
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(Refresh()));
    connect(ui->archiveBtn, SIGNAL(clicked()), this, SLOT(Archive()));

    // open RS232 port
    OpenRS232Port();

    // spin thread for polling rs232
    thread = new QThread;
    poller = new PollingWorker(this->GetBaudRate(), ui->msgNumLbl);
    poller->moveToThread(thread);
    connect(thread, SIGNAL(started()), poller, SLOT(PollRS232()));
    connect(poller, SIGNAL(finished()), thread, SLOT(quit()));
    connect(poller, SIGNAL(finished()), poller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

// cleans up the receiver window
ReceiverWindow::~ReceiverWindow()
{
    delete ui;

    // stop the thread from polling rs232
    thread->~QThread();
    poller->~PollingWorker();
    // close rs232 port
    CloseRS232Port();

}

// saves the message on the top of the current queue to a file
void ReceiverWindow::Archive()
{
    if (ui->msgOrderGrp->checkedButton()== ui->fifoRdoBtn){
      //  Msg *newMsg;
    //    newMsg->txt = this->GetMsgText().toAscii().data();
   //     AddToQueue(newMsg);
    }

}

// plays back the audio buffer stored in iBigBuf
void ReceiverWindow::Playback()
{
    this->SetMsgText(QString("Playing broadcast..."));
    PlayBuffer( iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    free(iBigBuf);
}

// get the baud rate from the combo box
int ReceiverWindow::GetBaudRate()
{
    return ui->baudRateCmb->itemText(ui->baudRateCmb->currentIndex()).toInt();
}

// restarts the polling thread if need be
void ReceiverWindow::Refresh()
{
    if(poller->GetBaudRate() != this->GetBaudRate())
        poller->SetBaudRate(this->GetBaudRate());
    if(!thread->isRunning())
        poller->moveToThread(thread);
        connect(thread, SIGNAL(started()), poller, SLOT(PollRS232()));
        connect(poller, SIGNAL(finished()), thread, SLOT(quit()));
        connect(poller, SIGNAL(finished()), poller, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
}

// gets the text of the text box
QString ReceiverWindow::GetMsgText() const
{
    return ui->msgTxt->toPlainText();
}

// sets the text of the text box
void ReceiverWindow::SetMsgText(QString &text)
{
    ui->msgTxt->setText(text);
}


// sets the text of how many messages we have received
void ReceiverWindow::SetNumMsgs(int numMsgs)
{
    ui->msgNumLbl->setText(QString("Number of Messages: %1").arg(numMsgs));
}
