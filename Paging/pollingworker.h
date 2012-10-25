#ifndef POLLINGWORKER_H
#define POLLINGWORKER_H

#include <QObject>
#include <QLabel>
#include <Windows.h>
#include <QMessageBox>
#include "pollingworker.h"
#include "rs232.h"
extern "C"
{
#include "TxtMessage.h"
}


class PollingWorker : public QObject
{
    Q_OBJECT
public:
    explicit PollingWorker(int ibaudRate, QLabel * numMsgs);
    ~PollingWorker();
    void SetBaudRate(int ibaudRate);
    int GetBaudRate();
    
signals:
    void finished();
    
public slots:
    void PollRS232();
    
private:
    int baudRate;
    QLabel * msgLabel;
};

#endif // POLLINGWORKER_H
