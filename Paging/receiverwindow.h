#ifndef RECEIVERWINDOW_H
#define RECEIVERWINDOW_H

#include <QMainWindow>
#include <Windows.h>


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
    void PollRS232(void * dummy);
    int GetNumMsgs();
    void SetNumMsgs(int numMsgs);
    
protected slots:
    void Playback();
    void Refresh();
    void Archive();

private:
    Ui::ReceiverWindow *ui;

};

#endif // RECEIVERWINDOW_H
