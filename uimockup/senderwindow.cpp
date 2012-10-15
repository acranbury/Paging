#include "senderwindow.h"
#include "ui_senderwindow.h"

SenderWindow::SenderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SenderWindow)
{
    ui->setupUi(this);
}

SenderWindow::~SenderWindow()
{
    delete ui;
}
