#include "pollingworker.h"

PollingWorker::PollingWorker(int ibaudRate)
{
    baudRate = ibaudRate;
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
    Header * headerBuffer;
    SetUpDCB(baudRate);
    long numBytesToGet;
    DWORD dwCommEvent, dwBytesTransferred;
    Msg * newMsg;
    while(1)
    {
        // set up the mask, EV_RXCHAR is the event when we receive a character
        if (!SetCommMask(hComm, EV_RXCHAR))
        {
            QMessageBox::information(NULL, "Error!", "Error setting communications mask.");
            emit finished();
        }

        // wait for a character to come in
        if (!WaitCommEvent(hComm, &dwCommEvent, NULL))
        {
            QMessageBox::information(NULL, "Error!", "Error occurred waiting for a character.");
            emit finished();
        }

        // we have a character, read the header to see if its good
        else
        {
            // set up the header buffer
            headerBuffer = (Header *)malloc(sizeof(struct Header));
            if(!ReadFile(hComm, (BYTE *)headerBuffer, HEADERSIZE, &dwBytesTransferred, 0))
            {
                QMessageBox::information(NULL, "Error!", "Error occurred receiving header.");
                emit finished();
            }

            // if the header is good, get the length of the message
            if(headerBuffer->lSignature == 0xDEADBEEF)
            {
                numBytesToGet = headerBuffer->lDataLength;
            }

            // get the message
            if(!ReadFile(hComm, readBuf, numBytesToGet, &dwBytesTransferred, 0))
            {
                QMessageBox::information(NULL, "Error!", "Error occurred receiving message.");
                emit finished();
            }

            // create a new message structure and put it on the queue
            // not all the header options we need are available - ask Jack!
            newMsg = (Msg *)malloc(sizeof(struct message));
            strcpy(newMsg->txt, readBuf);
            newMsg->senderID = rand() % 100;
            newMsg->receiverID = headerBuffer->bReceiverAddr;
            newMsg->msgNum = rand() % 100;
            AddToQueue(newMsg);
        }
    }
}
