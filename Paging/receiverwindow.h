#ifndef RECEIVERWINDOW_H
#define RECEIVERWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QThread>
#include "pollingworker.h"
extern "C"
{
#include "bst.h"
}
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
    void HandleTextChange(char chr);
    void HandleAudio(long audioSize, char* audio, short samplesPerSec);
    void HandleTransmitError();
    void UpdateQueueWindow();       // handles updating messages in the textEdit
    void DisplayPhonebook();


private:
    void BSTPrint(TreeNode * treeRoot);
    Ui::ReceiverWindow *ui;
    short *iBigBuf;          // pointer to audio buffer
    long lBigBufSize;       // audio playback buffer size
    QThread *thread;
    PollingWorker *poller;
    void Traverse(Msg *h);
    void PrintTenChars (Msg * msg);
    int displayInbox;
    int transmitErrorCount;
    char GetReceiverId(char lReceiverAdd[3]);
    char GetHammingDistance(char x, char y);
    int displayTree;
    void PopulatePhonebook();
    void SavePhonebook();
    void BSTSave(TreeNode * treeNode, FILE * fp);

};

#endif // RECEIVERWINDOW_H
