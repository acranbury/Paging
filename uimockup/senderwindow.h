#ifndef SENDERWINDOW_H
#define SENDERWINDOW_H

#include <QMainWindow>

namespace Ui {
class SenderWindow;
}

class SenderWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SenderWindow(QWidget *parent = 0);
    ~SenderWindow();
    
private:
    Ui::SenderWindow *ui;
};

#endif // SENDERWINDOW_H
