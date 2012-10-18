#include "rs232.h"
#include <QMessageBox>

// open the RS232 port
int OpenRS232Port()
{
    hComm = CreateFile( (LPWSTR)COMPORT4,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        0,
                        OPEN_EXISTING,
                        0,
                        0);

    // error opening port; abort
    if (hComm == INVALID_HANDLE_VALUE)
    {
        QMessageBox::information(NULL, "Error!", "Error opening serial port.");
        return(0);
    }
    return (1);
}

// set up comm port settings
int SetUpDCB(int baudRate)
{
    FillMemory(&portDCB, sizeof(DCB), 0);
    if (!GetCommState(hComm, &portDCB))
    {
          QMessageBox::information(NULL, "Error!", "Error getting current DCB settings.");
          return FALSE;
    }

    // update baud rate
    portDCB.BaudRate = baudRate;

    // turn parity off for now
    portDCB.Parity = FALSE;

    // turn on binary send
    portDCB.fBinary = TRUE;

    if(!SetCommState(hComm, &portDCB))
    {
        QMessageBox::information(NULL, "Error!", "Error setting current DCB settings. ");
        return FALSE;
    }

    return(1);
}

// write a buffer to RS232
int WriteToRS232(short * writeBuf, long *bufSize)
{
    // write the file to
    int result = WriteFile(hComm,
                           writeBuf,
                           *bufSize,
                           (LPDWORD)bufSize,
                           NULL);
    return result;
}

// read from RS232
void ReadFromRS232(BYTE * readBuf, long *bufSize)
{
    BYTE prev, *curr;
    prev = 0;
    curr = NULL;
    do
    {
        ReadFile (hComm,
                  curr,
                  1,
                  (LPDWORD)bufSize,
                  NULL);
        prev = *curr;

    }
}

// close the RS232 port
void CloseRS232Port()
{
    CloseHandle(hComm);
}
