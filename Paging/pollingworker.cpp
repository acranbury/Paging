#include "pollingworker.h"
extern "C"
{
#include "checksum.h"
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

// returns the hamming distance of two integers
char PollingWorker::GetHammingDistance(char x, char y)
{
    char dist = 0;
    char val = x ^ y;

    // count the number of set bits
    while(val)
    {
        ++dist;
        val &= val - 1;
    }

    return dist;
}

// extracts the receiver id from the repeated one in the header
char PollingWorker::GetReceiverId(char lReceiverAdd[3])
{
    char hamming[3];
    hamming[0] = this->GetHammingDistance(lReceiverAdd[0], lReceiverAdd[1]);
    hamming[1] = this->GetHammingDistance(lReceiverAdd[0], lReceiverAdd[2]);
    hamming[2] = this->GetHammingDistance(lReceiverAdd[1], lReceiverAdd[2]);

    if(hamming[0] == 0)
    {
        return lReceiverAdd[0];
    }else if(hamming[1] == 0)
    {
        return lReceiverAdd[0];
    }else if(hamming[2] == 0)
    {
        return lReceiverAdd[1];
    }else
    {
        // place holder for actual receiver id stuff, dont have it yet
        if(lReceiverAdd[0] == 0xFF || lReceiverAdd[1] == 0xFF || lReceiverAdd[2] == 0xFF)
            return 0xFF;
        else
            return 0x00;
    }


}

void PollingWorker::PollRS232()
{
    char * readBuf;
    char * rawByte;
    char * unCompressed; 
    Header * headerBuffer;
    Item * sender;
    long numBytesToGet;
    char receiveID;
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
                if(headerBuffer->lSignature == 0xDEADBEEF && (receiveID = GetReceiverId(headerBuffer->lReceiverAddr)) == 0xFF)
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

                    // calculate the checksum and compare
                    if(headerBuffer->sChecksum == CalculateChecksum((short*)readBuf, headerBuffer->lDataLength))
                    {
                        emit transmitError();
                    }

                    if (headerBuffer->bVersion == 0xFF)
                    {
                        if(!(unCompressed = (char *)malloc(sizeof(char)*headerBuffer->lDataUncompressed)))
                                emit error(QString("Error malloccing unCompressed."), (int)GetLastError());
                        Huffman_Uncompress((unsigned char*)readBuf, (unsigned char*)unCompressed, headerBuffer->lDataLength, headerBuffer->lDataUncompressed);
                        // For testing purposes.
                        emit error (QString("Here is the compressed data %1").arg(readBuf), (int)GetLastError());
                    }

                    if (headerBuffer->bDataType == 0){ // If the data is text.
                        // create a new message structure and put it on the queue
                        // not all the header options we need are available - ask Jack!
                        if(!(newMsg = (Msg *)malloc(sizeof(struct message))))
                            emit error(QString("Error malloccing newMsg."), (int)GetLastError());

                        strcpy(newMsg->txt, unCompressed);
                        newMsg->senderID = headerBuffer->bSenderAddr;
                        newMsg->receiverID = (short)receiveID;
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
                        emit audioReceived(headerBuffer->lDataUncompressed, unCompressed);
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
