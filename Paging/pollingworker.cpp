#include "pollingworker.h"

PollingWorker::PollingWorker(int ibaudRate, QCheckBox * rawText)
{
    baudRate = ibaudRate;
    isRaw = rawText;
}

PollingWorker::~PollingWorker()
{

}

void PollingWorker::SetBaudRate(int ibaudRate)
{
    baudRate = ibaudRate;
}

int PollingWorker::GetBaudRate()
{
    return baudRate;
}

void PollingWorker::PollRS232()
{
    char readBuf[BUFSIZE] = {0};
    char * rawByte;
    Header * headerBuffer;
    SetUpDCB(baudRate);
    long numBytesToGet;
    DWORD dwCommEvent, dwBytesTransferred;
    Msg * newMsg;
    while(1)
    {
        // set up the mask, EV_RXCHAR is the event when we receive a character
        if (!SetCommMask(hComm, EV_RXCHAR))
            emit error(QString("Error setting communications mask."), (int)GetLastError());

        // wait for a character to come in
        if (!WaitCommEvent(hComm, &dwCommEvent, NULL))
            emit error(QString("Error waiting for a character."), (int)GetLastError());

        // we have a character, read the header to see if its good
        else
        {
            if(!isRaw->isChecked())
            {
                // set up the header buffer
                headerBuffer = (Header *)malloc(sizeof(struct Header));
                if(!ReadFile(hComm, (BYTE *)headerBuffer, HEADERSIZE, &dwBytesTransferred, 0))
                    emit error(QString("Error setting up the header buffer."), (int)GetLastError());

                // if the header is good, get the length of the message
                if(headerBuffer->lSignature == 0xDEADBEEF && headerBuffer->bReceiverAddr == 0xFF)
                {
                    numBytesToGet = headerBuffer->lDataLength;

                    // get the message
                    if(!ReadFile(hComm, readBuf, numBytesToGet, &dwBytesTransferred, 0))
                        emit error(QString("Error getting the message."), (int)GetLastError());


                    // create a new message structure and put it on the queue
                    // not all the header options we need are available - ask Jack!
                    newMsg = (Msg *)malloc(sizeof(struct message));
                    strcpy(newMsg->txt, readBuf);
                    newMsg->senderID = headerBuffer->bSenderAddr;
                    newMsg->receiverID = headerBuffer->bReceiverAddr;
                    newMsg->msgNum = rand() % 100;
                    AddToQueue(newMsg);
                    emit labelEdit(QString("Number of Messages: %1").arg(numberOfMessages));
                }
            }
            else
            {
                do
                {
                    rawByte = (char *)malloc(sizeof(char));
                    if(!ReadFile(hComm, rawByte, 1, &dwBytesTransferred, 0))
                        emit error(QString("Error getting the raw data."), (int)GetLastError());

                    if(dwBytesTransferred != 0)
                        emit messageEdit(QString(rawByte));

                }while(dwBytesTransferred != 0);
            }
        }
    }
}
