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

// adds a receiver id to the receiver id combo box
void SenderWindow::AddReceiver()
{
    // pop up a dialog to add a sender id
    bool ok;
    QString receiver = QInputDialog::getText(NULL,"Add Receiver", "Enter new Receiver Id: ", QLineEdit::Normal, " ", &ok );

    if(ok && !receiver.isEmpty())
        ui->senderCmb->addItem(receiver);
}

// adds a sender id to the sender id combo box
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
    // get the sample rate from the ui
    g_nSamplesPerSec = ui->sampleCmb->currentText().toInt();

    // compute the buffer size
    lBigBufSize = g_nSamplesPerSec * RECORD_TIME;	// in samples

    // fill out the audio buffer
    iBigBuf = (short *)malloc(sizeof(short) * lBigBufSize);
    if (iBigBuf == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloc has failed.");
    }

    // set up the recording stuff
    InitializePlayback();

    // Record the special buffer
    InitializeRecording(iBigBuf, lBigBufSize);
    ui->msgTxt->append(QString("Recording....."));
    RecordBuffer(iBigBuf, lBigBufSize );
    ui->msgTxt->append(QString("\nDone."));
    CloseRecording();
}

// playback the audio buffer
void SenderWindow::Playback()
{
    ui->msgTxt->append(QString("Playing broadcast..."));
    InitializePlayback();
    PlayBuffer( iBigBuf, lBigBufSize );
    ui->msgTxt->append(QString("\nDone."));
    ClosePlayback();
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

    // set up the dcb with the baud rate from the ui
    SetUpDCB(this->GetBaudRate());

    // get the text from the textbox, put it into char array
    QByteArray ba = this->GetMsgText().toAscii();
    inBuf = ba.data();
    textBuf = ba.data();


    // get the number of characters in the array
    numChars = (long)this->GetMsgText().length() + 1;
    datasize = numChars;

    // if headers should be sent, set them up
    if (ui->headerChk->isChecked()){

        Header *msgHeader = (Header *)malloc(sizeof(Header));
        if (msgHeader == NULL)
        {
            QMessageBox::information(NULL, "Error!", "Malloc of msgHeader has failed.");
            return;
        }

        // set up the version first, so if there's compression it's overridden
        msgHeader->bVersion = 0;

        // If huffman encoding has been selected, encode it.
        if (!(QString::compare("huffman", ui->compressCmb->itemText(ui->compressCmb->currentIndex()), Qt::CaseInsensitive)))
        {
            datasize = Huffman_Compress((unsigned char*)inBuf, (unsigned char*)textBuf, numChars);
            textBuf[datasize] = NULL;
            msgHeader->bVersion = 0xFF;
        }

        // populate header
        msgHeader->lSignature = 0xDEADBEEF;
        for(int i = 0; i < 3; i++)   
        msgHeader->lReceiverAddr[i] = ui->receiverCmb->currentText().toInt();
        msgHeader->lDataLength = datasize;
        msgHeader->lDataUncompressed = numChars;
        msgHeader->bSenderAddr = ui->senderCmb->currentText().toInt();
        msgHeader->lPattern = 0xAA55AA55;
        msgHeader->bDataType = 0;
        msgHeader->sChecksum = ui->checksumChk->isChecked() ? CalculateChecksum(textBuf, datasize) + 1 : CalculateChecksum(textBuf, datasize);

        // send the header
        if(!WriteToRS232((BYTE *)msgHeader, &headerSize)){
            QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
            return;
        }
        // free the header, as we no longer need it
        free(msgHeader);
    }

    // send the text buffer
    if(!WriteToRS232((BYTE *)textBuf, (DWORD *)&datasize))
        QMessageBox::information(NULL, "Error!", "Write data to RS232 failed");
}

// send voice via RS232
void SenderWindow::SendVoice()
{
    char *voiceBuf;
    DWORD headerSize;
    headerSize = HEADERSIZE;

    // audio buffer size is in 16 bits, not 8, so multiply by 2
    int datasize = 2 * lBigBufSize;

    // +320 bytes for the huffman tree
    voiceBuf = (char*)calloc ((datasize + 320), sizeof(char));
    if (voiceBuf == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloccing voiceBuf has failed.");
    }

    // set up the dcb with the baud rate from the ui
    SetUpDCB(this->GetBaudRate());

    Header *msgHeader = (Header *)malloc(sizeof(Header));
    if (msgHeader == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloccing msgHeader has failed.");
    }



    // make a new buffer to put the char audio in
    char * charBuf = (char *)calloc(datasize, sizeof(char));
    if (charBuf == NULL)
    {
        QMessageBox::information(NULL, "Error!", "Malloccing charBuf has failed.");
    }

    // convert voice buffer from short to char array
    ShortToChar(charBuf, datasize, iBigBuf, lBigBufSize);

    // If huffman encoding has been selected, encode it.
    if (!(QString::compare("huffman", ui->compressCmb->itemText(ui->compressCmb->currentIndex()), Qt::CaseInsensitive)))
    {
        datasize = Huffman_Compress((unsigned char*)charBuf, (unsigned char*)voiceBuf, datasize);
        //voiceBuf[datasize] = NULL;
        msgHeader->bVersion = 0xFF;
    }else
    {
        // convert voice buffer from short to char array
        ShortToChar(voiceBuf, datasize, iBigBuf, lBigBufSize);
        // set up the version first, so if there's compression it's overridden
        msgHeader->bVersion = 0;
    }

    // populate header
    msgHeader->lSignature = 0xDEADBEEF;

    // use repetition to protect the receiver id
    for(int i = 0; i < 3; i++)
        msgHeader->lReceiverAddr[i] = ui->receiverCmb->currentText().toInt();

    msgHeader->lDataLength = datasize;
    msgHeader->lDataUncompressed = (lBigBufSize * 2); // lBigBufSize is size of short buffer, not char
    msgHeader->bSenderAddr = ui->senderCmb->currentText().toInt();
    msgHeader->lPattern = 0xAA55AA55;
    msgHeader->bDataType = 0xFF;
    msgHeader->sSamplesPerSec = ui->sampleCmb->currentText().toShort();
    // if the checksum error box is checked, send an error in the checksum for testing
    msgHeader->sChecksum = ui->checksumChk->isChecked() ? CalculateChecksum(voiceBuf, datasize) + 1 : CalculateChecksum(voiceBuf, datasize);

    // send the header
    if(!WriteToRS232((BYTE *)msgHeader, &headerSize)){
        QMessageBox::information(NULL, "Error!", "Write header to RS232 failed");
        return;
    }

    // send the audio buffer
    if(!WriteToRS232((BYTE *)voiceBuf, (DWORD *)&datasize))
    {
        QMessageBox::information(NULL, "Error!", "Write voice to RS232 failed");
    }

    // free the header and the buffer, as we no longer need them
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
                    msgHeader->lReceiverAddr[i] = ui->receiverCmb->currentText().toInt();
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

                // free the header, as we're done with it
                free(msgHeader);
            }
            if(!WriteToRS232((BYTE *)buffer, (DWORD *)&numChars))
                QMessageBox::information(NULL, "Error!", "Write data to RS232 failed");
        }
        Sleep(950);
    }
}
