#include "pollingworker.h"
extern "C"
{
#include "bst.h"
}


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

void PollingWorker::SetIsFinished(int finish)
{
    isFinish = finish;
}

void PollingWorker::PollRS232()
{
    char * readBuf;
    char * rawByte;
    char * original;
    Header * headerBuffer;
    Item * sender;
    long numBytesToGet;
    DWORD dwCommEvent, dwBytesTransferred;
    Msg * newMsg;
    isFinish = 0;
    while(!isFinish)
    {
        SetUpDCB(baudRate);
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
                if(!(headerBuffer = (Header *)malloc(sizeof(struct Header))))
                        emit error(QString("Error malloccing headerBuffer."), (int)GetLastError());

                if(!ReadFile(hComm, (BYTE *)headerBuffer, HEADERSIZE, &dwBytesTransferred, 0))
                    emit error(QString("Error setting up the header buffer."), (int)GetLastError());

               // emit error(QString("size of header"), (int)(dwBytesTransferred));

                // if the header is good, get the length of the message
                if(headerBuffer->lSignature == 0xDEADBEEF && headerBuffer->lReceiverAddr == 0xFF)
                {
                    numBytesToGet = headerBuffer->lDataLength;
                   // emit error(QString::number(numBytesToGet), 0);
                    readBuf = (char*)calloc(numBytesToGet,sizeof(char));
                    if (readBuf == NULL)
                        emit error(QString("Error mallocing readBuf."), (int)GetLastError());

                    // get the message
                    if(!ReadFile(hComm, readBuf, numBytesToGet, &dwBytesTransferred, 0))
                        emit error(QString("Error getting the message."), (int)GetLastError());
                  //  emit error(QString(readBuf), (int)(dwBytesTransferred));


                    if (headerBuffer->bVersion == 0xFF)
                    {
                        Huffman_Uncompress((unsigned char*)original, (unsigned char*)readBuf, numBytesToGet, headerBuffer->lDataLength);
                        // For testing purposes.
                        emit error (QString("Here is the compressed data %1").arg(original), (int)GetLastError());
                    }


                    if (headerBuffer->bDataType == 0){ // If the data is text.
                        // create a new message structure and put it on the queue
                        // not all the header options we need are available - ask Jack!
                        if(!(newMsg = (Msg *)malloc(sizeof(struct message))))
                            emit error(QString("Error malloccing newMsg."), (int)GetLastError());

                        strcpy(newMsg->txt, readBuf);
                        newMsg->senderID = headerBuffer->bSenderAddr;
                        newMsg->receiverID = headerBuffer->lReceiverAddr;
                        newMsg->msgNum = rand() % 100;
                        AddToQueue(newMsg);
                        // Check if the senderID has already been created. If not, create one.
                        if ((sender = BSTSearch(root, headerBuffer->bSenderAddr))== NULL)
                        {
                            Item * newItem = (Item*)(malloc (sizeof(Item)));
                            int * count = (int*)(malloc (sizeof(int)));
                            *count = 1;
                            newItem->key = headerBuffer->bSenderAddr;
                            newItem->data = count;
                            root = BSTInsert(root,newItem);
                        }
                        else // If it has been created, increment
                            *((int*)sender->data) = *((int*)sender->data) + 1;



                        emit labelEdit(QString("Number of Messages: %1").arg(numberOfMessages));
                    }
                    else
                    {
                        emit audioReceived(headerBuffer->lDataUncompressed, readBuf);
                    }
                }
            }
            else
            {
                do
                {
                    if(!(rawByte = (char *)calloc(512, sizeof(char))))
                        emit error(QString("Error malloccing rawByte."), (int)GetLastError());

                    if(!ReadFile(hComm, rawByte, 512, &dwBytesTransferred, 0))
                        emit error(QString("Error getting the raw data."), (int)GetLastError());

                    if(dwBytesTransferred != 0)
                        emit messageEdit(QString(rawByte));

                }while(dwBytesTransferred != 0);
            }
        }
    }
}
