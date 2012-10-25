#ifndef RECEIVERWINDOW_H
#define RECEIVERWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QThread>
#include "pollingworker.h"

namespace Ui {
class ReceiverWindow;
}

class ReceiverWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ReceiverWindow(QWidget *parent = 0);
    ~ReceiverWindow();
    QString GetMsgText() const;
    void SetMsgText(QString &text);
    int GetBaudRate();
    int GetNumMsgs();
    void SetNumMsgs(int numMsgs);
    QLabel *pmsgLabel;
    QTextEdit *ptextEdit;
    QCheckBox *pRawData;
    void StartPoller();
    
protected slots:
    void Playback();
    void Refresh();
    void Archive();
    void HandleErrors(QString error, int code);
    void HandleLabelChange(QString message);
    void HandleTextChange(QString message);

private:
    Ui::ReceiverWindow *ui;
    //void UpdateQueueWindow();       // handles updating messages in the textEdit
    //static HANDLE pollingHandle;    // handle for polling thread
    short *iBigBuf;          // pointer to audio buffer
    long lBigBufSize;       // audio playback buffer size
    QThread *thread;
    PollingWorker *poller;

};

#endif // RECEIVERWINDOW_H
