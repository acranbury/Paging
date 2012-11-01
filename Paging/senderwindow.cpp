#include <QMessageBox>
#include "senderwindow.h"
#include "ui_senderwindow.h"
#include "playback.h"
#include "rs232.h"
extern "C"
{
#include "TxtMessage.h"
#include "poisson.h"
#include "checksum.h"
}
#include <time.h>

static short *iBigBuf;      // audio buffer
static long	 lBigBufSize;	// in samples

// constructor
SenderWindow::SenderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SenderWindow)
{
    ui->setupUi(this);

    // connect the buttons with their handlers
    connect(ui->recordBtn, SIGNAL(clicked()), this, SLOT(Record()));
    connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(Playback()));
    connect(ui->sendMsgBtn, SIGNAL(clicked()), this, SLOT(SendText()));
    connect(ui->poissonBtn, SIGNAL(clicked()), this, SLOT(SendPoisson()));

    // open the rs232 port
    OpenRS232Port();

}

// destructor
SenderWindow::~SenderWindow()
{
    delete ui;

    // close the rs232 port
    CloseRS232Port();
}

// get the current baud rate
int SenderWindow::GetBaudRate()
{
    return QString(ui->baudRateCmb->itemText(ui->baudRateCmb->currentIndex())).toInt();
}

// get the text of the text box
QString SenderWindow::GetMsgText() const
{
    return ui->msgTxt->toPlainText();
}

// set the text of the text box
void SenderWindow::SetMsgText(QString &text)
{
    ui->msgTxt->setText(text);
}

// record the audio buffer
void SenderWindow::Record()
{
    iBigBuf = (short *)malloc(sizeof(short) * g_nSamplesPerSec * RECORD_TIME);
    lBigBufSize = g_nSamplesPerSec * RECORD_TIME;	// in samples

    InitializePlayback();

    ui->msgTxt->clear();
    // Record the special buffer
    InitializeRecording(iBigBuf, lBigBufSize);
    this->SetMsgText(QString("Recording....."));
    RecordBuffer(iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    CloseRecording();
}

// playback the audio buffer
void SenderWindow::Playback()
{
    ui->msgTxt->clear();
    this->SetMsgText(QString("Playing broadcast..."));
    PlayBuffer( iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    free(iBigBuf);
}

// send text via RS232
void SenderWindow::SendText()
{
    char *textBuf;
    long numChars;
    DWORD headerSize;
    headerSize = HEADERSIZE;

    SetUpDCB(this->GetBaudRate());

    // get the text from the textbox, put it into char array
    QByteArray ba = this->GetMsgText().toAscii();
    textBuf = ba.data();

    // get the number of characters in the array
    numChars = (long)this->GetMsgText().length() + 1;

    if (ui->headerChk->isChecked()){

        Header *msgHeader = (Header *)malloc(sizeof(Header));
        // populate header
        msgHeader->lSignature = 0xDEADBEEF;
        msgHeader->bReceiverAddr = 0xFF;
        msgHeader->bVersion = 0;
        msgHeader->lDataLength = numChars;
        msgHeader->bSenderAddr = DEFSENDER; //For now, should change this to variable/ link with button
        msgHeader->lPattern = 0xAA55AA55;


        if(!WriteToRS232((BYTE *)msgHeader, &headerSize)){
            QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
            return;
     }
    }
    if(!WriteToRS232((BYTE *)textBuf, (DWORD *)&numChars))
        QMessageBox::information(NULL, "Error!", "Write data to RS232 failed");
}
void SenderWindow::SendPoisson()
{
    float lambda = LAMBDA;
    int zeroTime;
    char buffer[LENGTH];
    long numChars;
    DWORD headerSize;
    headerSize = HEADERSIZE;

    SetUpDCB(this->GetBaudRate());

    zeroTime = time(NULL);

    Sleep (1000);

    srand(time(NULL));
    while(((time(NULL) - zeroTime)) % 60)
    {
        if(bIsItTimeForAMessage(lambda, (long)(time(NULL) - zeroTime) % 60))
        {
            GetMessageFromFile(buffer, LENGTH);

            // get the number of characters in the array
            numChars = LENGTH;

            if (ui->headerChk->isChecked()){

                Header *msgHeader = (Header *)malloc(sizeof(Header));
                // populate header
                msgHeader->lSignature = 0xDEADBEEF;
                msgHeader->bReceiverAddr = 0xFF;
                msgHeader->bVersion = 0;
                msgHeader->lDataLength = numChars;
                msgHeader->bSenderAddr = DEFSENDER; //For now, should change this to variable/ link with button
                msgHeader->lPattern = 0xAA55AA55;


                if(!WriteToRS232((BYTE *)msgHeader, &headerSize))
                {
                    QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
                    return;
                }
            }
            if(!WriteToRS232((BYTE *)buffer, (DWORD *)&numChars))
                QMessageBox::information(NULL, "Error!", "Write data to RS232 failed");
        }
        Sleep(950);
    }
}
