#ifndef SENDERWINDOW_H
#define SENDERWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <MMSystem.h>

#define DEFSENDER 0x45

namespace Ui {
class SenderWindow;
}

class SenderWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SenderWindow(QWidget *parent = 0);
    ~SenderWindow();
    QString GetMsgText() const;
    void SetMsgText(QString &text);
    int GetBaudRate();

protected slots:
    void Record();
    void Playback();
    void SendText();
    void SendPoisson();
    void SendVoice();
    void AddSender();

private:
    Ui::SenderWindow *ui;
};

#endif // SENDERWINDOW_H
