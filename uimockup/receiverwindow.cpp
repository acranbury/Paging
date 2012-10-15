#include "receiverwindow.h"
#include "ui_receiverwindow.h"

ReceiverWindow::ReceiverWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReceiverWindow)
{
    ui->setupUi(this);
}

ReceiverWindow::~ReceiverWindow()
{
    delete ui;
}
