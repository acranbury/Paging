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
    
protected slots:
    void Playback();

private:
    Ui::ReceiverWindow *ui;
    void ReadRS232(HANDLE hComm);
};

#endif // RECEIVERWINDOW_H
