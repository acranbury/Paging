#include <QMessageBox>
#include "senderwindow.h"
#include "ui_senderwindow.h"
#include "playback.h"
#include "rs232.h"

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

    SetUpDCB(this->GetBaudRate());

    // get the text from the textbox, put it into char array
    QByteArray ba = this->GetMsgText().toAscii();
    textBuf = ba.data();

    // get the number of characters in the array
    numChars = (long)this->GetMsgText().length() + 1;

    if(!WriteToRS232((BYTE *)textBuf, (DWORD *)&numChars))
        QMessageBox::information(NULL, "Error!", "Write to RS232 failed");
}
