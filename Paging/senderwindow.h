#ifndef SENDERWINDOW_H
#define SENDERWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <MMSystem.h>

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

protected slots:
    void Record();
    void Playback();

private:
    Ui::SenderWindow *ui;

};

#endif // SENDERWINDOW_H
