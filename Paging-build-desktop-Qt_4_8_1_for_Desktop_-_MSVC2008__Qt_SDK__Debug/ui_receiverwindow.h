/********************************************************************************
** Form generated from reading UI file 'receiverwindow.ui'
**
** Created: Fri Oct 19 14:03:59 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVERWINDOW_H
#define UI_RECEIVERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReceiverWindow
{
public:
    QWidget *centralwidget;
    QRadioButton *priorityRdoBtn;
    QRadioButton *fifoRdoBtn;
    QLabel *msOrderLbl;
    QPushButton *deleteBtn;
    QPushButton *readBtn;
    QLabel *msgOptionsLbl;
    QPushButton *archiveBtn;
    QLabel *msgNumLbl;
    QPushButton *refreshBtn;
    QPushButton *phoneBookBtn;
    QPushButton *audioBtn;
    QLabel *errorLbl;
    QComboBox *baudRateCmb;
    QLabel *baudRateLbl;
    QPushButton *pushButton;
    QTextEdit *msgTxt;
    QMenuBar *menubar;
    QButtonGroup *msgOrderGrp;

    void setupUi(QMainWindow *ReceiverWindow)
    {
        if (ReceiverWindow->objectName().isEmpty())
            ReceiverWindow->setObjectName(QString::fromUtf8("ReceiverWindow"));
        ReceiverWindow->resize(410, 322);
        centralwidget = new QWidget(ReceiverWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        priorityRdoBtn = new QRadioButton(centralwidget);
        msgOrderGrp = new QButtonGroup(ReceiverWindow);
        msgOrderGrp->setObjectName(QString::fromUtf8("msgOrderGrp"));
        msgOrderGrp->addButton(priorityRdoBtn);
        priorityRdoBtn->setObjectName(QString::fromUtf8("priorityRdoBtn"));
        priorityRdoBtn->setGeometry(QRect(10, 30, 61, 17));
        fifoRdoBtn = new QRadioButton(centralwidget);
        msgOrderGrp->addButton(fifoRdoBtn);
        fifoRdoBtn->setObjectName(QString::fromUtf8("fifoRdoBtn"));
        fifoRdoBtn->setGeometry(QRect(10, 50, 51, 17));
        msOrderLbl = new QLabel(centralwidget);
        msOrderLbl->setObjectName(QString::fromUtf8("msOrderLbl"));
        msOrderLbl->setGeometry(QRect(10, 10, 81, 16));
        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName(QString::fromUtf8("deleteBtn"));
        deleteBtn->setGeometry(QRect(10, 100, 75, 23));
        readBtn = new QPushButton(centralwidget);
        readBtn->setObjectName(QString::fromUtf8("readBtn"));
        readBtn->setGeometry(QRect(10, 130, 75, 23));
        msgOptionsLbl = new QLabel(centralwidget);
        msgOptionsLbl->setObjectName(QString::fromUtf8("msgOptionsLbl"));
        msgOptionsLbl->setGeometry(QRect(10, 80, 91, 16));
        archiveBtn = new QPushButton(centralwidget);
        archiveBtn->setObjectName(QString::fromUtf8("archiveBtn"));
        archiveBtn->setGeometry(QRect(10, 160, 75, 23));
        msgNumLbl = new QLabel(centralwidget);
        msgNumLbl->setObjectName(QString::fromUtf8("msgNumLbl"));
        msgNumLbl->setGeometry(QRect(120, 10, 121, 16));
        refreshBtn = new QPushButton(centralwidget);
        refreshBtn->setObjectName(QString::fromUtf8("refreshBtn"));
        refreshBtn->setGeometry(QRect(290, 10, 75, 23));
        phoneBookBtn = new QPushButton(centralwidget);
        phoneBookBtn->setObjectName(QString::fromUtf8("phoneBookBtn"));
        phoneBookBtn->setGeometry(QRect(230, 260, 75, 23));
        audioBtn = new QPushButton(centralwidget);
        audioBtn->setObjectName(QString::fromUtf8("audioBtn"));
        audioBtn->setGeometry(QRect(310, 260, 91, 23));
        errorLbl = new QLabel(centralwidget);
        errorLbl->setObjectName(QString::fromUtf8("errorLbl"));
        errorLbl->setGeometry(QRect(10, 280, 46, 13));
        baudRateCmb = new QComboBox(centralwidget);
        baudRateCmb->setObjectName(QString::fromUtf8("baudRateCmb"));
        baudRateCmb->setGeometry(QRect(10, 240, 69, 22));
        baudRateLbl = new QLabel(centralwidget);
        baudRateLbl->setObjectName(QString::fromUtf8("baudRateLbl"));
        baudRateLbl->setGeometry(QRect(10, 220, 51, 16));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(90, 260, 121, 23));
        msgTxt = new QTextEdit(centralwidget);
        msgTxt->setObjectName(QString::fromUtf8("msgTxt"));
        msgTxt->setGeometry(QRect(110, 50, 271, 191));
        msgTxt->setReadOnly(true);
        ReceiverWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ReceiverWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 410, 21));
        ReceiverWindow->setMenuBar(menubar);

        retranslateUi(ReceiverWindow);

        QMetaObject::connectSlotsByName(ReceiverWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ReceiverWindow)
    {
        ReceiverWindow->setWindowTitle(QApplication::translate("ReceiverWindow", "Paging Receiver", 0, QApplication::UnicodeUTF8));
        priorityRdoBtn->setText(QApplication::translate("ReceiverWindow", "Priority", 0, QApplication::UnicodeUTF8));
        fifoRdoBtn->setText(QApplication::translate("ReceiverWindow", "FIFO", 0, QApplication::UnicodeUTF8));
        msOrderLbl->setText(QApplication::translate("ReceiverWindow", "Message Order", 0, QApplication::UnicodeUTF8));
        deleteBtn->setText(QApplication::translate("ReceiverWindow", "Delete", 0, QApplication::UnicodeUTF8));
        readBtn->setText(QApplication::translate("ReceiverWindow", "Read", 0, QApplication::UnicodeUTF8));
        msgOptionsLbl->setText(QApplication::translate("ReceiverWindow", "Message Options", 0, QApplication::UnicodeUTF8));
        archiveBtn->setText(QApplication::translate("ReceiverWindow", "Archive", 0, QApplication::UnicodeUTF8));
        msgNumLbl->setText(QApplication::translate("ReceiverWindow", "Number of Messages: 0", 0, QApplication::UnicodeUTF8));
        refreshBtn->setText(QApplication::translate("ReceiverWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        phoneBookBtn->setText(QApplication::translate("ReceiverWindow", "Phonebook", 0, QApplication::UnicodeUTF8));
        audioBtn->setText(QApplication::translate("ReceiverWindow", "Audio Messages", 0, QApplication::UnicodeUTF8));
        errorLbl->setText(QApplication::translate("ReceiverWindow", "Errors: 0", 0, QApplication::UnicodeUTF8));
        baudRateCmb->clear();
        baudRateCmb->insertItems(0, QStringList()
         << QApplication::translate("ReceiverWindow", "2400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ReceiverWindow", "4800", 0, QApplication::UnicodeUTF8)
        );
        baudRateLbl->setText(QApplication::translate("ReceiverWindow", "Baud Rate", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ReceiverWindow", "Change Receiver ID(s)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReceiverWindow: public Ui_ReceiverWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVERWINDOW_H
