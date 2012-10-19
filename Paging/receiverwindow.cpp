#include "receiverwindow.h"
#include "ui_receiverwindow.h"
#include "playback.h"
#include "rs232.h"

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

    // open RS232 port
    OpenRS232Port();
}

ReceiverWindow::~ReceiverWindow()
{
    delete ui;

    // close rs232 port
    CloseRS232Port();
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

void ReceiverWindow::Refresh()
{
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
