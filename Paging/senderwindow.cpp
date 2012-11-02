#include <QMessageBox>
#include <QInputDialog>
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
#include "huffman.h"

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
    connect(ui->sendAudioBtn, SIGNAL(clicked()), this, SLOT(SendVoice()));
    connect(ui->addSenderBtn, SIGNAL(clicked()), this, SLOT(AddSender()));

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

// adds a sender id to the combo box
void SenderWindow::AddSender()
{
    // pop up a dialog to add a sender id
    bool ok;
    QString sender = QInputDialog::getText(NULL,"Add Sender", "Enter new Sender Id: ", QLineEdit::Normal, " ", &ok );

    if(ok && !sender.isEmpty())
        ui->senderCmb->addItem(sender);
}

// record the audio buffer
void SenderWindow::Record()
{
    g_nSamplesPerSec = ui->sampleCmb->currentText().toInt();
    iBigBuf = (short *)malloc(sizeof(short) * g_nSamplesPerSec * RECORD_TIME);
    lBigBufSize = g_nSamplesPerSec * RECORD_TIME;	// in samples
    if (iBigBuf == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloc has failed.");
    }

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
    InitializePlayback();
    PlayBuffer( iBigBuf, lBigBufSize );
    this->SetMsgText(QString("%1\nDone.").arg(this->GetMsgText()));
    ClosePlayback();
    //free(iBigBuf);
}

// send text via RS232
void SenderWindow::SendText()
{
    char *textBuf;
    char *inBuf;
    long numChars;
    DWORD headerSize;
    headerSize = HEADERSIZE;
    int datasize;

    SetUpDCB(this->GetBaudRate());

    // get the text from the textbox, put it into char array
    QByteArray ba = this->GetMsgText().toAscii();
    inBuf = ba.data();
    textBuf = ba.data();


    // get the number of characters in the array
    numChars = (long)this->GetMsgText().length() + 1;
    datasize = numChars;



    if (ui->headerChk->isChecked()){



        Header *msgHeader = (Header *)malloc(sizeof(Header));
        msgHeader->bVersion = 0;
        if (msgHeader == NULL)
        {
            QMessageBox::information(NULL, "Error!", "Malloc of msgHeader has failed.");
        }

        // If huffman encoding has been selected, encode it.
        if (!(QString::compare("huffman", ui->compressCmb->itemText(ui->compressCmb->currentIndex()), Qt::CaseInsensitive)))
        {
            datasize = Huffman_Compress((unsigned char*)originalText, (unsigned char*)textBuf, numChars);
            textBuf[datasize] = NULL;
            msgHeader->bVersion = 0xFF;
        }

        // populate header
        msgHeader->lSignature = 0xDEADBEEF;
        for(int i = 0; i < 3; i++)   
 		msgHeader->lReceiverAddr[i] = 0xFF;
        msgHeader->lDataLength = datasize;
        msgHeader->lDataUncompressed = numChars;
        msgHeader->bSenderAddr = ui->senderCmb->currentText().toInt();
        msgHeader->lPattern = 0xAA55AA55;
        msgHeader->bDataType = 0;
        msgHeader->sChecksum = ui->checksumChk->isChecked() ? CalculateChecksum(textBuf, datasize) + 1 : CalculateChecksum(textBuf, datasize);


        if(!WriteToRS232((BYTE *)msgHeader, &headerSize)){
            QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
            return;
        }
        free(msgHeader);
    }
    if(!WriteToRS232((BYTE *)textBuf, (DWORD *)&datasize))
        QMessageBox::information(NULL, "Error!", "Write data to RS232 failed");
}

// send voice via RS232
void SenderWindow::SendVoice()
{
    char *voiceBuf;
    DWORD headerSize;
    headerSize = HEADERSIZE;
    int datasize = lBigBufSize;
    voiceBuf = (char*)calloc ((lBigBufSize + 320), sizeof(char));
    if (voiceBuf == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloccing voiceBuf has failed.");
    }

    SetUpDCB(this->GetBaudRate());

    Header *msgHeader = (Header *)malloc(sizeof(Header));
    if (msgHeader == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloccing msgHeader has failed.");
    }
    msgHeader->bVersion = 0;

    // If huffman encoding has been selected, encode it.
    if (!(QString::compare("huffman", ui->compressCmb->itemText(ui->compressCmb->currentIndex()), Qt::CaseInsensitive)))
    {
        datasize = Huffman_Compress((unsigned char*)iBigBuf, (unsigned char*)voiceBuf, lBigBufSize);
        //voiceBuf[datasize] = NULL;
        msgHeader->bVersion = 0xFF;
    }

    // populate header
    msgHeader->lSignature = 0xDEADBEEF;
    for(int i = 0; i < 3; i++)
        msgHeader->lReceiverAddr[i] = 0xFF;

    msgHeader->lDataLength = datasize;
    msgHeader->lDataUncompressed = lBigBufSize;
    msgHeader->bSenderAddr = ui->senderCmb->currentText().toInt();
    msgHeader->lPattern = 0xAA55AA55;
    msgHeader->bDataType = 0xFF;
    msgHeader->sChecksum = ui->checksumChk->isChecked() ? CalculateChecksum(voiceBuf, datasize) + 1 : CalculateChecksum(voiceBuf, datasize);

    QMessageBox::information(NULL, "Error!", QString("Checksum: %1").arg(QString::number(msgHeader->sChecksum)));



    if(!WriteToRS232((BYTE *)msgHeader, &headerSize)){
        QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
        return;
    }

    if(!WriteToRS232((BYTE *)voiceBuf, (DWORD *)&datasize))
    {
        QMessageBox::information(NULL, "Error!", "Write voice to RS232 failed");
    }
    free(msgHeader);
    free(voiceBuf);
}

// Sends messages automatically according to the poisson distribution
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
                if (msgHeader == NULL)
                {
                    QMessageBox::information(NULL, "Error!", "Malloc has failed.");
                }
                // populate header
                msgHeader->lSignature = 0xDEADBEEF;
                for(int i = 0; i < 3; i++)
                    msgHeader->lReceiverAddr[i] = 0xFF;
                msgHeader->bVersion = 0;
                msgHeader->lDataLength = numChars;
                msgHeader->bSenderAddr = ui->senderCmb->currentText().toInt();
                msgHeader->lPattern = 0xAA55AA55;
                msgHeader->bDataType = 0;
                msgHeader->sChecksum = ui->checksumChk->isChecked() ? CalculateChecksum(buffer, numChars) + 1 : CalculateChecksum(buffer, numChars);


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
