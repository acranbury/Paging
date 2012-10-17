#ifndef RECEIVERWINDOW_H
#define RECEIVERWINDOW_H

#include <QMainWindow>

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
};

#endif // RECEIVERWINDOW_H
