#include <process.h>
#include <QMessageBox>
#include "receiverwindow.h"
#include "pollingworker.h"
#include "ui_receiverwindow.h"
#include "playback.h"
#include "rs232.h"
#include "huffman.h"
extern "C"
{
#include "TxtMessage.h"
#include "checksum.h"
#include "bst.h"
}



ReceiverWindow::ReceiverWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReceiverWindow)
{
    ui->setupUi(this);

    // connect audio button to playback, not final
    connect(ui->audioBtn, SIGNAL(clicked()), this, SLOT(Playback()));
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(Refresh()));
    connect(ui->archiveBtn, SIGNAL(clicked()), this, SLOT(Archive()));
    connect(ui->readBtn, SIGNAL(clicked()), this, SLOT(UpdateQueueWindow()));
    connect(ui->phoneBookBtn, SIGNAL(clicked()), this, SLOT(DisplayPhonebook()));

    // open RS232 port
    OpenRS232Port();
    displayInbox = 0;
    displayTree = 0;


}

// cleans up the receiver window
ReceiverWindow::~ReceiverWindow()
{
    poller->SetIsFinished(1);
    delete ui;


    // close rs232 port
    CloseRS232Port();

    if (thread->isFinished()){
        delete poller;
        delete thread;
    }
}


void ReceiverWindow::StartPoller()
{
    thread = new QThread;
    poller = new PollingWorker(this->GetBaudRate(), ui->rawModeChk);
    poller->moveToThread(thread);
    connect(poller, SIGNAL(error(QString, int)), this, SLOT(HandleErrors(QString, int)));
    connect(poller, SIGNAL(labelEdit(QString)), this, SLOT(HandleLabelChange(QString)));
    connect(poller, SIGNAL(messageEdit(QString)), this, SLOT(HandleTextChange(QString)));
    connect(poller, SIGNAL(audioReceived(long,char*)), this, SLOT(HandleAudio(long, char*)));
    connect(thread, SIGNAL(started()), poller, SLOT(PollRS232()));
    connect(poller, SIGNAL(finished()), thread, SLOT(quit()));
    connect(poller, SIGNAL(finished()), poller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void ReceiverWindow::HandleTextChange(QString message)
{
    ui->msgTxt->append(message);
    //QMessageBox::information(NULL, "Info", message);
}
void ReceiverWindow::HandleAudio(long audioSize, char* audio)
{
    lBigBufSize = audioSize;
    iBigBuf = (short *)audio;
    QMessageBox::information(NULL,"Audio Broadcast Received", "You have a new audio message, press 'Audio Messages'");
}

void ReceiverWindow::HandleLabelChange(QString message)
{
    // update number of messages
    ui->msgNumLbl->setText(message);
    // clear the text box
    ui->msgTxt->clear();
    // Print all messages in the queue.
    Traverse(head);
}

// displays the error of the polling thread
void ReceiverWindow::HandleErrors(QString error, int code)
{
    QMessageBox::information(NULL, "Error", QString("Message: %1 Code: %2").arg(error, QString::number(code)));
}

void ReceiverWindow::DisplayPhonebook()
{
    ui->msgTxt->clear();
    if (displayTree)
    {
        ui->phoneBookBtn->setText(QString("Phonebook"));
        displayTree = 0;
    }
    else
    {
        displayTree = 1;
        BSTPrint(root);
        ui->phoneBookBtn->setText(QString("Inbox"));
    }
}

void ReceiverWindow::BSTPrint(TreeNode * treeRoot)
{
    if (treeRoot == NULL) return;		// reached leaf
    BSTPrint( treeRoot->pLeft );
    ui->msgTxt->append(QString ("Key: %1\n").arg(QString::number(treeRoot->item->key)));
    BSTPrint( treeRoot->pRight );
    return;
}

// Traverse list and print messages in order.
void ReceiverWindow::Traverse(Msg *h) {
    if ( h == NULL ) return;
    this->PrintTenChars(h);
    Traverse(h->next);
}
// Visit function, simply prints the message at that node.
void ReceiverWindow::PrintTenChars (Msg * msg){
    int i;
    ui->msgTxt->append(QString("%1").arg(QString::number(msg->msgNum)));

    ui->msgTxt->append(QString(msg->txt));

    ui->msgTxt->append(QString("\n"));
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
    //this->SetMsgText(QString("Playing broadcast..."));
    PlayBuffer( iBigBuf, lBigBufSize );
    //this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    free(iBigBuf);
}

// get the baud rate from the combo box
int ReceiverWindow::GetBaudRate()
{
    return ui->baudRateCmb->itemText(ui->baudRateCmb->currentIndex()).toInt();
}
void ReceiverWindow::UpdateQueueWindow()
{
    //
    if (displayInbox)
    {
        ui->msgTxt->clear();
        Traverse(head);
        displayInbox = 0;
        ui->readBtn->setText(QString("Read"));
    }
    else
    {
        Msg *message = DeleteFromQueue();
        ui->msgTxt->setText(QString(message->txt));
        displayInbox = 1;
        ui->readBtn->setText(QString("Inbox"));
        ui->msgNumLbl->setText(QString("Number of Messages: %1").arg(QString::number(numberOfMessages)));
        free (message);
    }
}

// restarts the polling thread if need be
void ReceiverWindow::Refresh()
{
    if(poller->GetBaudRate() != this->GetBaudRate())
        poller->SetBaudRate(this->GetBaudRate());
    if(!thread->isRunning())
        this->StartPoller();
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
