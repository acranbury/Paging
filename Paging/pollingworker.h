#ifndef POLLINGWORKER_H
#define POLLINGWORKER_H

#include <QObject>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <Windows.h>
#include <QMessageBox>
#include "rs232.h"
extern "C"
{
#include "TxtMessage.h"
#include "huffman.h"
}

class PollingWorker : public QObject
{
    Q_OBJECT
public:
    explicit PollingWorker(int ibaudRate, QCheckBox * rawText);
    ~PollingWorker();
    void SetBaudRate(int ibaudRate);
    int GetBaudRate();
    void SetIsFinished(int finish);
    
signals:
    void finished();
    void error(QString err, int code);
    void labelEdit(QString message);
    void messageEdit(QString message);
    void audioReceived (long lBigBufSize, char * BigBuf, short samplesPerSec);
    void transmitError();
    
public slots:
    void PollRS232();
    
private:
    int baudRate;
    QCheckBox * isRaw;
    int isFinish;
    char GetHammingDistance(char x, char y);
    char GetReceiverId(char lReceiverAdd[3]);
};

#endif // POLLINGWORKER_H
