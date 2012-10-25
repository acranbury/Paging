#include <process.h>
#include <QMessageBox>
#include "receiverwindow.h"
#include "ui_receiverwindow.h"
#include "playback.h"
#include "rs232.h"
#include "TxtMessage.h"

static short *iBigBuf;
static long	 lBigBufSize;	// in samples

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
    _beginthread(this->PollRS232, 0, NULL);
}

ReceiverWindow::~ReceiverWindow()
{
    delete ui;

    // close rs232 port
    CloseRS232Port();
}

void ReceiverWindow::Archive()
{
    if (ui->msgOrderGrp->checkedButton()== ui->fifoRdoBtn){
      //  Msg *newMsg;
    //    newMsg->txt = this->GetMsgText().toAscii().data();
   //     AddToQueue(newMsg);
    }

}

void ReceiverWindow::Playback()
{
    this->SetMsgText(QString("Playing broadcast..."));
    PlayBuffer( iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    free(iBigBuf);
}

int ReceiverWindow::GetBaudRate()
{
    return ui->baudRateCmb->itemText(ui->baudRateCmb->currentIndex()).toInt();
}

void ReceiverWindow::PollRS232(void *dummy)
{

    char readBuf[BUFSIZE] = {0};
    Header * headerBuffer;
    SetUpDCB(this->GetBaudRate());
    long numBytesToGet;
    DWORD dwCommEvent, dwBytesTransferred;
    Msg * newMsg;
    while(1)
    {
        if(portDCB.BaudRate != this->GetBaudRate())
            SetUpDCB(this->GetBaudRate());



        if (!SetCommMask(hComm, EV_RXCHAR))
        {
            QMessageBox::information(NULL, "Error!", "Error setting communications mask.");
            return FALSE;
        }

        if (!WaitCommEvent(hComm, &dwCommEvent, NULL))
        {
            QMessageBox::information(NULL, "Error!", "Error occurred waiting for a character.");
            return FALSE;
        }
        else
        {
            headerBuffer = (Header *)malloc(sizeof(struct Header));
            if(!ReadFile(hComm, (BYTE *)headerBuffer, HEADERSIZE, dwBytesTransferred, 0))
            {
                QMessageBox::information(NULL, "Error!", "Error occurred receiving header.");
                return FALSE;
            }
            if(headerBuffer->lSignature == 0xDEADBEEF)
            {
                numBytesToGet = headerBuffer->lDataLength;
            }
            if(!ReadFile(hComm, readBuf, numBytesToGet, dwBytesTransferred, 0))
            {
                QMessageBox::information(NULL, "Error!", "Error occurred receiving message.");
                return FALSE;
            }

            newMsg = (Msg *)malloc(sizeof(struct message));
            strcpy(newMsg->txt, readBuf);
            newMsg->senderID = rand() % 100;
            newMsg->receiverID = headerBuffer->bReceiverAddr;
            newMsg->msgNum = rand() % 100;
            AddToQueue(newMsg);
        }
    }
}

void ReceiverWindow::Refresh()
{
    SetUpDCB(this->GetBaudRate());
    char readBuf[BUFSIZE] = {0};
    DWORD numBytes = 0;
    SetUpDCB(this->GetBaudRate());
    ReadFromRS232((BYTE *)readBuf, &numBytes);
    this->SetMsgText(QString(readBuf));
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

int ReceiverWindow::GetNumMsgs()
{
    return ui->msgNumLbl->text().toInt();
}

void ReceiverWindow::SetNumMsgs(int numMsgs)
{
    ui->msgNumLbl->setText(QString("Number of Messages: %1").args(numMsgs));
}
