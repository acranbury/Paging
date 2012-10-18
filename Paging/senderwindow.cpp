#include <QMessageBox>
#include "senderwindow.h"
#include "ui_senderwindow.h"
#include "playback.h"


static short *iBigBuf;      // audio buffer
static long	 lBigBufSize;	// in samples
static DCB   portDCB;       // com port settings structure

// constructor
SenderWindow::SenderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SenderWindow)
{
    ui->setupUi(this);

    // connect the buttons with their handlers
    connect(ui->recordBtn, SIGNAL(clicked()), this, SLOT(Record()));
    connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(Playback()));
}

// destructor
SenderWindow::~SenderWindow()
{
    delete ui;
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
