#include "rs232.h"

#include <QMessageBox>
#define BYTESIZE  8

// open the RS232 port
int OpenRS232Port()
{
    hComm = CreateFile( TEXT(COMPORT4),
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        0,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        0);

    // error opening port; abort
    if (hComm == INVALID_HANDLE_VALUE)
    {
        QMessageBox::information(NULL, "Error!", "Error opening serial port COM4.\n Press OK to try COM3.");
        hComm = CreateFile( TEXT(COMPORT3),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            0,
                            0);
         if (hComm == INVALID_HANDLE_VALUE)
         {
             QMessageBox::information(NULL, "Error!", "Error opening serial port COM3.");
             return(0);
         }
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
    portDCB.ByteSize = (BYTE)BYTESIZE;
    portDCB.StopBits = ONESTOPBIT;

    if(!SetCommState(hComm, &portDCB))
    {
        QMessageBox::information(NULL, "Error!", "Error setting current DCB settings. ");
        return FALSE;
    }


    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 100;

    if (!SetCommTimeouts(hComm, &timeouts))
    {
        QMessageBox::information(NULL, "Error!", "Error setting timeouts. ");
        return FALSE;
    }

    return(1);
}

// write a buffer to RS232
int WriteToRS232(BYTE * writeBuf, long *bufSize)
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
void ReadFromRS232(BYTE * readBuf)
{
    BYTE temp;
    DWORD dwBytesTransferred;
    DWORD dwCommModemStatus;
    int i = 0;

    // events to monitor
    SetCommMask(hComm, EV_RXCHAR);

    // wait for character to come into the port
    WaitCommEvent(hComm, &dwCommModemStatus, 0);

    SetCommMask(hComm, EV_RXCHAR);

    if(dwCommModemStatus & EV_RXCHAR)
    {
        // loop to wait for data
        do
        {
            // Read data from serial port
            ReadFile(hComm, &temp, 1, &dwBytesTransferred, 0);

            // copy into the byte array
            if(dwBytesTransferred == 1)
            {
                readBuf[i] = temp;
                i++;
            }
        }while(dwBytesTransferred == 1 && i < BUFSIZE);
    }


}

// close the RS232 port
void CloseRS232Port()
{
    CloseHandle(hComm);
}
