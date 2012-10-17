#include "senderwindow.h"
#include "ui_senderwindow.h"
#include "playback.h"

static short *iBigBuf;
static long	 lBigBufSize;	// in samples

SenderWindow::SenderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SenderWindow)
{
    ui->setupUi(this);

    connect(ui->recordBtn, SIGNAL(clicked()), this, SLOT(Record()));
    connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(Playback()));
}

SenderWindow::~SenderWindow()
{
    delete ui;
}

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

void SenderWindow::Playback()
{
    ui->msgTxt->clear();
    this->SetMsgText(QString("Playing broadcast..."));
    PlayBuffer( iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    free(iBigBuf);
}

QString SenderWindow::GetMsgText() const
{
    return ui->msgTxt->toPlainText();
}

void SenderWindow::SetMsgText(QString &text)
{
    ui->msgTxt->setText(text);
}
