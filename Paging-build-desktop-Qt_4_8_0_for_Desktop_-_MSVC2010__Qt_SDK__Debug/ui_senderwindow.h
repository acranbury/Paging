/********************************************************************************
** Form generated from reading UI file 'senderwindow.ui'
**
** Created: Fri Nov 9 14:18:39 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDERWINDOW_H
#define UI_SENDERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SenderWindow
{
public:
    QWidget *centralWidget;
    QTextEdit *msgTxt;
    QComboBox *senderCmb;
    QComboBox *receiverCmb;
    QPushButton *recordBtn;
    QPushButton *playBtn;
    QPushButton *sendAudioBtn;
    QLabel *emergBrdLbl;
    QPushButton *sendMsgBtn;
    QLabel *senderIdLbl;
    QLabel *receiverIdLbl;
    QPushButton *addSenderBtn;
    QPushButton *addReceiverBtn;
    QComboBox *sampleCmb;
    QLabel *brdSampleLbl;
    QLabel *baudRateLbl;
    QComboBox *baudRateCmb;
    QLabel *msgPriorityLbl;
    QComboBox *msgPriorityCmb;
    QPushButton *testMenu;
    QCheckBox *headerChk;
    QPushButton *poissonBtn;
    QComboBox *compressCmb;
    QLabel *compressLbl;
    QCheckBox *checksumChk;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *SenderWindow)
    {
        if (SenderWindow->objectName().isEmpty())
            SenderWindow->setObjectName(QString::fromUtf8("SenderWindow"));
        SenderWindow->resize(388, 347);
        SenderWindow->setMinimumSize(QSize(388, 347));
        SenderWindow->setMaximumSize(QSize(388, 347));
        centralWidget = new QWidget(SenderWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        msgTxt = new QTextEdit(centralWidget);
        msgTxt->setObjectName(QString::fromUtf8("msgTxt"));
        msgTxt->setGeometry(QRect(160, 110, 201, 131));
        senderCmb = new QComboBox(centralWidget);
        senderCmb->setObjectName(QString::fromUtf8("senderCmb"));
        senderCmb->setGeometry(QRect(80, 80, 51, 22));
        receiverCmb = new QComboBox(centralWidget);
        receiverCmb->setObjectName(QString::fromUtf8("receiverCmb"));
        receiverCmb->setGeometry(QRect(80, 110, 51, 22));
        recordBtn = new QPushButton(centralWidget);
        recordBtn->setObjectName(QString::fromUtf8("recordBtn"));
        recordBtn->setGeometry(QRect(10, 220, 75, 23));
        playBtn = new QPushButton(centralWidget);
        playBtn->setObjectName(QString::fromUtf8("playBtn"));
        playBtn->setGeometry(QRect(10, 250, 75, 23));
        sendAudioBtn = new QPushButton(centralWidget);
        sendAudioBtn->setObjectName(QString::fromUtf8("sendAudioBtn"));
        sendAudioBtn->setGeometry(QRect(10, 280, 75, 23));
        emergBrdLbl = new QLabel(centralWidget);
        emergBrdLbl->setObjectName(QString::fromUtf8("emergBrdLbl"));
        emergBrdLbl->setGeometry(QRect(10, 200, 111, 16));
        sendMsgBtn = new QPushButton(centralWidget);
        sendMsgBtn->setObjectName(QString::fromUtf8("sendMsgBtn"));
        sendMsgBtn->setGeometry(QRect(280, 260, 81, 23));
        senderIdLbl = new QLabel(centralWidget);
        senderIdLbl->setObjectName(QString::fromUtf8("senderIdLbl"));
        senderIdLbl->setGeometry(QRect(10, 80, 51, 16));
        receiverIdLbl = new QLabel(centralWidget);
        receiverIdLbl->setObjectName(QString::fromUtf8("receiverIdLbl"));
        receiverIdLbl->setGeometry(QRect(10, 110, 61, 16));
        addSenderBtn = new QPushButton(centralWidget);
        addSenderBtn->setObjectName(QString::fromUtf8("addSenderBtn"));
        addSenderBtn->setGeometry(QRect(10, 30, 75, 23));
        addReceiverBtn = new QPushButton(centralWidget);
        addReceiverBtn->setObjectName(QString::fromUtf8("addReceiverBtn"));
        addReceiverBtn->setGeometry(QRect(100, 30, 75, 23));
        sampleCmb = new QComboBox(centralWidget);
        sampleCmb->setObjectName(QString::fromUtf8("sampleCmb"));
        sampleCmb->setGeometry(QRect(280, 30, 69, 22));
        brdSampleLbl = new QLabel(centralWidget);
        brdSampleLbl->setObjectName(QString::fromUtf8("brdSampleLbl"));
        brdSampleLbl->setGeometry(QRect(280, 10, 71, 16));
        baudRateLbl = new QLabel(centralWidget);
        baudRateLbl->setObjectName(QString::fromUtf8("baudRateLbl"));
        baudRateLbl->setGeometry(QRect(190, 10, 51, 16));
        baudRateCmb = new QComboBox(centralWidget);
        baudRateCmb->setObjectName(QString::fromUtf8("baudRateCmb"));
        baudRateCmb->setGeometry(QRect(190, 30, 69, 22));
        msgPriorityLbl = new QLabel(centralWidget);
        msgPriorityLbl->setObjectName(QString::fromUtf8("msgPriorityLbl"));
        msgPriorityLbl->setGeometry(QRect(10, 150, 81, 16));
        msgPriorityCmb = new QComboBox(centralWidget);
        msgPriorityCmb->setObjectName(QString::fromUtf8("msgPriorityCmb"));
        msgPriorityCmb->setGeometry(QRect(100, 150, 51, 22));
        testMenu = new QPushButton(centralWidget);
        testMenu->setObjectName(QString::fromUtf8("testMenu"));
        testMenu->setEnabled(false);
        testMenu->setGeometry(QRect(100, 280, 75, 23));
        headerChk = new QCheckBox(centralWidget);
        headerChk->setObjectName(QString::fromUtf8("headerChk"));
        headerChk->setGeometry(QRect(180, 290, 70, 17));
        headerChk->setChecked(true);
        poissonBtn = new QPushButton(centralWidget);
        poissonBtn->setObjectName(QString::fromUtf8("poissonBtn"));
        poissonBtn->setGeometry(QRect(280, 290, 75, 23));
        compressCmb = new QComboBox(centralWidget);
        compressCmb->setObjectName(QString::fromUtf8("compressCmb"));
        compressCmb->setGeometry(QRect(280, 80, 81, 22));
        compressLbl = new QLabel(centralWidget);
        compressLbl->setObjectName(QString::fromUtf8("compressLbl"));
        compressLbl->setGeometry(QRect(280, 60, 71, 16));
        checksumChk = new QCheckBox(centralWidget);
        checksumChk->setObjectName(QString::fromUtf8("checksumChk"));
        checksumChk->setGeometry(QRect(180, 270, 101, 17));
        SenderWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SenderWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 388, 21));
        SenderWindow->setMenuBar(menuBar);

        retranslateUi(SenderWindow);

        QMetaObject::connectSlotsByName(SenderWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SenderWindow)
    {
        SenderWindow->setWindowTitle(QApplication::translate("SenderWindow", "Paging Transmitter", 0, QApplication::UnicodeUTF8));
        senderCmb->clear();
        senderCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "69", 0, QApplication::UnicodeUTF8)
        );
        receiverCmb->clear();
        receiverCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "FF", 0, QApplication::UnicodeUTF8)
        );
        recordBtn->setText(QApplication::translate("SenderWindow", "Record", 0, QApplication::UnicodeUTF8));
        playBtn->setText(QApplication::translate("SenderWindow", "Playback", 0, QApplication::UnicodeUTF8));
        sendAudioBtn->setText(QApplication::translate("SenderWindow", "Send", 0, QApplication::UnicodeUTF8));
        emergBrdLbl->setText(QApplication::translate("SenderWindow", "Emergency Broadcast", 0, QApplication::UnicodeUTF8));
        sendMsgBtn->setText(QApplication::translate("SenderWindow", "Send Message", 0, QApplication::UnicodeUTF8));
        senderIdLbl->setText(QApplication::translate("SenderWindow", "Sender ID", 0, QApplication::UnicodeUTF8));
        receiverIdLbl->setText(QApplication::translate("SenderWindow", "Receiver ID", 0, QApplication::UnicodeUTF8));
        addSenderBtn->setText(QApplication::translate("SenderWindow", "Add Sender", 0, QApplication::UnicodeUTF8));
        addReceiverBtn->setText(QApplication::translate("SenderWindow", "Add Receiver", 0, QApplication::UnicodeUTF8));
        sampleCmb->clear();
        sampleCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "8000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "11025", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "22050", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "44100", 0, QApplication::UnicodeUTF8)
        );
        brdSampleLbl->setText(QApplication::translate("SenderWindow", "Sampling Rate", 0, QApplication::UnicodeUTF8));
        baudRateLbl->setText(QApplication::translate("SenderWindow", "Baud Rate", 0, QApplication::UnicodeUTF8));
        baudRateCmb->clear();
        baudRateCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "2400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "4800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "14400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "28800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "57600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "115200", 0, QApplication::UnicodeUTF8)
        );
        msgPriorityLbl->setText(QApplication::translate("SenderWindow", "Message Priority", 0, QApplication::UnicodeUTF8));
        msgPriorityCmb->clear();
        msgPriorityCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "9", 0, QApplication::UnicodeUTF8)
        );
        testMenu->setText(QApplication::translate("SenderWindow", "Testing Menu", 0, QApplication::UnicodeUTF8));
        headerChk->setText(QApplication::translate("SenderWindow", "Headers", 0, QApplication::UnicodeUTF8));
        poissonBtn->setText(QApplication::translate("SenderWindow", "Poisson", 0, QApplication::UnicodeUTF8));
        compressCmb->clear();
        compressCmb->insertItems(0, QStringList()
         << QApplication::translate("SenderWindow", "Huffman", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "RLE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "Differential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SenderWindow", "None", 0, QApplication::UnicodeUTF8)
        );
        compressLbl->setText(QApplication::translate("SenderWindow", "Compression", 0, QApplication::UnicodeUTF8));
        checksumChk->setText(QApplication::translate("SenderWindow", "Checksum Error", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SenderWindow: public Ui_SenderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDERWINDOW_H
